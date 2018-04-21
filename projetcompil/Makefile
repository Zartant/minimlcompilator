SHELL=/bin/sh
LEX=flex
YACC=bison
CC=gcc
CFLAGS=-g -std=c11 -pedantic -Wall
LDFLAGS=-ly

LEXOPTS=-D_POSIX_SOURCE -DYY_NO_INPUT --nounput

all : test

test : test.o typetree.o exprtree.o envi.o typecontr.o

lexer.yy.c: lexer.l
	$(LEX) $(LEXOPTS) $<

test.o : test.c typetree.c typetree.h mltype.h

typetree.o: typetree.c typetree.h mltype.h

exprtree.o: exprtree.c typetree.h envi.h typecontr.h mloper.h mltype.h

envi.o: envi.c envi.h exprtree.h

typecontr.o: typecontr.c typecontr.h typetree.h

clean:
	-rm $(PROG) *.o lex.yy.* $(PROG).tab.* *.err *.output *.out *.dot

