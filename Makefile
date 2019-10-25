cwd = $(shell pwd)

image:
	sudo docker build -t nvsl/pmkv:latest .

run:
	mkdir -p data
	sudo docker run --rm --privileged -it -v $(cwd):/home/cse291a -v $(cwd)/data:/mnt/ramdisk nvsl/pmkv:latest /bin/bash
