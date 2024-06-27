PWD := $(shell pwd)

all: run-docker

build-docker:
	docker build -t crossy-jsmn-build .

run-docker: build-docker
	docker run --rm -it \
		-v ${PWD}/:/app \
		crossy-jsmn-build \
		/bin/bash -c "make -f Makefile.docker"

clean:
	rm -rf build