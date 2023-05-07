.PHONY: build
.PHONY: clean

CC = gcc
HIDE = @

build:
	$(HIDE)$(CC) ../YamlParser/parser.c ../YamlParser/data.c ../YamlParser/lexer.c main.c -o ../bin/config.o
	$(HIDE)./../bin/config.o mbr
	$(HIDE)cd ..

eve:
	$(HIDE)./../bin/config.o eve
	$(HIDE)cd ..

clean:
	rm -rf *.o