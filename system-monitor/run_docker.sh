#!/bin/bash
echo "prog$1"
docker run -u $(id -u):$(id -g) \
    -p 0.0.0.0:6006:6006 \
	-v /Users/$(whoami):/home/$(whoami) \
	-w /home/$(whoami)/ --rm -it \
	--entrypoint=/bin/bash $1
