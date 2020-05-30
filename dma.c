/*Author:Oishika Chaudhury
WARNING: Some of the pointer casting and conversions may not make
 sense at first go, which is why reading through the README.md file
 might be a better idea, where I have tried to explain the specifications
 of what I am trying to implement. Along with it is a picture that I hope
 will make my intentions clearer. Forgive the loquaciousness.
 
 Happy reading! :D
 
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dma.h>

/*
 

 
 */


void* HEADER;
void* CURR_TAIL;
int ALLOCATED;
int MALLOC_NUM;
int FREE_NUM;

/*

 */

int throughput(){
    int val = -(HEADER - sbrk(0));
    printf("The number of requested bytes is %d, and total heap utilisation is %d\n", ALLOCATED, val);
}

/*
 The purpose of the init function is to create the initial
 empty heap with a dummy header node and set values for global
 variables.
 
 */

void init(){
    //1. Set HEADER to be the beginning of unallocated block
    HEADER = sbrk(0);
    //2. Increase heap size to make space for dummy header
    CURR_TAIL = sbrk(24);
    //3. Put the size of the HEADER in the header block
    *(int*)HEADER = 0x19;
    //4. Put the size of the HEADER in the footer of the block
    *(int *)(((char*)HEADER + 20)) = 24 | 0x1;
    //5. Set the predecessor and the sucessor block to 0s.
    * (long *) ( (unsigned int*)HEADER + 1 )  = NULL;
    * (long *) ( (unsigned int*)HEADER + 3 ) = NULL;
    
    CURR_TAIL = ( void *) ( (unsigned int*)HEADER + 3);
    
    MALLOC_NUM,FREE_NUM, ALLOCATED = 0;
    
}



void* findFit(int size){
    /*NOTE: (int*)HEADER is done to cast it to an int* such that wheb 3 is added,
     we add 12 bytes to the start of header(4 bytes for header, 8 bytes for previous block)
    Then, we cast it to a long* so that when we derefernce it, we can read the next 8 bytes,
     which is the address of the sucessor node. */
    
    void* dummy = * (long*) ( (int*)HEADER + 3);
    
    /*Here we traverse through the free list in order to find a fit in the free block
     where we can place the alloacted block. */
    
    while (dummy != 0){
         /* Casting dummy to unsigned int* in order to be able to read
          the next 4 bytes in memory, and then dereferencing it to find the
          size */
         if ((  *(unsigned int*)dummy  &  ~0x7  ) > size ) return dummy;
        
        /*Increment dummy node by 12 bytes, by first casting to an int pointer
         and then adding 3 to it. Then interpret it as a (long*) in order to read
         the address of the next node. */
        
         dummy = * (long*)( (int*)dummy + 3);
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
    if (!val) val = sbrk(size);
    

    
    //4. Assign header
    *(int*)val = size | 0x1;
    
    //5. Assign footer
    * (int *)   (  (char*)val + size - 4)   = size | 0x1;
    
    MALLOC_NUM += 1;
    
    //printf("%d %d \n", (int*)val, (int*)val + 4);
    
    //6. Return pointer to the address where the user can start manipulating the data
    return (char*) val + 4;

}



void freeAttempt(char* memoryAddress){
    //1. Check whether the predecessor block and the successor blocks are free.
    int pred = *(memoryAddress - 8) & 0x1;
    int succ = *(memoryAddress + *(int*)(memoryAddress - 4) - 5) & 0x1;
   
    
    //2. Depending on whether the predecessor and successor blocks are free or not,
    //coalesce the block + change the tail + set the current block to free


    if (pred && succ){ //If both previous and next blocks are occupied.
        //a. Set the last bit to 0 to indicate that it has been freed.
        *(int*)(memoryAddress - 4) = *(int*)(memoryAddress - 4) & ~0x1;
        *((char*)memoryAddress + *(int *)((char*)memoryAddress - 4) - 1 - 8) = *((char*)memoryAddress + *(int *)((char*)memoryAddress - 4) - 1 - 8) & ~0x1;
        
        //b. Change the current tail to point to memory address of the block being freed.
        *(long*)CURR_TAIL = memoryAddress-4;
        CURR_TAIL = memoryAddress + 12;
        *(long*)CURR_TAIL = NULL;
    }
    
    else if (!pred && succ) { //If the prev block is freed, but next block is allocated
        
        //a. Just change the header in the previous block and footer in the current block and you're good
        
        int sizePrev = *(int *)(memoryAddress - 8); //Size of the previous block
        int sizeCurr = *(int *)(memoryAddress - 4) - 1; //Size of the current block
        
        * (int*) (memoryAddress - 4 - sizePrev) = sizeCurr + sizePrev; //change header in the previous block
        * (int*) (memoryAddress - 4 + sizeCurr) = sizeCurr + sizePrev; //change footer in the current block
    }
    
    else if (pred && !succ) { //If the prev block is allocated but the next block is freed
        /*1. We need to change the header of the current block, and the footer of the next block.
        But before we do that, we need to store the values of the predecessor and succesor block
        from the next block. */
        
        long* predBlock = *(long *)(   (memoryAddress + *(int*)(memoryAddress - 4) - 5)   +   4);
        long* nextBlock = *(long *)(   (memoryAddress + *(int*)(memoryAddress - 4) - 5)   +   12);
        
        
        int sizeNext = *(int *)(memoryAddress + *(int*)(memoryAddress - 4) - 5); //Size of the next block
        int sizeCurr = *(int *)(memoryAddress - 4) - 1; //Size of the current block
        
        *(int *)(memoryAddress - 4) = sizeCurr + sizeNext; //change header of curr block
        *(long *)(memoryAddress) = predBlock; //add in values for predecessor
        *(long *)(memoryAddress + 8) = nextBlock; //add in values of successor
        
        *(int *)(memoryAddress - 4 + sizeCurr + sizeNext - 4) = sizeCurr + sizeNext; //change footer of next block
        
    }

}




    
