.PHONY: build
.PHONY: clean

CC = gcc
HIDE = @

build:
	$(HIDE)$(CC) ../YamlParser/parser.c ../YamlParser/data.c ../YamlParser/lexer.c main.c -o ../bin/config.o
	$(HIDE)./../bin/config.o
	$(HIDE)cd .. && make run

clean:
	rm -rf *.o