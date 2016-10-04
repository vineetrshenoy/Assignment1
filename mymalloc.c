
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
int beenAccessed = 0;

void* mymalloc(size_t size){


	MemStruct* tempStruct = (MemStruct*)myblock;
	if (beenAccessed == 0){ 																					// the memory is empty
		beenAccessed = 1;
		tempStruct->isAllocated = 0;
		tempStruct->size = sizeof(myblock) - sizeof(MemStruct);
		if ((size <= tempStruct->size) && (size > (tempStruct->size - sizeof(MemStruct) - 1))){ 					// use the whole memory block for the request
			tempStruct->isAllocated = 1;
			printf("Entire memory block used for request\n");
			printf("Initial allocation complete...\n");
			return ++tempStruct;
		}
		else if (size <= (tempStruct->size - sizeof(MemStruct) - 1)){ 												// create block of given size, dividing memory in two
        	MemStruct* newStruct = (MemStruct*)(tempStruct + sizeof(MemStruct) + size);
        	newStruct->isAllocated = 0;
        	newStruct->size = tempStruct->size - sizeof(MemStruct) - size;
        	tempStruct->size = size;
            tempStruct->isAllocated = 1;
            printf("Initial allocation complete...\n\n");
            return ++tempStruct;
        }
        else {
        	printf("Cannot fulfill allocation request:  Not enough memory\n");
        	return NULL;
        }
	}
	else {

		int currentAddress = 0;
		// if the starting point has already been allocated
		// at this point, tempStruct is a pointer to the beginning of myblock

		while (tempStruct->isAllocated == 1 || size > tempStruct->size){

			printf("MemStruct at %d, size: %d %d\n", currentAddress, tempStruct->size, tempStruct->isAllocated);
			currentAddress += sizeof(MemStruct) + tempStruct->size;
			tempStruct = (MemStruct*)(tempStruct +  sizeof(MemStruct) + tempStruct->size);
			printf("Next to MemStruct at %d, size: %d %d\n\n", currentAddress, tempStruct->size, tempStruct->isAllocated);


		}
		printf("==== Proceed to allocation ====\n");
		// at this point tempStruct points to the first free memory that is large enough to hold the given size

		MemStruct* newStruct = (MemStruct*)(tempStruct + sizeof(MemStruct) + size);
		newStruct->isAllocated = 0;
		newStruct->size = tempStruct->size - sizeof(MemStruct) - size;

		printf("newStruct->size = %d\n", newStruct->size);
		tempStruct->size = size;
		tempStruct->isAllocated = 1;
		printf("tempStruct->size = %d\n\n", tempStruct->size);
		return ++tempStruct;

	}

	return NULL;


}

void myfree(void* ptr){

}

int main(){
	int* one = mymalloc(2000);
	char* two = mymalloc(500);
	double* three = mymalloc(600);
	float* four = mymalloc(1000);


	printf("\n================\n"
			"Process complete\n"
			"================\n");
	return 0;
}
