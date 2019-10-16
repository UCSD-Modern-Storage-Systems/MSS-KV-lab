# Base image
FROM ubuntu:18.04
ARG build_core
ARG project_home

RUN echo "Build using $build_core cores"

# Install prerequisites: https://github.com/pmem/pmemkv/blob/master/INSTALLING.md#ubuntu
RUN apt update && apt install -y \
	autoconf \
	automake \
	build-essential \
	cmake libdaxctl-dev \
	libndctl-dev \
	libnuma-dev \
	libtbb-dev \
	libtool \
	rapidjson-dev \
	git \
	pkg-config \
	vim

RUN apt install -y ndctl

# Create code structure
RUN mkdir -p $project_home && cd $project_home && mkdir src && mkdir include && mkdir lib && mkdir test && mkdir bench

# Copy basic codes
ADD src $project_home/src
ADD include $project_home/include
ADD test $project_home/test
ADD bench $project_home/bench
ADD Makefile.docker $project_home/Makefile

# Install PMDK
RUN cd $project_home/lib && git clone https://github.com/pmem/pmdk && \
	cd pmdk && \
	make -j$build_core && \
	make install

# Install libpmemobj-cpp
RUN cd $project_home/lib && git clone https://github.com/pmem/libpmemobj-cpp && \
	cd libpmemobj-cpp && \
	mkdir build && \
	cd build && \
	cmake .. && \
	make -j$build_core && \
	make install

# Install memkind
# The extra stuff in front of ./build.sh prevents running the tests.
RUN cd $project_home/lib && git clone https://github.com/memkind/memkind && \
	cd memkind && \
	MAKEOPTS=check_PROGRAMS= ./build.sh && \
	make install

# Install gtest
RUN apt install -y libgtest-dev
RUN cd /usr/src/gtest && \
	cmake CMakeLists.txt && \
	make -j$build_core && \
	cp *.a /usr/lib

# Build pmemkv
RUN cd $project_home/lib && \git clone https://github.com/pmem/pmemkv && \
	cd pmemkv && \
	mkdir build && \
	cd build && \
	cmake .. -DENGINE_CMAP=ON -DENGINE_STREE=ON -DENGINE_TREE3=ON && \
	make -j$build_core

# Set shared lib path
ENV LD_LIBRARY_PATH=$project_home/lib/pmemkv/build

# Go to project home after login
RUN echo "cd $project_home" >> ~/.bashrc
