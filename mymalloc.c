/*
 * my_new_malloc.c
 *
 *  Created on: Oct 6, 2016
 *      Author: RyanMini
 */

#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"

#define WORDSIZE 4
#define DSIZE (2*WORDSIZE)

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

static char myblock[10000];


void init(){

	char* ptr = myblock;
	printf("%p\n", ptr);

}

void myfree(void* ptr, int a, int b){


}

void* mymalloc(size_t size, int a, int b){
	return NULL;
}






int main(){

	init();

}

