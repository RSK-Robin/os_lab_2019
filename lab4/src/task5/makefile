CC=gcc
CFLAGS=-I.

all : sequential_min_max parallel_min_max

task1 : parallel_min_max

task2 : 
	$(CC) -o task2 task2.c $(CFLAGS)

task3 : 
	$(CC) -o task3 process_memory.c $(CFLAGS)

task55 : utils.o sum.o
	$(CC) -std=gnu99 -pthread -o task51 utils.o sum.o parallel_sum.c $(CFLAGS)

sequential_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o sequential_min_max find_min_max.o utils.o sequential_min_max.c $(CFLAGS)

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -std=gnu99 -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

sum.o : sum.h
	$(CC) -std=gnu99 -o sum.o -c sum.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

clean :
	rm utils.o find_min_max.o sequential_min_max parallel_min_max
