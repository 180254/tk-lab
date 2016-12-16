WIN ?= 0
ifeq ($(WIN), 0)
	CC=clang++
	OUT=xcomp-elf32
else
	CC=i686-w64-mingw32-g++
	OUT=xcomp-win32.exe
endif

DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CPPFLAGS=-Wall -Wextra -std=gnu++11 -O0 -g -ggdb
	LDFLAGS=
else
	CPPFLAGS=-Wall -Wextra -std=gnu++11 -O2
	LDFLAGS=-s -static
endif

SOURCES=main.cpp node.cpp inter.cpp error.cpp parser.cpp lexer.cpp
OBJECTS=$(SOURCES:.cpp=.o)

.PHONY: dist clean clear

.cpp.o:
	$(CC) $(CPPFLAGS) -c -o $@ $<

${OUT}: $(OBJECTS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

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
		parser.cpp parser.hpp parser.output lexer.cpp *.asm

clear: clean
