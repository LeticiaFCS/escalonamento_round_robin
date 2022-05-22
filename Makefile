HEADER = filas.h 

default: main

main.o: main.c $(HEADER)
	gcc -c main.c -o main.o

main: main.o
	gcc main.o -o main

clean:
	-rm -f main.o
	-rm -f main
