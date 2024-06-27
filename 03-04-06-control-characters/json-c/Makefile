PWD := $(shell pwd)

all: run-docker

build-docker:
	docker build -t crossy-json-c-build .

run-docker: build-docker
	docker run --rm -it \
		-v ${PWD}/:/app \
		crossy-json-c-build \
		/bin/bash -c "make -f Makefile.docker"

clean:
	rm -rf build