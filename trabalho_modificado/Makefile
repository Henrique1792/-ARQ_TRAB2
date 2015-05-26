all: main.o screen.o debug.o booktag.o
	gcc -g -Wall -o trabalho main.o screen.o debug.o booktag.o

screen.o:
	gcc -Wall -g -c screen.c

debug.o:
	gcc -Wall -g -c debug.c

booktag.o:
	gcc -Wall -g -c booktag.c

run:
	./trabalho

clean:
	rm -rf *.o trabalho
