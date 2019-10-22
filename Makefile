image:
	sudo docker build -t nvsl/pmkv:latest --build-arg project_home=/home/cse291a .

run:
	sudo docker run --privileged -i -t nvsl/pmkv:latest
