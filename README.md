# Dynamic-Memory-Allocator
Personal Dynamic Memory Allocator -

In this project I have tried to create a dynamic memory allocator that supports the basic malloc, free, calloc functions. Additionally I have added a function that can help measure the throughput. 

A few attributes I have attempted to inculcate within this allocator are:

1. Explicit free listing (a doubly linked list with a header node and tail node) 
2. A header and footer for each block
3. This imposes an additional limitation of each block size being at least 24bytes(4 bytes each for header and footer, and 8 bytes each to store the address of the predecessor and successor nodes)
4. Immediate coalescing

This is a work in progress. Enter at your own risk. 
