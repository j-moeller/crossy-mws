PWD := $(shell pwd)

all: run-docker

build-docker:
	docker build -t crossy-jansson-build .

run-docker: build-docker
	docker run --rm -it \
		-v ${PWD}/:/app \
		crossy-jansson-build \
		/bin/bash -c "make -f Makefile.docker"

clean:
	rm -rf build