PWD := $(shell pwd)

all: run-docker

build-docker:
	docker build -t crossy-json-parser-build .

run-docker: build-docker
	docker run --rm -it \
		-v ${PWD}/:/app \
		crossy-json-parser-build \
		/bin/bash -c "make -f Makefile.docker"