.PHONY: all
all: format test build

.PHONY: format
format:
	clang-format src/* -i

.PHONY: build
build:
	mkdir -p build
	cd build && \
	cmake .. && \
	make -j$(noproc)

.PHONY: clean
clean:
	rm -rf build
