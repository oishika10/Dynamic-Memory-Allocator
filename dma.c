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
    int val = -(HEADER - sbrk(0));
    printf("The number of requested bytes is %d, and total heap utilisation is %d\n", ALLOCATED, val);
}

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
    void* dummy = *(long*)((int*)HEADER+3); //NOTE: (int*)HEADER+3 is not the address of the successor block, but the adress where this address is stored
    while (dummy != 0){
         if ((*(unsigned int*)dummy & ~0x7) > size ) return dummy;
         dummy = *(long*)((int*)dummy+3); //dummy node is now equal to the successor node
    }
    return NULL;
}

void* mallocAttempt(int size){
    ALLOCATED += size;
    void* val;
    //1. Adjust size to make sure it is rounded up to the nearest multiple of 24
    if (size < 24) size = 24;
    else if (size % 24) size = (24*((size/24) + 1));
    //2. Look for size bytes in the free blocks
    val = findFit(size);
    //3. Otherwise request for more memory from heap
    if (!(val)) val = sbrk(size);
    *(unsigned int*)val = size | 0x1; //assign header
    *(int *)(((char*)val + size - 4)) = size | 0x1;
    MALLOC_NUM += 1;
    return (unsigned int*)val + 4;
}

int main() {
    init();
    printf("Printing values after initiliasing the allocator. \n");
    printf("HEADER : %d CURR_TAIL: %d\n", HEADER, CURR_TAIL);
    printf("size: %d predecessor: %d successor: %d \n", *(int*)HEADER, *((long *)((unsigned int*)CURR_TAIL + 1)),*((long *)((unsigned int*)CURR_TAIL + 1) + 1));
    printf("----------------------------------------------------------\n");
    printf("MALLOC ATTEMPT 1 \n");
    int* new = (int *)mallocAttempt(32);
    *new = 42;
    printf("Value stored at new is: %d and the size of the requested block is: %d, %d\n", *new, *(new-4)-1, *(int *)(((char*)(new-4) + (*(new-4)-1) - 4)));
    printf("MALLOC ATTEMPT 2 \n");
    int* new2 = (int *)mallocAttempt(14);
    *new2 = 55;
    printf("Value stored at new is: %d and the size of the requested block is: %d %d\n", *new2, *(new2-4)-1, *(int *)(((char*)(new2-4) + (*(new2-4)-1) - 4)));
    printf("PRINTING THROUGHPUT\n");
    throughput();
}
