# Makefile for comp-project
# make WIN=(0|32|64) DEBUG=[01]
# make // defaults: WIN=0 DEBUG=0

WIN ?= 0
ifeq ($(WIN), 32)
	CC=i686-w64-mingw32-g++
	OUT=xcomp-win32.exe
else ifeq ($(WIN), 64)
	CC=x86_64-w64-mingw32-g++
	OUT=xcomp-win64.exe
else
	CC=clang++
	OUT=xcomp
endif

DEBUG ?= 0
ifeq ($(DEBUG), 0)
	CPPFLAGS=-Wall -Wextra -std=gnu++11 -O2
	LDFLAGS=-s -static
else
	CPPFLAGS=-Wall -Wextra -std=gnu++11 -O0 -g -ggdb
	LDFLAGS=
endif

SOURCES=main.cpp node.cpp inter.cpp error.cpp parser.cpp lexer.cpp
HEADERS=main.hpp node.hpp inter.hpp error.hpp parser.m.hpp lexer.hpp
OBJECTS=$(SOURCES:.cpp=.o)

.PHONY: dist clean clear

.cpp.o:
	$(CC) $(CPPFLAGS) -c -o $@ $<

${OUT}: $(OBJECTS)
	$(CC) $(CPPFLAGS) $(LDFLAGS) -o $@ $^

${OBJECTS}: global.hpp $(HEADERS)

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
