//Author:Oishika Chaudhury

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* HEADER;
void* CURR_TAIL;
int ALLOCATED;
int MALLOC_NUM;
int FREE_NUM;


//int throughput(){
//    return ALLOCATED/(sbrk(0)-HEADER);

/*Create the initial empty heap with a dummy header node, set values for global variables */
void init(){
    HEADER = sbrk(0); //beginning of unallocated block
    CURR_TAIL = sbrk(24); //increase heap size to make space for dummy header and tail
    *(int*)HEADER = 0x19;
    *((long *)((unsigned int*)CURR_TAIL + 1)) = NULL;
    *((long *)((unsigned int*)CURR_TAIL + 1) + 1) = NULL;
    MALLOC_NUM,FREE_NUM, ALLOCATED = 0;
}

/*This function looks for a block which is large enough to hold our allocated block*/
void* findFit(int size){
    void* dummy = (int *)HEADER+3;
    while (*(int *)dummy != 0){
         if ((*(unsigned int*)dummy & ~0x7) > size ) return dummy;
         dummy = (unsigned int*)dummy + 3; //move up 12 bytes, i.e size bytes
    }
    return NULL;
}

void* mallocAttempt(int size){
    void* val;
    if (size < 24) size = 24;
    else if (size % 24) size = (24*((size/24) + 1)); /*round up size to the nearest multiple of 24*/
    val = findFit(size);
    if (!(val)) val = sbrk(size);  /*We have found a free block large enough for our allocate request*/
    *(unsigned int*)val = size | 0x1; //assign header
    *((unsigned int*)val + size - 4) = size | 0x1;
    MALLOC_NUM += 1;
    ALLOCATED += size;
    return (unsigned int*)val + 4;
}

int main() {
    init();
    printf("Printing values after initiliasing the allocator.\n");
    printf("HEADER : %d CURR_TAIL: %d\n", HEADER, CURR_TAIL);
    printf("size: %d predecessor: %d successor: %d \n", *(int*)HEADER, *((long *)((unsigned int*)CURR_TAIL + 1)),*((long *)((unsigned int*)CURR_TAIL + 1) + 1));
    printf("----------------------------------------------------------\n");
    printf("Malloc attempt 1: \n");
    int* new = (int *)mallocAttempt(32);
    *new = 42;
    printf("Value stored at new is: %d and the size of the requested block is: %d \n", *new, *(new-4));
    
}
