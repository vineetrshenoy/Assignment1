/*
 * memgrind.c
 *
 *  Created on: Oct 4, 2016
 *      Author: RyanMini
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "mymalloc.h"


#define malloc(x) 	mymalloc(x, __FILE__, __LINE__)
#define free(x)		myfree(x, __FILE__, __LINE__)


void grindA(){


	char *ptrArr[3000];
	int i;
	for (i = 0; i < 3000; i++){
		ptrArr[i] = (char*)malloc(sizeof(char));
	}
	for (i = 0; i < 3000; i++){
		free(ptrArr[i]);
	}
}

void grindB(){

	char* ptr = (char*)malloc(sizeof(char));
	int i;
	for (i = 0; i < 3000; i++){
		free(ptr);
	}
}

void grindC(){

	int max = 3000;
	char* ptrArr[max];
	int i = 0;
	int j = 0;


	srand(time(NULL));

	while(j < max){
		int r = rand();
		int choice = r % 2;
		if (choice == 0){
			if (j >= i){
				ptrArr[i] = (char*)malloc(1);
				i++;
			}
			else {
				free(ptrArr[j]);
				j++;
			}
		}
		else {
			if (i < max){
				ptrArr[i] = (char*)malloc(1);
				i++;
			}
			else {
				continue;
			}
		}
		printf("Number allocated = %d, number freed = %d\n\n", i, j);
	}



}

void grindD(){

	int max = 3000;
	char* ptrArr[max];
	int i = 0;
	int j = 0;


	srand(time(NULL));

	while(j < max){
		int r = rand();
		int size = (rand() % 200);
		int choice = r % 2;
		if (choice == 0){
			if (j == i){
				// printf("Allocating...\n");
				ptrArr[i] = (char*)malloc(size);
				printf("Size is %d\n", size);
				i++;
			}
			else {
				// printf("Freeing...\n");
				free(ptrArr[j]);
				// printf("Freed\n");
				j++;
			}
		}
		else {
			if (i < max){
				// printf("Allocating...\n");
				ptrArr[i] = (char*)malloc(size);
				printf("Size is %d\n", size);
				i++;
			}
			else {
				continue;
			}
		}
		printf("Number allocated = %d, number freed = %d\n\n", i, j);
	}


}


int main(){

	grindA();
	grindB();
	grindC();
	grindD();

	printf("Process complete!\n");
	return 0;

}

