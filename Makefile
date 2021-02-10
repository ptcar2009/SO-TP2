IDIR=inc
CC=gcc
CFLAGS=-lpthread -I$(IDIR) 
DFLAGS=-g -Wall -lpthread -I$(IDIR) -DDEBUG
BDIR=bin
ODIR=$(BDIR)/obj

SDIR=src
RDIR=$(BDIR)/release
DDIR=$(BDIR)/debug

SCRDIR=scripts

TARGET=main

_DEPS = $(shell ls $(IDIR))
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))
SRC := hashmap.c paginations.c pagination_map.c pager.c main.c
_OBJ = $(patsubst %.c, %.o, $(SRC))
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
OBJDEBUG = $(patsubst %,$(ODIR)/debug_%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $(RDIR)/$@.out $^ $(CFLAGS)


$(ODIR)/debug_%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(DFLAGS)
# debug compila o programa com flag de debugging para o g++
# e também retira a necessidade de colocar o número de iterações
# na chamada do comando. É útil para testes em valgrind e etc
debug: $(OBJDEBUG)
	$(CC) -o $(DDIR)/$(TARGET).out $^ $(DFLAGS)

.PHONY: test
test: debug
	sh $(SCRDIR)/test.sh

# run depende da variável de ambiente N_ITERATIONS
# para rodar o comando, coloque o comando como N_ITERATIONS=x make run
run: $(TARGET)
	. ./.env && ./$(RDIR)/main.out $$ALGORITHM $$TARGET_FILE $$PAGE_SIZE $$MEMORY_SIZE

.PHONY: clean
clean:
	rm -f bin/**/main.out
	rm -f bin/**/*.o
