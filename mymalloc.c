
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"

void test(){
	printf("Entered the void method\n");
	memvoid();
}

typedef struct{
	unsigned short isAllocated;
	size_t size;
} MemStruct;

static char myblock[5000];

void* mymalloc(size_t size){

	MemStruct* tempStruct = (MemStruct*)myblock;
	if (tempStruct->size == 0){ 																					// the memory is empty
		tempStruct->isAllocated = 0;
		tempStruct->size = sizeof(myblock) - sizeof(MemStruct);
		if ((size <= tempStruct->size) && (size > (tempStruct->size - sizeof(MemStruct) - 1))){ 					// use the whole memory block for the request
			tempStruct->isAllocated = 1;
			printf("Entire memory block used for request\n");
			return tempStruct;
		}
		else if (size <= (tempStruct->size - sizeof(MemStruct) - 1)){ 												// create block of given size, dividing memory in two
        	MemStruct* newStruct = (MemStruct*)(tempStruct + sizeof(MemStruct) + size);
        	newStruct->isAllocated = 0;
        	newStruct->size = tempStruct->size - sizeof(MemStruct) - size;
        	tempStruct->size = size;
            tempStruct->isAllocated = 1;
            printf("%d\n", tempStruct->size);
            printf("%d\n", newStruct->size);
            return tempStruct;
        }
        else {
        	printf("Cannot fulfill allocation request:  Not enough memory\n");
        	return NULL;
        }
	}
	else {

	}

	return NULL;


}

void myfree(void* ptr){

}

int main(){
	mymalloc(4968);
	return 0;
}

