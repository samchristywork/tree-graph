CC=gcc
CFLAGS=-Isrc/
LIBS=-lcairo -lm

all: build/tree-render

.PHONY: objects
objects: $(patsubst src/%.c, build/%.o, $(wildcard src/*.c))

build/%.o: src/%.c
	mkdir -p build
	$(CC) -c $(CFLAGS) $< -o $@

build/tree-render: build/main.o build/draw.o build/tree.o
	${CC} build/*.o ${LIBS} -o $@

.PHONY: sample
sample: all
	./build/tree-render -o sample.png < assets/sample.txt
	@echo "Sample output written to sample.png"

.PHONY: clean
clean:
	rm -rf build
