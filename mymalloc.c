#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"


#define HDRSIZE 4
#define MEMSIZE 32

void test(){
	printf("Entered the void method\n");
	memvoid();
} 


static char myBlock[32];


/* Creates a space in memory based on size, if available. Returns NULL if not
	INPUT: size_t of the block to created
	OUTPUT: The char pointer of the available space in memory; NULL if no place
	This also initializes the header and footer.
*/

char * mymalloc(size_t size){
	size_t extendedSize;
	char * ptr;
	char *headerPointer;
	char * footerPointer;
	int oldSize, difference, adjustedSize;

	//Spurrious case. size = 0
	if (size == 0)
		return NULL;

	//Adjustment for overhead and alignment
	adjustedSize = (HDRSIZE - (size % HDRSIZE)) % HDRSIZE;
	extendedSize = size + adjustedSize + (2 * HDRSIZE);

	//Get pointer to block (beginnig of usuable memory) and set value if usuable
	if ((ptr = findFit(extendedSize)) != NULL){
		 oldSize = getSize(ptr);		//Get the old size of the block
		 difference =  oldSize - extendedSize;
		//Case 1
		if ((oldSize > extendedSize) && (difference > (2 * HDRSIZE))){	
			setValue(getFooter(ptr), difference,0);
			setValue(getHeader(ptr), extendedSize,1);
			footerPointer = ptr;
			footerPointer = ptr + extendedSize - (2 * HDRSIZE);
			//footerPointer = (char *) (ptr + extendedSize - (2 * HDRSIZE));
			setValue(footerPointer,extendedSize,1);
			setValue(footerPointer + 4, difference, 0);

		}
		else{
			setValue(getHeader(ptr), oldSize, 1);
			setValue(getFooter(ptr), oldSize, 1);
		}


		return ptr;
	}
	// Pointer way null -- no place available
	return NULL;

}





/* Uses the first fit algorithm to find the next block of adequate size
	INPUT: The size of the block to be allocated
	OUTPUT: The char pointer of the available space in memory; NULL if no place
*/

char * findFit(int extendedSize){
	char * ptr = myBlock;	//beginning of memory
	ptr = ptr + (2 * HDRSIZE); 	//Move past prologue block and header
	
	//blockSize and allocated bit of the first block in memory
	int blockSize  = getSize(ptr);
	int allocBit = getAllocation(ptr);
	while ((blockSize != 0) && (allocBit != 1))	{	//Conditions for epilogue block
		//if it is unallocated and the current size can accommodate the new block
		if ((allocBit != 1) && (blockSize >= extendedSize))
			return ptr;
		
		//go to next block and get blockSize and allocBit
		ptr = ptr + blockSize;
		blockSize = getSize(ptr);
		allocBit = getAllocation(ptr);
	}
	//at epilogue block without finding space
	return NULL;

}




/* Initializes the memory block with prologue, epilogue, header and footer
	INPUT: The char pointer to the beginning of the memory block
	OUTPUT: None
*/

void initialize(char * memBlock){
	setValue(memBlock,0,1); 	//Setting the prologue block
	char * epilogue = memBlock + MEMSIZE - HDRSIZE;	//Get address of epilogue
	setValue(epilogue, 0, 1);	//Setting the epilogue block

	int memorySize = MEMSIZE - (2 * HDRSIZE);	//Remaining memory size after prologue/epilogue
	//Setting value for one contiguous memory block
	setValue((memBlock + HDRSIZE), memorySize, 0);	//Set Header
	setValue((epilogue - HDRSIZE), memorySize, 0);	//Set footer
	
}



/* Frees the memory to which the pointer references
	INPUT: The void pointer
	OUTPUT: None
*/

void free(void * ptr){
	//Gets the header and footer
	char * header = (char *) getHeader(ptr);
	char * footer = (char *) getFooter(ptr);
	
	//Gets the size and allocated bit
	int size = getSize(ptr);
	
	//sets the value for header and footer
	setValue(header, size, 0);
	setValue(footer, size, 0);


	//coalesce

}

/* Returns the pointer to the previous block. NOTE: this pointer points to the beginning
	of usable memory. You must call getHeader() to get the header
	INPUT: The char pointer pointing to the beginning of usuable memory
	OUTPUT: The address of the previous block stored in a pointer
*/
char * getPrevious(char * ptr){
	//RYAN CHECK THIS BECAUSE IT DIFFERS FROM THE TEXTBOOK
	char * footer = ptr - 8;
	int size = (*(int*) footer) & ~1;
	// This is the first block. The previous block is the prologue block
	if ((size == 0) )
		return NULL;
	char * previous = ptr - size;
	return previous;
}


/* Returns the pointer to the next block. NOTE: this pointer points to the beginning
	of usable memory. You must call getHeader() to get the header
	INPUT: The char pointer pointing to the beginning of usuable memory
	OUTPUT: The address of the next block stored in a pointer
*/
char * getNext(char * ptr){
	//RYAN CHECK THIS BECAUSE IT DIFFERS FROM THE TEXTBOOK
	int size = getSize(ptr);
	char * next = ptr + size;
	//The current block is the last block. We have reached the epilogue block
	if ( (getSize(next) == 0) && (getAllocation(next) == 1) )
		return NULL;
	return next;
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
	p = p + getSize(p) - (2 * HDRSIZE);
	return p;
}


/*Gets the size from the 4 byte header
	INPUT: char pointer to header
	OUPUT: return the size stored as an int
*/
int getSize(char * ptr){
	int size = (*(int *)getHeader(ptr)) & ~1;
	return size;
}


/*Gets the allocated bit from the 4 byte header
	INPUT: char pointer to header
	OUPUT: return the last bit as an int
*/
int getAllocation(char * ptr){
	int allocated = (*(int *)getHeader(ptr)) & 1;
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
	initialize(myBlock);
	printf("The address of myBlock is %p \n", myBlock);
	int *block = (int *) (myBlock + 4);
	printf("The header has address %p and value %#010x\n", block, *block);
	int * footer = (int *) (myBlock + *block);
	printf("The footer has address %p and value %#010x\n", footer, *footer);
	char * test = mymalloc(14);
	printf("Process complete\n");

	/*
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
	*/

}

    

