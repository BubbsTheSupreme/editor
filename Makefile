output: main.o editor.o
	gcc editor.o main.o -lcurses -g -o edit
main.o: main.c
	gcc -c main.c
editor.o: editor.o editor.h
	gcc -c editor.c
clean:
	rm edit *.o