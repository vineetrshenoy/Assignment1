void test();
void memvoid();
char * getHeader(char * p);
char * getFooter(char * p);
int getSize(char * headerPointer);
int getAllocation(char * p);
char * createExtremities(char * p, int size, int allocated);
void myfree(void * ptr, char * b, int a);
void setValue(char * p, int size, int allocation);
char * getNext(char * ptr);
char * getPrevious(char * ptr);
void initialize();
char * findFit(int extendedSize);
void * mymalloc(size_t size, char * b, int a);
void coalesce(char * ptr);
