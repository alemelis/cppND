#!/bin/bash
echo "prog$1"
docker run -u $(id -u):$(id -g) \
	-v /Users/$(whoami):/home/$(whoami) \
	-w /home/$(whoami)/ --rm -it \
	--entrypoint=/bin/bash $1
