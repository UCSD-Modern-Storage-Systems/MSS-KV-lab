GCC = gcc
GCCFLAGS =

ifeq ($(DEBUG), 1)
GCCFLAGS = -g -O0
endif

# pmemkv
PMEMKV_HOME = ../lib/pmemkv
PMEMKV_INC = $(PMEMKV_HOME)/src
PMEMKV_LIB = $(PMEMKV_HOME)/build

INC_DIR = ../include $(PMEMKV_INC)
INC_PATH = $(foreach d, $(INC_DIR), -I$d)

LIB_DIR = ./ $(PMEMKV_LIB)
LIB_PATH = $(foreach d, $(LIB_DIR), -L$d)

LIBS = -lpmemkv -pthread

STATIC_LIB = libpmkv.a
EXAMPLE_STATIC_LIB = libpmkv_ex.a

all: lib

lib: $(STATIC_LIB) $(EXAMPLE_STATIC_LIB)

%.o: %.c
	$(GCC) $(GCCFLAGS) $(INC_PATH) -c $< -o $@ $(LIB_PATH) $(LIBS)

$(STATIC_LIB): pmkv.o
	ar rsv $@ $<

# example
example: $(EXAMPLE_STATIC_LIB)

$(EXAMPLE_STATIC_LIB): pmemkv.o
	ar rsv $@ $<


.PHONY: clean

clean:
	rm -f *.o $(STATIC_LIB) $(EXAMPLE_STATIC_LIB)
