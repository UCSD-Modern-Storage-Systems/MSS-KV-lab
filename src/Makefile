GCC = gcc
GCCFLAGS =

ifeq ($(DEBUG), 1)
GCCFLAGS = -g -O0
endif

# pmemkv
PMEMKV_HOME = ../lib/pmemkv
PMEMKV_INC = $(PMEMKV_HOME)/src
PMEMKV_LIB = $(PMEMKV_HOME)/build

# PMDK
PMDK_HOME = ../lib/pmdk
PMDK_INC = $(PMDK_HOME)/src/include
ifeq ($(DEBUG), 1)
PMDK_LIB = $(PMDK_HOME)/src/debug
else
PMDK_LIB = $(PMDK_HOME)/src/nondebug
endif

INC_DIR = ../include $(PMEMKV_INC) $(PMDK_INC)
INC_PATH = $(foreach d, $(INC_DIR), -I$d)

LIB_DIR = ./ $(PMEMKV_LIB) $(PMDK_LIB)
LIB_PATH = $(foreach d, $(LIB_DIR), -L$d)

LIBS = -lpmemkv -pthread -lpmemobj

PMKV_OBJ = pmkv.o
ifeq ($(PMEMKV), 1)
PMKV_OBJ = pmemkv.o
endif

STATIC_LIB = libpmkv.a

all: lib libpmemobj_example

lib: $(STATIC_LIB)

%.o: %.c
	$(GCC) $(GCCFLAGS) $(INC_PATH) -c $< -o $@ $(LIB_PATH) $(LIBS)

libpmemobj_example: libpmemobj_example.c
	$(GCC) $(GCCFLAGS) $(INC_PATH) -o $@ $< $(LIB_PATH) $(LIBS)

$(STATIC_LIB): $(PMKV_OBJ)
	ar rsv $@ $<

.PHONY: clean

clean:
	rm -f *.o $(STATIC_LIB) libpmemobj_example
