#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymalloc.h"


#define HDRSIZE 4
#define MEMSIZE 5000

#define malloc(x) mymymalloc(x,__LINE__,__FILE__)
#define free(x) myfree(x,__LINE__,__FILE__)

void test(){
	printf("Entered the void method\n");
	memvoid();
} 


static char myBlock[5000];


/* Creates a space in memory based on size, if available. Returns NULL if not
	INPUT: size_t of the block to created
	OUTPUT: The char pointer of the available space in memory; NULL if no place
	This also initializes the header and footer.
*/

void * mymalloc(size_t size, char * b, int a){
	size_t extendedSize;
	char * ptr;
	char *headerPointer;
	char * footerPointer;
	int oldSize, difference, adjustedSize;

	//Spurrious case. size = 0
	if (size == 0){
		// printf("WARNING. Zero size. Returning NULL Pointer at %s and line %d\n", b, a );
		return NULL;
	}
	else if(size > MEMSIZE){
		//printf("Warning: size exceeds memory size. Return null at %s and line %d\n", b,a);
		return NULL;
	}

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
	//printf("Unable to allocated space. Returning NULL in %s at line %d \n", __FILE__, __LINE__);
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
	while ( blockSize != 0 )	{	//Conditions for epilogue block
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

void initialize(){
	char * memBlock;

	memBlock = myBlock;
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

void myfree(void * ptr, char * b, int a){
	char *  next;
	char * previous;
	int size;
	
	if (ptr == NULL){
		//printf("Unable to free a NULL pointer in %s line  %d \n",b,a);
		return;
	}

	int relativeAddress = (char*)(ptr) - (char*)myBlock;

	if (relativeAddress > sizeof(myBlock) - 2*HDRSIZE || relativeAddress < HDRSIZE){
		// printf("Not a freeable memory address in %s line  %d \n",__FILE__,__LINE__);
		return;
	}

	if (getAllocation(ptr) == 0){
		//printf("Can not free an already free block in %s line %d\n", __FILE__, __LINE__);
	}

	//Gets the size and allocated bit
	size = getSize(ptr);
	
	//sets the value for header and footer
	setValue(getHeader(ptr), size, 0);
	setValue(getFooter(ptr), size, 0);


	coalesce(ptr);

}

/* Coalesces memory blcoks based on the current, previous, and next blocks
	INPUT: The char pointer pointing to the block we want to free
	OUTPUT: None
*/
void coalesce(char * ptr){
	char * previous;
	char * next;
	int allocPrevious, allocNext, size;

	previous  = getPrevious(ptr);
	next = getNext(ptr);
	size = getSize(ptr);

	//This is to check if the next or previos blocks are null
	if (previous != NULL)
		allocPrevious = getAllocation(previous);
	else
		allocPrevious = 1;
	
	if (next != NULL)
		allocNext = getAllocation(next);
	else
		allocNext = 1;


	//Case 1: Neither the previous or next blocks are free
	if ((allocPrevious == 1) && (allocNext == 1)){
		return;
	}
	//Case 2: Previous block is free, but next block is allocated
	else if ((allocPrevious == 0) && (allocNext == 1)){
		size = size + getSize(previous); 
		setValue(getFooter(previous), 0 ,0);
		setValue(getHeader(previous), size, 0);
		setValue(getFooter(ptr), size, 0);
		setValue(getHeader(ptr), 0,0);

	}

	//Case 3: Next block is free, but previous block is allocated
	else if ((allocPrevious == 1) && (allocNext == 0)){
		size = size + getSize(next); 
		setValue(getFooter(ptr), 0 ,0);
		setValue(getHeader(ptr), size, 0);
		setValue(getFooter(next), size, 0);
		setValue(getHeader(next), 0,0);

	}

	//Case 4: Both previous and next blocks are free
	else if ((allocPrevious == 0) && (allocNext == 0)){
		size = size + getSize(previous) + getSize(next); 
		setValue(getFooter(previous),0 ,0);
		setValue(getHeader(previous), size, 0);
		setValue(getFooter(ptr), 0,0);
		setValue(getHeader(ptr), 0, 0);
		setValue(getFooter(next), size, 0);
		setValue(getHeader(next), 0 , 0);

	}


	
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




/*

int main(){
	printf("HelloWorld\n");	
	initialize(myBlock);
	printf("The address of myBlock is %p \n", myBlock);
	int *block = (int *) (myBlock + 4);
	printf("The header has address %p and value %#010x\n", block, *block);
	int * footer = (int *) (myBlock + *block);
	printf("The footer has address %p and value %#010x\n", footer, *footer);
	char * test1 = mymymalloc(8);
	printf("Process complete\n");
	char * test2 = mymymalloc(8);
	char * test3 = mymymalloc(8);
	//char * test3 = mymymalloc(8);
	myfree(test1);
	myfree(test2);
	printf("Process Complete\n");
	return 0;
	/*
	char * ptr = (char * ) mymalloc(256*sizeof(char));
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

 
 */   

