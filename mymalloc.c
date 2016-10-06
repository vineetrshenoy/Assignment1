
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"


#define HDRSIZE 4

void test(){
	printf("Entered the void method\n");
	memvoid();
} 


/* Returns the address to the HEADER
	INPUT: The char pointer 
	OUTPUT: The address of the header stored in a pointer
*/
char * getHeader(char * p){
	p = p - HDRSIZE;
	int sizetwo = getSize(p);
	return p;
}


/* Returns the address to the FOOTER
	INPUT: The char pointer
	OUTPUT: The address of the header stored in a pointer
*/
char * getFooter(char * p){
	p = p + getSize(p) - (2 * HDRSIZE);
	return p;
}


/*Gets the size from the 4 byte header
	INPUT: char pointer to header
	OUPUT: return the size stored as an int
*/
int getSize(char * p){
	int size = (*p) & ~1;
	return size;
}


/*Gets the allocated bit from the 4 byte header
	INPUT: char pointer to header
	OUPUT: return the last bit as an int

*/
int getAllocation(char * p){
	int allocated = (*p) & 1;
	return allocated;
}


/*  Creates the header and footer given a pointer, size, and allocated flag
	INPUT: char pointer, int size, int allocated flag
	OUTPUT: None

*/

char * createExtremities(char * p, int size, int allocated){
	// CREATE HEADER: ORs the size and allocated bit. This writes size to the upper
	// 31 bits and the allocated flag to the LSB
	*p = size | allocated;


	p = p  + size - HDRSIZE; 		// moves to the end of the block
	
	//CREATE FOOTER: Same as CREATE HEADER
	*p = size | allocated;

	p = (p - size) + (2* HDRSIZE); // returns the beginning of usuable memory
	return p;
}






int main(){
	printf("HelloWorld\n");	
	char * ptr = (char * ) malloc(256*sizeof(char));
	printf("The address BEFORE manipulation is %p\n", ptr);
	ptr = createExtremities(ptr, 32, 1);
	printf("The address AFTER  manipulation is %p\n", ptr);
	char * header = getHeader(ptr);
	printf("The HEADER is at location %p \n", header);
	printf("and has value %#010x \n", *header);
	char * footer = getFooter(ptr);
	printf("The FOOTER is at location %p \n", footer);
	printf("and has value %#010x \n", *footer);

}
