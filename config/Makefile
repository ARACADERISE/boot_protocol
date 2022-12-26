.PHONY: build
.PHONY: clean

CC = gcc
HIDE = @

build:
	$(HIDE)$(CC) ../YamlParser/src/parser.c ../YamlParser/src/data.c ../YamlParser/src/lexer.c config.c -o ../bin/main.o
	$(HIDE)./../bin/main.o
clean:
	rm -rf *.o