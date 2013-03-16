GCCFLAGS = -Wall -Werror -std=gnu99 -g

all: init hydrogen oxygen

clean:
	rm oxygen hydrogen init sem_common.o

oxygen: sem_common.o oxygen.c
	gcc $(GCCFLAGS) -o oxygen oxygen.c sem_common.o

hydrogen: sem_common.o hydrogen.c
	gcc $(GCCFLAGS) -o hydrogen hydrogen.c sem_common.o

init: sem_common.o init.c
	gcc $(GCCFLAGS) -o init init.c sem_common.o

sem_common.o: sem_common.c sem_common.h
	gcc $(GCCFLAGS) -c sem_common.c
