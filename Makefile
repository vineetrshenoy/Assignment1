	# This is a comment !
#
# ’myprogram’ is built from file1.c file2.c and file3.c
CC = gcc
CFLAGS = -Wall -g
OBJS = memgrind.o mymalloc.o

mymalloc: $(OBJS)
	$(CC) -g -o mymalloc $(OBJS) 

memgrind.o: memgrind.c mymalloc.h
	$(CC) -c -g memgrind.c 

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) -c -g mymalloc.c