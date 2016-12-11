CC=clang++
CFLAGS=
CPPFLAGS=-Wall -Wextra -std=gnu++14 -O0 -g -ggdb

SOURCES=main.cpp node.cpp inter.cpp error.cpp parser.cpp lexer.cpp
OBJECTS=$(SOURCES:.cpp=.o)

OUT=xcomp

.PHONY: dist clean clear

.cpp.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

${OUT}: $(OBJECTS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^

${OBJECTS}: global.hpp \
	main.hpp node.hpp inter.hpp error.hpp parser.m.hpp lexer.hpp

parser.cpp parser.hpp: parser.y
	bison --warning=all --feature=all \
	--defines=parser.hpp -o parser.cpp parser.y

lexer.cpp: lexer.l
	flex -o lexer.cpp lexer.l

dist: $(OUT)

clean:
	-rm -f $(OUT) $(OBJECTS) \
		parser.cpp parser.hpp parser.output lexer.cpp output*.asm

clear: clean
