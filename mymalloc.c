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

	return ((char*)(bp) + getSize(getHeader(bp)) - DSIZE);

}

// return pointer to next block given block pointer
void* nextBlock(void* bp){

	return ((char*)(bp) + getSize(getHeader(bp)));

}

// return pointer to previous block given block pointer
void* prevBlock(void* bp){

	char* ptr = (char*)(bp) - DSIZE;
	return ((char*)ptr - getSize(ptr) + DSIZE);

}

static char myblock[131072];


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

// combines consecutive free blocks
static void* coalesce(void* bp){

	printf("Attempting to coalesce blocks of size %d, %d, %d\n", getSize(getFooter(prevBlock(bp))), getSize(getHeader(bp)), getSize(getHeader(nextBlock(bp))));
	printf("Where prev footer is at %d and next header is at %d\n", (char*)getFooter(prevBlock(bp)) - (char*)myblock, (char*)getHeader(nextBlock(bp)) - (char*)myblock);
	unsigned int prev_alloc = getAlloc(getFooter(prevBlock(bp)));
	unsigned int next_alloc = getAlloc(getHeader(nextBlock(bp)));
	unsigned int size = getSize(getHeader(bp));
	printf("PREV ALLOC IS %d, NEXT ALLOC IS %d\n", prev_alloc, next_alloc);



	if (prev_alloc && next_alloc){ 					// CASE 1
		return bp;
	}
	else if (prev_alloc && !next_alloc){			// CASE 2
		size += getSize(getHeader(nextBlock(bp)));
		put(getHeader(bp), pack(size, 0));
		put(getFooter(nextBlock(bp)), pack(size, 0));
	}
	else if (!prev_alloc && next_alloc){			// CASE 3
		if ((char*)(bp) - (char*)myblock == DSIZE){
			size += 0;
			put(getFooter(bp), pack(size, 0));
			put(getHeader(bp), pack(size, 0));
		}
		else {
			size += getSize(getFooter(prevBlock(bp)));
			put(getFooter(bp), pack(size, 0));
			put(getHeader(prevBlock(bp)), pack(size, 0));
			bp = prevBlock(bp);
		}

	}
	else { 											// CASE 4
		if ((char*)(bp) - (char*)myblock == DSIZE){
			size += getSize(getHeader(nextBlock(bp)));
			put(getHeader(bp), pack(size, 0));
			put(getFooter(nextBlock(bp)), pack(size, 0));

		}
		else {
			size += getSize(getHeader(nextBlock(bp))) + getSize(getFooter(prevBlock(bp)));
			put(getHeader(prevBlock(bp)), pack(size, 0));
			put(getFooter(nextBlock(bp)), pack(size, 0));
			bp = prevBlock(bp);
		}

	}
	return bp;

}

// frees allocated memory given block pointer bp
void myfree(void* bp, int a, int b){

	printf("\nBeginning free process\n");

	size_t size = getSize(getHeader(bp));

	put(getHeader(bp), pack(size, 0));
	printf("Address at %p, myblock starts at %p, relative address is %d\n", (char*)(bp), (char*)myblock, (char*)(bp) - (char*)myblock);
	printf("relative address of header is %d, relative address of footer is %d\n", (char*)(getHeader(bp)) - (char*)myblock, (char*)(getFooter(bp)) - (char*)myblock);
	put(getFooter(bp), pack(size, 0));


	coalesce(bp);
	printf("Coalesce complete, free successful!\n\n");

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
	if ((csize - size) > (2*DSIZE)){
		put(getHeader(bp), pack(size, 1));
		put(getFooter(bp), pack(size, 1));
		printf("relative address of header is %d, relative address of footer is %d\n", (char*)(getHeader(bp)) - (char*)myblock, (char*)(getFooter(bp)) - (char*)myblock);

		bp = nextBlock(bp);
		put(getHeader(bp), pack(csize-size, 0));
		put(getFooter(bp), pack(csize-size, 0));
		printf("relative address of new header is %d, relative address of new footer is %d\n", (char*)(getHeader(bp)) - (char*)myblock, (char*)(getFooter(bp)) - (char*)myblock);
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
	int adjSize = 4 - (size % 4);
	asize = size + adjSize;
	asize += 2*WORDSIZE;
	printf("%d %d %d %d\n", size, adjSize, size + adjSize, asize);

	// search the free list for a fit
	if ((bp = findFit(asize)) != NULL){
		printf("Relative address of bp is %d\n", (char*)(bp) - (char*)myblock);
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

	char *wilbur[3000];
		int i;
		for (i = 0; i < 3000; i++){
			wilbur[i] = (char*)malloc(sizeof(char));
			*(wilbur[i]) = 'a';
		}
		for (i = 0; i < 3000; i++){
			free(wilbur[i]);
		}

}

