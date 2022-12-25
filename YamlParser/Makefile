.PHONY: run
.PHONY: build
.PHONY: clean

CC = gcc

run: build
	./main.o

build:
	$(CC) src/*.c -o main.o

clean:
	rm -rf *.o
