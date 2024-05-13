all: main.o movies.o
	gcc -g --std=gnu99 main.o movies.o -o movies

main.o: main.c
	gcc -g --std=gnu99 -c -Wall main.c
	
movies.o: movies.c
	gcc -g --std=gnu99 -c -Wall movies.c

clean:
	rm -f *.o main