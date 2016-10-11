/*
 * mymalloc.h
 *
 *  Created on: Oct 2, 2016
 *      Author: RyanMini
 */

#ifndef MYMALLOC_H_
#define MYMALLOC_H_

unsigned int get(void* hp);
void put(void* hp, unsigned int val);
unsigned int getSize(void* hp);
unsigned int getAlloc(void* hp);
void* getHeader(void* bp);
void* getFooter(void* bp);
void* nextBlock(void* bp);
void* prevBlock(void* bp);
void init();
static void* coalesce(void* bp);
void myfree(void* bp, int a, int b);
void* findFit(size_t size);
static void place(void* bp, size_t size);
void* mymalloc(size_t size, int a, int b);
int main();




#endif /* MYMALLOC_H_ */
