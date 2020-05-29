//Author:Oishika Chaudhury

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* HEADER;
void* CURR_TAIL;
int ALLOCATED;
int MALLOC_NUM;
int FREE_NUM;


int throughput(){
    return ALLOCATED/(sbrk(0)-HEADER);
}

/*Create the initial empty heap with a dummy header node, set values for global variables */
void init(){
    HEADER = sbrk(0);
    CURR_TAIL = sbrk(16);
    *(unsigned int*)CURR_TAIL = NULL;
    *(unsigned int*)HEADER = NULL;
    MALLOC_NUM,FREE_NUM, ALLOCATED = 0;
}

void* findFit(int size){
    void* dummy = HEADER;
    while (dummy != NULL){
        if ((*(unsigned int*)dummy & ~0x7) > size ) {
            return dummy;
        }
        dummy = (unsigned int*)dummy + 3; //move up 4 bytes, i.e size bytes
    }
    return NULL;
}

//NEEDS WORK WHEN PLACING BLOCK IN AN ALREADY ALLOCATED BLOCK, CAUSES FRAGMENTATION, SHOULD WE ALLOCATE WHOLE BLOCK OR PART OF IT?
void* mallocAttempt(int size){
    void* val;
    if (size < 24) size = 24;
    else if (size % 24) size = (24*((size%24) + 1)); /*round up size to the nearest multiple of 24*/
    val = findFit(size);
    if (!(val)) val = sbrk(size);  /*We have found a free block large enough for our allocate request*/
    *(unsigned int*)val = size | 0x1; //assign header
    *((unsigned int*)val + size - 4) = size | 0x1;
    MALLOC_NUM += 1;
    ALLOCATED += size;
    return (unsigned int*)val + 4;
    
}

void freeAttempt(int* mem_location){
    
}

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}
