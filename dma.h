//
//  dma.h
//  
//
//  Created by Oishika Chaudhury on 5/30/20.
//

#ifndef dma_h
#define dma_h

/*

HEADER is the address of the prologue header, i.e,
the very first location in the heap before the program is run.
The prologue header is just 24 bytes following the data segment
in memory, where we store the size of this prologue beader, and
the address of the next free block.

TAIL is the address where the address of the most recently freed block
is stored.

ALLOCATED stores the total number of bytes of requested blocks.
MALLOC_NUM and FREE_NUM are the number of malloc and free requests
made by the program.

*/



extern void* HEADER;
extern void* CURR_TAIL;
extern int ALLOCATED;
extern int MALLOC_NUM;
extern int FREE_NUM;

/*

The purpose of this function is to print out the total
bytes of requested memory, and the current heap size, which
may not always be equal due to a number of reasons such as,
padding, minimum size restrictions(24 bytes in this case),
memory utilisation in terms of storing headers, footers and
memory alignment restrictions.

*/

int throughput();

/*
The purpose of the init function is to create the initial
empty heap with a dummy header node and set values for global
variables.

*/


void init();

/*This function looks for a free block which is large enough
to meet the requested size of our the block. Return NULL
if unable to find suitable block.
*/

void* findFit(int size);

/* mallocAttempt tries to immitate the malloc function. */

void* mallocAttempt(int size);

/* freeAttempt tries to immitate the free function. */

void freeAttempt(char* memoryAddress);

#endif /* dma_h */
