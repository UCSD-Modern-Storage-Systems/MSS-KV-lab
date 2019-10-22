# Base image
FROM ubuntu:18.04
ARG project_home

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
	vim \
	ndctl \
	valgrind \
	libgtest-dev

# Create structure
RUN mkdir -p $project_home && cd $project_home

# Set shared lib path
ENV LD_LIBRARY_PATH=/usr/local/lib

# Make programs using PMDK assume the underlying storage is PMEM
ENV PMEM_IS_PMEM_FORCE=1

# Create /mnt/ramdisk
RUN mkdir -p /mnt/ramdisk && chmod -R 777 /mnt/ramdisk

# Go to project home after login
RUN echo "cd $project_home" >> ~/.bashrc
