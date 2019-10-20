GCC = gcc
GCCFLAGS =

ifeq ($(DEBUG), 1)
GCCFLAGS = -g -O0
endif

# PMDK
PMDK_HOME = ../lib/pmdk
PMDK_INC = $(PMDK_HOME)/src/include
ifeq ($(DEBUG), 1)
PMDK_LIB = $(PMDK_HOME)/src/debug
else
PMDK_LIB = $(PMDK_HOME)/src/nondebug
endif

INC_DIR = $(PMDK_INC)
INC_PATH = $(foreach d, $(INC_DIR), -I$d)

LIB_DIR = $(PMDK_LIB)
LIB_PATH = $(foreach d, $(LIB_DIR), -L$d)

LIBS = -pthread -lpmemobj

EXAMPLE_FILES = $(wildcard *.c)
TARGET = $(basename $(EXAMPLE_FILES))

all: $(TARGET)

%: %.c
	mkdir -p bin
	$(GCC) $(GCCFLAGS) $(INC_PATH) -o bin/$@ $< $(LIB_PATH) $(LIBS)

.PHONY: clean

clean:
	rm -rf bin
