//
//  main.c
//  
//
//  Created by Oishika Chaudhury on 5/30/20.
//

#include "dma.h"
#include <stdio.h>


//TEST_CODE
int main() {
    init();
    printf("Printing values after initiliasing the allocator. \n");
    printf("HEADER : %d CURR_TAIL: %d\n", HEADER, CURR_TAIL);
    printf("size: %d predecessor: %d successor: %d \n", *(int*)HEADER, *((long *)((unsigned int*)CURR_TAIL + 1)),*((long *)((unsigned int*)CURR_TAIL + 1) + 1));
    printf("----------------------------------------------------------\n");
    
    printf("MALLOC ATTEMPT 1 \n");
    int* new = (int *)mallocAttempt(32);
    *new = 42;

    
    printf("Value stored at new is: %d and the size of the total block(requested + added padding) is: %d, %d\n", *new, * ((char*)new - 4) - 1, *( (char*)new + *(int *)((char*)new - 4) - 1 - 8)) ;
    
    printf("----------------------------------------------------------\n");
    
    
    printf("MALLOC ATTEMPT 2 \n");
    int* new2 = (int *)mallocAttempt(14);
    *new2 = 55;
    printf("Value stored at new is: %d and the size of the total block(requested + added padding) is: %d %d\n", *new2, * ((char*)new2 - 4) - 1,  *( (char*)new2 + *(int *)((char*)new2 - 4) - 1 - 8));
    int* new3 = (int *)mallocAttempt(63);
    
    printf("----------------------------------------------------------\n");
    freeAttempt(new);
    freeAttempt(new2);
    freeAttempt(new3);
    
    
    printf("PRINTING THROUGHPUT\n");
    throughput();

    
}
