CC=gcc
CFLAGS=-Isrc/
LIBS=-lcairo

all: build/tree-render

.PHONY: objects
objects: $(patsubst src/%.c, build/%.o, $(wildcard src/*.c))

build/%.o: src/%.c
	mkdir -p build
	$(CC) -c $(CFLAGS) $< -o $@

build/tree-render: build/main.o
	${CC} build/*.o ${LIBS} -o $@

.PHONY: clean
clean:
	rm -rf build
