OBJECTS=main.o emitter.o error.o init.o lexer.o parser.o symbol.o

comp: ${OBJECTS}
	cc -o comp ${OBJECTS}

main.o: main.c global.h
emitter.o: emitter.c global.h
error.o: error.c global.h
init.o: init.c global.h
lexer.o: lexer.c global.h
parser.o: parser.c global.h
symbol.o: symbol.c global.h

.PHONY: clean

clean:
	-rm comp ${OBJECTS}