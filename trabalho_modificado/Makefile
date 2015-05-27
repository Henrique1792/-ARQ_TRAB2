all: main.o screen.o debug.o booktag.o idx.o
	gcc -g -Wall -o trabalho main.o screen.o debug.o booktag.o

main.o:
	gcc -Wall -g -c main.c

screen.o:
	gcc -Wall -g -c screen.c

debug.o:
	gcc -Wall -g -c debug.c

booktag.o:
	gcc -Wall -g -c booktag.c

idx.o:
	gcc -Wall -g -c idc.c

run:
	./trabalho

clean:
	rm -rf *.o trabalho
