CC=gcc
CFLAGS=-Wall -Wextra -O2 -std=gnu11
OBJECTS=main.o emitter.o error.o init.o lexer.o parser.o symbol.o
OUT=comp

${OUT}: ${OBJECTS}
	${CC} -o $@ $^

${OBJECTS}: global.h 

global.h: y.tab.h
y.tab.h: parser.c

parser.c: parser.y
	bison --defines=y.tab.h --warning=all  -o $@ $^

lexer.c: lexer.l
	flex -o $@ $^

.PHONY: clean

clean:
	-rm -rf ${OUT} ${OBJECTS} lexer.c parser.c y.tab.h
