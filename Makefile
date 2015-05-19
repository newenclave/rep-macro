

all: r-macro.o
	gcc main.c r-macro.o -o r-macro -s -O2

r-macro.o: r-macro.c r-macro.h
	gcc -c r-macro.c -o r-macro.o
