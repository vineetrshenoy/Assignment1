#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"


#define HDRSIZE 4

void test(){
	printf("Entered the void method\n");
	memvoid();
} 


static char myBlock[100000];


/* Frees the memory to which the pointer references
	INPUT: The void pointer
	OUTPUT: None
*/

void free(void * ptr){
	//Gets the header and footer
	char * header = (char *) getHeader(ptr);
	char * footer = (char *) getFooter(ptr);
	
	//Gets the size and allocated bit
	int size = getSize(header);
	
	//sets the value for header and footer
	setValue(header, size, 0);
	setValue(footer, size, 0);


	//coalesce

}



/* Returns the address to the HEADER
	INPUT: The char pointer 
	OUTPUT: The address of the header stored in a pointer
*/
char * getHeader(char * p){
	p = p - HDRSIZE;
	return p;
}


/* Returns the address to the FOOTER
	INPUT: The char pointer
	OUTPUT: The address of the header stored in a pointer
*/
char * getFooter(char * p){
	p = p + getSize(getHeader(p)) - (2 * HDRSIZE);
	return p;
}


/*Gets the size from the 4 byte header
	INPUT: char pointer to header
	OUPUT: return the size stored as an int
*/
int getSize(char * headerPointer){
	int size = (*(int *)headerPointer) & ~1;
	return size;
}


/*Gets the allocated bit from the 4 byte header
	INPUT: char pointer to header
	OUPUT: return the last bit as an int
*/
int getAllocation(char * p){
	int allocated = (*(int *)p) & 1;
	return allocated;
}


/*  Sets the value of a four byte word based on size and allocation bit
	INPUT: char pointer, int size, int allocated flag
	OUTPUT: None
*/
void setValue(char * p, int size, int allocation){
	int * ptr = (int *) p; 	// Casts to int pointer. Good practice since we are writing ints
	*ptr = size | allocation;
}



/*  Creates the header and footer given a pointer, size, and allocated flag
	INPUT: char pointer, int size, int allocated flag
	OUTPUT: None
*/

char * createExtremities(char * p, int size, int allocated){
	// CREATE HEADER: ORs the size and allocated bit. This writes size to the upper
	// 31 bits and the allocated flag to the LSB
	*p = (int) (size | allocated);


	p = p  + size - HDRSIZE; 		// moves to the end of the block
	
	//CREATE FOOTER: Same as CREATE HEADER
	*p = (int) (size | allocated);

	p = (p - size) + (2* HDRSIZE); // returns the beginning of usuable memory
	return p;
}






int main(){
	printf("HelloWorld\n");	
	char * ptr = (char * ) malloc(256*sizeof(char));
	printf("The address BEFORE manipulation is %p\n", ptr);
	ptr = createExtremities(ptr, 16, 1);
	setValue(getHeader(ptr), 4,0);
	setValue(getFooter(ptr),4,0);
	printf("The address AFTER  manipulation is %p\n", ptr);
	char * header = getHeader(ptr);
	printf("The HEADER is at location %p \n", header);
	printf("and has value %#010x \n", *header);
	char * footer = getFooter(ptr);
	printf("The FOOTER is at location %p \n", footer);
	printf("and has value %#010x\n", *footer);

}

    

