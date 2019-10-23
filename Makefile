cwd = $(shell pwd)
mnt = /mnt/ramdisk

image:
	sudo docker build -t nvsl/pmkv:latest --build-arg project_home=/home/cse291a .

run:
	sudo docker run --rm --privileged -it -v $(cwd):/home/cse291a -v $(mnt):/mnt/ramdisk nvsl/pmkv:latest /bin/bash
