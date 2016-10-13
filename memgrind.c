#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"
#include <time.h>
#include <stdint.h>

#define BILLION 1000000000L

void memvoid(){
	printf("Entered the memvoid method\n");
}


void grindA(){
	int i, count;
	char *pointers[3000];
	uint64_t diff; 
	struct timespec start, end;

	printf("Beginning grind process A \n");
	initialize();

	clock_gettime(CLOCK_MONOTONIC, &start); /* mark start time */
	for (i = 0; i < 3000; i++){
		pointers[i] = mymalloc(1);
		if (pointers[i] != NULL)
			count++;
	}

	for (i = 0; i < 3000; i++){
		myfree(pointers[i]);
	}
	clock_gettime(CLOCK_MONOTONIC, &end); /* mark the end time */
	

	diff =  ((end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec); 
	printf("\n");
	printf("\n");
	printf("\n");
	printf("NUMBER OF SUCCESSFUL MALLOCS: %d \n", count);
	printf("\n");
	printf("\n");
	printf("\n");
	printf("elapsed time grindB= %llu nanoseconds\n", (long long unsigned int) diff);	
}


void grindB(){
	int i, count;
	char *pointers[3000];
	uint64_t diff; 
	struct timespec start, end;

	initialize();
	clock_gettime(CLOCK_MONOTONIC, &start);

	char * test  = mymalloc(1);
	for (i = 0; i < 3000; i++)
		myfree(test);

	clock_gettime(CLOCK_MONOTONIC, &end); /* mark the end time */
	

	diff =  ((end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec); 
	printf("\n");
	printf("\n");
	printf("\n");
	printf("NUMBER OF SUCCESSFUL MALLOCS: \n");
	printf("\n");
	printf("\n");
	printf("\n");
	printf("elapsed time grindB = %llu nanoseconds\n", (long long unsigned int) diff);
}




int main(){
	grindA();
	grindB();
	return 0;
}
