# Base image
FROM ubuntu:18.04

# prerequisites: https://github.com/pmem/pmemkv/blob/master/INSTALLING.md#ubuntu
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
	pkg-config

# PMDK
RUN git clone https://github.com/pmem/pmdk
RUN cd pmdk && make && make install

# libpmemobj-cpp
RUN git clone https://github.com/pmem/libpmemobj-cpp
RUN cd libpmemobj-cpp && mkdir build && cd build && cmake .. && make && make install

# memkind
# The extra stuff in front of ./build.sh prevents running the tests.
RUN git clone https://github.com/memkind/memkind
RUN cd memkind && MAKEOPTS=check_PROGRAMS= ./build.sh && make install

# gtest
RUN apt install -y libgtest-dev
RUN cd /usr/src/gtest && cmake CMakeLists.txt && make && cp *.a /usr/lib

# pmemkv
RUN git clone https://github.com/pmem/pmemkv
RUN cd pmemkv && mkdir build && cd build && cmake .. && make
