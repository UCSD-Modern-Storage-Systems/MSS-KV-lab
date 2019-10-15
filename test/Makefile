GPP = g++
GPPFLAGS = -O3 -std=c++11 -w

# gtest
GTEST_HOME = /usr/src/gtest
GTEST_INC = $(GTEST_HOME)/include
GTEST_LIB = $(GTEST_HOME)/build/lib

# include
INC_DIR = ./ $(GTEST_INC)
INC_PATH = $(foreach d, $(INC_DIR), -I$d)

# library
LIB_DIR = ./ $(GTEST_LIB)
LIB_PATH = $(foreach d, $(LIB_DIR), -L$d)
LIBS = -pthread -lgtest

all: basic_test

%: %.cc
	$(GPP) $(GPPFLAGS) $(INC_PATH) -o $@ $^ $(LIB_PATH) $(LIBS)

.PHONY: clean

clean:
	rm -f basic_test
