cwd = $(shell pwd)

image:
	sudo docker pull junokim8/mss-kv-lab:latest

run:
	mkdir -p data
	sudo docker run --rm --privileged -it -v "$(cwd)":/home/cse291a -v "$(cwd)/data":/mnt/ramdisk junokim8/mss-kv-lab:latest /bin/bash

test_all:
	make -C src clean
	make -C src
	make -C test run

bench_all:
	make -C src clean
	make -C src
	make -C test run
	make -C bench run
