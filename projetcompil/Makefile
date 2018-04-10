
CC=gcc
CFLAGS=-g -std=c11 -pedantic -Wall


all : test

test : test.o typetree.o exprtree.o envi.o typecontr.o

test.o : test.c typetree.c typetree.h mltype.h

typetree.o: typetree.c typetree.h mltype.h

exprtree.o: exprtree.c typetree.h envi.h typecontr.h mloper.h mltype.h

envi.o: envi.c envi.h exprtree.h

typecontr.o: typecontr.c typecontr.h typetree.h

clean:
	-rm $(PROG) *.o lex.yy.* $(PROG).tab.* *.err *.output *.out *.dot

