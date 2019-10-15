GPP = g++
GPPFLAGS = -O3 -std=c++11 -w

# gtest
GTEST_HOME = /usr/src/gtest
GTEST_INC = $(GTEST_HOME)/include
GTEST_LIB = $(GTEST_HOME)/build/lib

# include
INC_DIR = ../include $(GTEST_INC)
INC_PATH = $(foreach d, $(INC_DIR), -I$d)

# library
LIB_DIR = ../src $(GTEST_LIB)
LIB_PATH = $(foreach d, $(LIB_DIR), -L$d)
LIBS = -l:libpmkv_ex.a -pthread -lgtest -lpmemkv

all: basic_test

%: %.cc
	$(GPP) $(GPPFLAGS) $(INC_PATH) -o $@ $^ $(LIB_PATH) $(LIBS)

.PHONY: clean

clean:
	rm -f basic_test
