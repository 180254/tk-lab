CC=clang++
CFLAGS=
CPPFLAGS=-Wall -Wextra -O0 -std=gnu++14 -g -ggdb

SOURCES=main.cpp node.cpp error.cpp parser.cpp lexer.cpp
OBJECTS=$(SOURCES:.cpp=.o)

OUT=xcomp

.PHONY: all clean

.cpp.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $< 

${OUT}: $(OBJECTS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^

global.hpp: main.hpp node.hpp error.hpp parser.m.hpp lexer.hpp

${OBJECTS}: global.hpp

parser.cpp parser.hpp: parser.y
	bison --warning=all --feature=all --defines=parser.hpp -o parser.cpp parser.y

lexer.cpp: lexer.l
	flex -o lexer.cpp lexer.l

all: $(OUT)

clean:
	-rm -f $(OUT) $(OBJECTS) parser.cpp parser.hpp parser.output lexer.cpp

