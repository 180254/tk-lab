CC=g++
CPPFLAGS=-Wall -Wextra -O2 -std=gnu++14
OBJECTS=main.o error.o parser.o lexer.o
OUT=xcomp

.PHONY: all clean

${OUT}: ${OBJECTS}
	${CC} -o $@ $^

${OBJECTS}: global.h parser.cpp

parser.cpp: parser.y
	bison --defines=parser.h --warning=all -o $@ $^

lexer.cpp: lexer.l
	flex -o $@ $^

all: ${OUT}

clean:
	-rm -rf ${OUT} ${OBJECTS} parser.cpp lexer.cpp parser.h
