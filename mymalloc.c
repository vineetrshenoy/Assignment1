/*
 * my_new_malloc.c
 *
 *  Created on: Oct 6, 2016
 *      Author: RyanMini
 */

// algorithm from textbook

#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

#define WORDSIZE 4
#define DSIZE (2*WORDSIZE)

#define malloc(x) mymalloc(x,__LINE__,__FILE__)
#define free(x)   myfree(x,__LINE__,__FILE__)

unsigned int pack(size, alloc){

	return (size | alloc);

}

// read a word at the given header pointer p
unsigned int get(void* hp){

	return *(unsigned int*)(hp);

}

// write a word to the given header pointer p
void put(void* hp, unsigned int val){

	*(unsigned int*)(hp) = val;

}

// returns the size of the block given header pointer p
unsigned int getSize(void* hp){

	return (get(hp) & ~0x1);

}

// return the allocation bit given header pointer p
unsigned int getAlloc(void* hp){

	return (get(hp) & 0x1);

}

// return the header pointer given block pointer
void* getHeader(void* bp){

	return ((char*)(bp) - WORDSIZE);

}

// returns the footer pointer given block pointer
void* getFooter(void* bp){

	return ((char*)(bp + getSize(getHeader(bp)) - DSIZE));

}

// return pointer to next block given block pointer
void* nextBlock(void* bp){

	return ((char*)(bp) + getSize(getHeader(bp)) - WORDSIZE);

}

// return pointer to previous block given block pointer
void* prevBlock(void* bp){

	return ((char*)(bp) - getSize(getHeader(bp)) - DSIZE);

}

static char myblock[16384];


void init(){

	char* ptr = myblock;
	// creating the prologue and epilogue blocks
	put(ptr, pack(0,1));
	put((char*)(ptr) + sizeof(myblock) - WORDSIZE, pack(0,1));

	// creating the initial free block space
	put((char*)(ptr) + WORDSIZE, pack(sizeof(myblock) - DSIZE, 0));
	put((char*)(ptr) + sizeof(myblock) - DSIZE, pack(sizeof(myblock) - DSIZE, 0));

	printf("\n========================\n"
			"Initialization complete!"
			"\n========================\n");

}

void myfree(void* ptr, int a, int b){


}

void* findFit(size_t size){

	char* bp;
	for (bp = (char*)myblock + DSIZE; getSize(getHeader(bp)) > 0; bp = nextBlock(bp)){
		printf("Size is: %d, Alloc is %d\n", getSize(getHeader(bp)), getAlloc(getHeader(bp)));
		if (!getAlloc(getHeader(bp)) && (size <= getSize(getHeader(bp)))){
			return bp;
		}
	}
	return NULL;
}

static void place(void* bp, size_t size){



	size_t csize = getSize(getHeader(bp));
	if ((csize - size) >= (2*DSIZE)){
		put(getHeader(bp), pack(size, 1));
		put(getFooter(bp), pack(size, 1));
		bp = nextBlock(bp);
		put(getHeader(bp), pack(csize-size, 0));
		put(getFooter(bp), pack(csize-size, 0));
	}
	else {
		put(getHeader(bp), pack(csize, 1));
		put(getFooter(bp), pack(csize, 1));
	}
}

void* mymalloc(size_t size, int a, int b){

	size_t asize;
	char* bp;

	// ignore spurious requests
	if (size <= 0){
		return NULL;
	}

	// adjust block size to include overhead and alignment requirements
	int adjSize = size % 4;
	asize = size + adjSize;
	asize += 2*WORDSIZE;

	// search the free list for a fit
	if ((bp = findFit(asize)) != NULL){
		place(bp, asize);
		printf("\nAllocation complete!\n");
		return bp;
	}

	// no fit found, report insufficient memory
	printf("\nRequest denied: insufficient memory\n");
	return NULL;
}




int main(){
	init();

	int* ptr = malloc(500);
	int* ptr2 = malloc(274);
	int* ptr3 = malloc(1234);

}

