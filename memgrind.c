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


double grindA(){
	int i, count;
	char *pointers[3000];
	double diff; 
	struct timespec start, end;

	//printf("Beginning grind process A \n");
	initialize();

	clock_gettime(CLOCK_MONOTONIC, &start); /* mark start time */
	//Perfoming 3000 1 byte mallocs
	for (i = 0; i < 3000; i++){
		pointers[i] = mymalloc(1);
		if (pointers[i] != NULL)
			count++;
	}
	//freeing all pointers
	for (i = 0; i < 3000; i++){
		myfree(pointers[i]);
	}
	clock_gettime(CLOCK_MONOTONIC, &end); /* mark the end time */
	

	diff = (double) (((end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec))/BILLION; 
	/*
	printf("\n");
	printf("\n");
	printf("\n");
	printf("NUMBER OF SUCCESSFUL MALLOCS: %d \n", count);
	printf("\n");
	printf("\n");
	printf("\n");
	printf("elapsed time grindA= %.5f seconds\n",  diff);	
	*/
	return diff;
}


double grindB(){
	int i, count;
	char *pointers[3000];
	double diff; 
	struct timespec start, end;

	initialize();
	clock_gettime(CLOCK_MONOTONIC, &start);
	//A single byte malloc followed by 3000 frees
	char * test  = mymalloc(1);
	for (i = 0; i < 3000; i++)
		myfree(test);

	clock_gettime(CLOCK_MONOTONIC, &end); /* mark the end time */
	

	diff = (double) (((end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec))/BILLION; 
	/*
	printf("\n");
	printf("\n");
	printf("\n");
	//printf("NUMBER OF SUCCESSFUL MALLOCS: \n");
	printf("\n");
	printf("\n");
	printf("\n");
	//printf("elapsed time grindB= %.5f second\n",  diff);	
	*/
	return diff;
}




double grindC(){
	int MAX = 3000;
	char * pointers[3000];
	int mallocCount, freeCount, randNum;
	double diff; 
	struct timespec start, end;



	mallocCount = freeCount = 0;

	clock_gettime(CLOCK_MONOTONIC, &start);
	while(mallocCount < MAX){	//until we hit 3000 mallocs
		randNum = rand();
		randNum = randNum % 2;
		if (randNum == 0){		//if remainder is zero, mallloc and increas count
			pointers[mallocCount] = mymalloc(1);
			mallocCount++;
		}
		else{	//check if the freeCountis before malloc. then it is ok to free
			if (freeCount < mallocCount){
				myfree(pointers[freeCount]);
				freeCount++;
			}
		}

	}


	while(freeCount < MAX){
		myfree(pointers[freeCount]);
		freeCount++;
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	diff = (double) (((end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec))/BILLION;

	/*
	printf("\n");
	printf("\n");
	printf("\n");
	//printf("elapsed time grindC= %.5f second\n",  diff);
	*/
	return diff;
}


double grindD(){
	int MAX = 3000;
	char * pointers[3000];
	int mallocCount, freeCount, randNum;
	double diff; 
	struct timespec start, end;



	mallocCount = freeCount = 0;

	clock_gettime(CLOCK_MONOTONIC, &start);
	while(mallocCount < MAX){	//until we hit 3000 mallocs
		randNum = rand();
		randNum = randNum % 2;
		if (randNum == 0){		//if remainder is zero, mallloc and increas count
			pointers[mallocCount] = mymalloc(rand() % 3000);
			mallocCount++;
		}
		else{	//check if the freeCountis before malloc. then it is ok to free
			if (freeCount < mallocCount){
				myfree(pointers[freeCount]);
				freeCount++;
			}
		}

	}


	while(freeCount < MAX){
		myfree(pointers[freeCount]);
		freeCount++;
	}
	clock_gettime(CLOCK_MONOTONIC, &end);
	diff = (double) (((end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec))/BILLION;

	/*
	printf("\n");
	printf("\n");
	printf("\n");
	printf("elapsed time grindD= %.5f second\n",  diff);
	*/
	return diff;
}


int main(){
	double timeA, timeB, timeC, timeD, sum, i;
	sum = 0;
	srand(time(NULL));

	//One-hundred grindA
	for (i = 0; i < 100; i++){
		timeA = grindA();
		sum += timeA;

	}

	sum = sum / 100;
	printf("The average time for 100 grindA is: %.5f \n", sum);
	printf("\n");
	printf("\n");
	
	//One Hundred grindB
	sum = 0;
	for (i = 0; i < 100; i++){
		timeB = grindB();
		sum += timeB;

	}

	sum = sum / 100;
	printf("The average time for 100 grindB is: %.5f \n", sum);
	printf("\n");
	printf("\n");
	

	//One hundred grindC
	sum = 0;
	for (i = 0; i < 100; i++){
		timeC = grindC();
		sum += timeC;

	}

	sum = sum / 100;
	printf("The average time for 100 grindC is: %.5f \n", sum);
	printf("\n");
	printf("\n");	


	//One hundred grindD
	sum = 0;
	for (i = 0; i < 100; i++){
		timeD = grindD();
		sum += timeD;

	}
	
	sum = sum / 100;
	printf("The average time for 100 grindD is: %.5f \n", sum);
	printf("\n");
	printf("\n");	


	return 0;
}
