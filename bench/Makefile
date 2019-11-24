GPP = g++
GPPFLAGS = -O3 -std=c++11 -w -DOS_LINUX -fno-builtin-memcmp -march=native -DNDEBUG

LIB_HOME = ../../lib

# pmemkv
PMEMKV_INC = $(LIB_HOME)/pmemkv/src
PMEMKV_LIB = $(LIB_HOME)/pmemkv/build

# include
INC_DIR = ../include ./include ./ ./util $(PMEMKV_INC)
INC_PATH = $(foreach d, $(INC_DIR), -I$d)

# library
LIB_DIR = ../src $(PMEMKV_LIB)
LIB_PATH = $(foreach d, $(LIB_DIR), -L$d)
LIBS = -l:libpmkv.a -ldl -pthread -lpmemkv -lpmemobj

bench:
	mkdir -p bin
	g++ $(GPPFLAGS) ./bench.cc ./port/port_posix.cc ./util/env.cc ./util/env_posix.cc ./util/histogram.cc ./util/logging.cc ./util/status.cc ./util/testutil.cc -o bin/$@ $(INC_PATH) $(LIB_PATH) $(LIBS)

run: fillseq fillrandom overwrite readseq readrandom deleteseq deleterandom summarize

fillseq:
	./bin/bench --benchmarks=fillseq --threads=4 --num=1000000 --value_size=100 | tee fillseq_100.txt
	./bin/bench --benchmarks=fillseq --threads=4 --num=100000 --value_size=1024 | tee fillseq_1024.txt

fillrandom:
	./bin/bench --benchmarks=fillrandom --threads=4 --num=1000000 --value_size=100 | tee fillrandom_100.txt
	./bin/bench --benchmarks=fillrandom --threads=4 --num=100000 --value_size=1024 | tee fillrandom_1024.txt

overwrite:
	./bin/bench --benchmarks=fillrandom,overwrite --threads=4 --num=1000000 --value_size=100 | tee overwrite_100.txt
	./bin/bench --benchmarks=fillrandom,overwrite --threads=4 --num=100000 --value_size=1024 | tee overwrite_1024.txt

readseq:
	./bin/bench --benchmarks=fillseq,readseq --threads=4 --num=1000000 --value_size=100 | tee readseq_100.txt
	./bin/bench --benchmarks=fillseq,readseq --threads=4 --num=100000 --value_size=1024 | tee readseq_1024.txt

readrandom:
	./bin/bench --benchmarks=fillrandom,readrandom --threads=4 --num=1000000 --value_size=100 | tee readrandom_100.txt
	./bin/bench --benchmarks=fillrandom,readrandom --threads=4 --num=100000 --value_size=1024 | tee readrandom_1024.txt

deleteseq:
	./bin/bench --benchmarks=fillseq,deleteseq --threads=4 --num=1000000 --value_size=100 | tee deleteseq_100.txt
	./bin/bench --benchmarks=fillseq,deleteseq --threads=4 --num=100000 --value_size=1024 | tee deleteseq_1024.txt

deleterandom:
	./bin/bench --benchmarks=fillrandom,deleterandom --threads=4 --num=1000000 --value_size=100 | tee deleterandom_100.txt
	./bin/bench --benchmarks=fillrandom,deleterandom --threads=4 --num=100000 --value_size=1024 | tee deleterandom_1024.txt

summarize:
	python summarize.py perf.csv

.PHONY: clean

clean:
	rm -rf bin
