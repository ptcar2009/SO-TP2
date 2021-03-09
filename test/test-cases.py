import os
import time


algorithms = ["fifo", "2a", "lru", "ram", "mod"]
files = ["compilador.log", "simulador.log", "matriz.log", "compressor.log"]
page_sizes = [4,8,16,32,64]
board_sizes = [64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384]
csvfile = "test/output.csv"
f = open(csvfile, "w")
f.write("file,algorithm,page_size,board_size,reads,writes,page_faults,dirty,exec_time\n")

for algorithm in algorithms:
    for file in files:
        for page_size in page_sizes:
            for board_size in board_sizes:
               f.write(os.popen("./bin/release/main.out %s test/%s %s %s -1" % (algorithm, file, page_size, board_size)).read())

