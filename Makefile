cwd = $(shell pwd)
mnt = /mnt/ramdisk

image:
	sudo docker build -t nvsl/pmkv:latest .

run:
	sudo docker run --rm --privileged -it -v $(cwd):/home/cse291a -v $(mnt):/mnt/ramdisk nvsl/pmkv:latest /bin/bash
