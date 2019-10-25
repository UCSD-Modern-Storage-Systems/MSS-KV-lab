GPP = g++
GPPFLAGS = -O3 -std=c++11 -w

LIB_HOME = ../../lib

# gtest
GTEST_HOME = /usr/src/gtest
GTEST_INC = $(GTEST_HOME)/include
GTEST_LIB = $(GTEST_HOME)/gtest

# pmemkv
PMEMKV_INC = $(LIB_HOME)/pmemkv/src
PMEMKV_LIB = $(LIB_HOME)/pmemkv/build

# include
INC_DIR = ../include $(GTEST_INC) $(PMEMKV_INC)
INC_PATH = $(foreach d, $(INC_DIR), -I$d)

# library
LIB_DIR = ../src $(GTEST_LIB) $(PMEMKV_LIB)
LIB_PATH = $(foreach d, $(LIB_DIR), -L$d)
LIBS = -l:libpmkv.a -pthread -lgtest -lpmemkv

all: basic_test

%: %.cc
	mkdir -p bin
	$(GPP) $(GPPFLAGS) $(INC_PATH) -o bin/$@ $^ $(LIB_PATH) $(LIBS)

run:
	make clean
	make
	./bin/basic_test

.PHONY: clean

clean:
	rm -rf bin
