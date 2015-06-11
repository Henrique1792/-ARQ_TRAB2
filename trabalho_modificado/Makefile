all: main.o screen.o debug.o booktag.o idc.o 
	gcc -g -Wall -o trabalho main.o screen.o debug.o booktag.o idc.o


main.o:
	gcc -Wall -g -c main.c

screen.o:
	gcc -Wall -g -c screen.c

debug.o:
	gcc -Wall -g -c debug.c

booktag.o:
	gcc -Wall -g -c booktag.c

idc.o:
	gcc -Wall -g -c idc.c

run:
	./trabalho

clean:
	rm -rf *.o trabalho

step:
	make clean
	make
	make run
