void test();
void memvoid();
char * getHeader(char * p);
char * getFooter(char * p);
int getSize(char * headerPointer);
int getAllocation(char * p);
char * createExtremities(char * p, int size, int allocated);
void free(void * ptr);
void setValue(char * p, int size, int allocation);