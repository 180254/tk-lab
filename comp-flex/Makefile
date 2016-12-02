CC=gcc
CFLAGS=-Wall -Wextra -O2 -std=gnu11
OBJECTS=main.o emitter.o error.o init.o lexer.o parser.o symbol.o
OUT=comp

${OUT}: ${OBJECTS}
	${CC} -o $@ $^

${OBJECTS}: global.h 

lexer.c: lexer.l
	lex -o $@ $^

.PHONY: clean

clean:
	-rm -rf ${OUT} ${OBJECTS} lexer.c
