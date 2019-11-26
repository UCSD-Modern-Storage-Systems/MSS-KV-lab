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

run: build fillseq fillrandom overwrite readseq readrandom deleteseq deleterandom summarize

build:
	make clean
	make

fillseq:
	numactl -N 0 -m 0 ./bin/bench --benchmarks=fillseq --db_size_in_gb=4 --threads=4 --num=500000 --value_size=100 | tee fillseq_100.txt
	numactl -N 0 -m 0 ./bin/bench --benchmarks=fillseq --db_size_in_gb=4 --threads=4 --num=50000 --value_size=1024 | tee fillseq_1024.txt

fillrandom:
	numactl -N 0 -m 0 ./bin/bench --benchmarks=fillrandom --db_size_in_gb=4 --threads=4 --num=500000 --value_size=100 | tee fillrandom_100.txt
	numactl -N 0 -m 0 ./bin/bench --benchmarks=fillrandom --db_size_in_gb=4 --threads=4 --num=50000 --value_size=1024 | tee fillrandom_1024.txt

overwrite:
	numactl -N 0 -m 0 ./bin/bench --benchmarks=fillrandom,overwrite --db_size_in_gb=4 --threads=4 --num=500000 --value_size=100 | tee overwrite_100.txt
	numactl -N 0 -m 0 ./bin/bench --benchmarks=fillrandom,overwrite --db_size_in_gb=4 --threads=4 --num=50000 --value_size=1024 | tee overwrite_1024.txt

readseq:
	numactl -N 0 -m 0 ./bin/bench --benchmarks=fillseq,readseq --db_size_in_gb=4 --threads=4 --num=500000 --value_size=100 | tee readseq_100.txt
	numactl -N 0 -m 0 ./bin/bench --benchmarks=fillseq,readseq --db_size_in_gb=4 --threads=4 --num=50000 --value_size=1024 | tee readseq_1024.txt

readrandom:
	numactl -N 0 -m 0 ./bin/bench --benchmarks=fillrandom,readrandom --db_size_in_gb=4 --threads=4 --num=500000 --value_size=100 | tee readrandom_100.txt
	numactl -N 0 -m 0 ./bin/bench --benchmarks=fillrandom,readrandom --db_size_in_gb=4 --threads=4 --num=50000 --value_size=1024 | tee readrandom_1024.txt

deleteseq:
	numactl -N 0 -m 0 ./bin/bench --benchmarks=fillseq,deleteseq --db_size_in_gb=4 --threads=4 --num=500000 --value_size=100 | tee deleteseq_100.txt
	numactl -N 0 -m 0 ./bin/bench --benchmarks=fillseq,deleteseq --db_size_in_gb=4 --threads=4 --num=50000 --value_size=1024 | tee deleteseq_1024.txt

deleterandom:
	numactl -N 0 -m 0 ./bin/bench --benchmarks=fillrandom,deleterandom --db_size_in_gb=4 --threads=4 --num=500000 --value_size=100 | tee deleterandom_100.txt
	numactl -N 0 -m 0 ./bin/bench --benchmarks=fillrandom,deleterandom --db_size_in_gb=4 --threads=4 --num=50000 --value_size=1024 | tee deleterandom_1024.txt

summarize:
	python summarize.py perf.csv

.PHONY: clean

clean:
	rm -rf bin
