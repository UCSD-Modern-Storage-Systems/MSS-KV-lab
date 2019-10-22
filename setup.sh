#!/bin/bash

build_core=8
cwd=$PWD
mkdir -p $cwd/lib

# Install gtest
cd /usr/src/gtest && \
	cmake CMakeLists.txt && \
	make -j$build_core && \
	cp *.a /usr/lib

# Install PMDK
cd $cwd/lib
git clone https://github.com/pmem/pmdk && \
	cd pmdk && \
	git checkout --track origin/stable-1.7 && \
	make -j$build_core && \
	make install

# Install libpmemobj-cpp (version: stable-1.8)
cd $cwd/lib
git clone https://github.com/pmem/libpmemobj-cpp && \
	cd libpmemobj-cpp && \
	git checkout --track origin/stable-1.8 && \
	mkdir build && \
	cd build && \
	cmake .. && \
	make -j$build_core && \
	make install

# Install memkind
# The extra stuff in front of ./build.sh prevents running the tests.
cd $cwd/lib
git clone https://github.com/memkind/memkind && \
	cd memkind && \
	MAKEOPTS=check_PROGRAMS= ./build.sh && \
	make install

# Build pmemkv (version: stable-1.0)
cd $cwd/lib
git clone https://github.com/pmem/pmemkv && \
	cd pmemkv && \
	git checkout --track origin/stable-1.0 && \
	mkdir build && \
	cd build && \
	cmake .. -DENGINE_CMAP=ON -DENGINE_STREE=ON -DENGINE_TREE3=ON && \
	make -j$build_core && \
	make install
