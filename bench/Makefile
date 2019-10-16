GPP = g++
GPPFLAGS = -O3 -std=c++11 -w -DOS_LINUX -fno-builtin-memcmp -march=native -DNDEBUG

# pmemkv
PMEMKV_INC = ../lib/pmemkv/src
PMEMKV_LIB = ../lib/pmemkv/build

# include
INC_DIR = ../include ./include ./ ./util $(PMEMKV_INC)
INC_PATH = $(foreach d, $(INC_DIR), -I$d)

# library
LIB_DIR = ../src $(PMEMKV_LIB)
LIB_PATH = $(foreach d, $(LIB_DIR), -L$d)
LIBS = -l:libpmkv_ex.a -ldl -pthread -lpmemkv

bench:
	g++ $(GPPFLAGS) ./db_bench.cc ./port/port_posix.cc ./util/env.cc ./util/env_posix.cc ./util/histogram.cc ./util/logging.cc ./util/status.cc ./util/testutil.cc -o $@ $(INC_PATH) $(LIB_PATH) $(LIBS)
