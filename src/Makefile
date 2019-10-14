GCC = gcc
GCCFLAGS =

ifeq ($(DEBUG), 1)
GCCFLAGS = -g -O0
endif

# pmemkv
PMEMKV_HOME = ../lib/pmemkv
PMEMKV_INC = $(PMEMKV_HOME)/src
PMEMKV_LIB = $(PMEMKV_HOME)/build

INC_DIR = ./ $(PMEMKV_INC)
INC_PATH = $(foreach d, $(INC_DIR), -I$d)

LIB_DIR = ./ $(PMEMKV_LIB)
LIB_PATH = $(foreach d, $(LIB_DIR), -L$d)

LIBS = -lpmemkv -pthread

all: pmkv_test

pmkv_test: pmkv_test.c pmemkv.c
	$(GCC) $(GCCFLAGS) $(INC_PATH) -o $@ $^ $(LIB_PATH) $(LIBS)

%: %.c
	$(GCC) $(GCCFLAGS) $(INC_PATH) -o $@ $< $(LIB_PATH) $(LIBS)

.PHONY: clean

clean:
	rm -rf pmkv_test
