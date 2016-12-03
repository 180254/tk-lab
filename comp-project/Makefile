CC=g++
CFLAGS=
CPPFLAGS=-Wall -Wextra -O2 -std=gnu++14
OBJECTS=main.o error.o parser.o lexer.o node.o
OUT=xcomp

.PHONY: all clean

.cpp.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $< 
    
${OUT}: $(OBJECTS)
	$(CC) -o $@ $^

${OBJECTS}: global.h parser.h node.h

parser.cpp parser.h: parser.y
	bison --warning=all --feature=all --defines=parser.h -o parser.cpp parser.y

lexer.cpp: lexer.l
	flex -o lexer.cpp lexer.l

all: $(OUT)

clean:
	-rm -f $(OUT) $(OBJECTS) parser.cpp parser.h parser.output lexer.cpp
