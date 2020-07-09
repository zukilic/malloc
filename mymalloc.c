/*
    Zeynep Ülkü Kılıç
    18120205011
*/

#include "mymalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//allocates the memory space as given size
void *mymalloc(size_t size){
    static int firstcall = 1;/*executes only once*/
    if(firstcall){
        heap_start = sbrk(sizeof(Block)*1024);
        heap_start->info.size = 1024;
        heap_start->info.isfree = 0;
        end = (void*)heap_start + sizeof(Block) + heap_start->info.size;
        end->info.size = 1024;
        end->info.isfree = 0;
        heap_start->next = end;
        heap_start->prev = NULL;
        end->next = NULL;
        end->prev = heap_start;
        free_list = heap_start;
        free_list->info.size = 1024;    
        free_list->info.isfree = 0;
        firstcall = 0;  /*no more execution for this block*/
        /*available free space is 1024*/
    }

    //making the size to multiple of 32. 
    if(size%32 != 0){
        int x = size%32;
        x = 32 - x;
        size = size + x;
    }

    Block *p;

    //memory allocation by strategy selection
    if(strategy == bestFit){
        p = free_list;
        unsigned int min_size = p->info.size;
       /*finding the smallest size in the list and choose it for p*/
        while( p < end && (p->info.isfree || p->info.size < size )){
            if((min_size > p->info.size) && (size >= p->info.size)){
                min_size = p->info.size;
            }
            else{
                p = p->next;
            }
        }
    }
    else if(strategy == worstFit){
        p = free_list;
        unsigned int max_size = p->info.size;
        /*finding the largest size in the list and choose it for p*/
        while( p < end && (p->info.isfree || p->info.size < size )){
            if((max_size < p->info.size) && (size >= p->info.size)){
                max_size = p->info.size;
            }
            else{
                p = p->next;
            }
        }
    }
    else if(strategy == firstFit){
        p = free_list;
        /*finding first suitable block for p*/
        while( p < end && (p->info.isfree || p->info.size < size )){
            p = p->next;
        }
    }
    
    /*p is the found block*/ 
    if (p == end){
        /*no available space*/
        fprintf(stderr,"no available space");
        return NULL;
    }

    if(size < p->info.size){
        split(p, size); /*to large space: split into 2*/
    }
    else{
        p->info.isfree = 1;
        p->info.size = size;
    }
    
    free_list = p->next;
    return sizeof(Block) + (void*)p; /*start of the block address*/
}


//freeing the memory allocation for p
void *myfree(void *p){
    Block *b = (Block*)(p-sizeof(Block)); /*block header*/
    b->info.isfree = 0;

    Block *prev = b->prev; /*previous block*/
    if(!prev->info.isfree){
        prev->info.size += b->info.size+sizeof(Block);
        b = prev;
    }

    Block *next = (void*)b + sizeof(Block) + b->info.size; /*next block*/
    if((!next->info.isfree) && (next < end)){
        b->info.size += next->info.size+sizeof(Block);
        next = b;
        b = next;
    }
    if(b->next < end){
        b->next = free_list;
        free_list = b;
    }
    return (void *)b;
}

//splitting the block into two parts
Block *split(Block *b, size_t size){
    int oldsize = b->info.size;
    b->info.size = size;
    b->info.isfree = 1;
    Block *q = (void*)b + sizeof(Block) + b->info.size;
    q->info.size = oldsize-size-sizeof(Block);
    q->info.isfree = 0;
    q->prev = b;
    q->next = b->next;
    b->next = q; 
    return q;
}


//testing our strategies
int test(int fitStrategy){
    if(fitStrategy == bestFit){
        printf("BEST FIT STRATEGY\n");
        printf("\n");
        int *ptr1, *ptr2, *ptr3, *ptr4, *ptr5, *ptr6;
        ptr1 = mymalloc(24);
        if (ptr1 == NULL) { 
            printf("Memory not allocated.\n"); 
            exit(0); 
        }
        ptr2 = mymalloc(47); 
        if (ptr2 == NULL) { 
            printf("Memory not allocated.\n"); 
            exit(0); 
        }
        ptr3 = mymalloc(32);
        if (ptr3 == NULL) { 
            printf("Memory not allocated.\n"); 
            exit(0); 
        }
        ptr4 = mymalloc(21);
        if (ptr4 == NULL) { 
            printf("Memory not allocated.\n"); 
            exit(0); 
        }
        ptr5 = mymalloc(35);
        if (ptr5 == NULL) { 
            printf("Memory not allocated.\n"); 
            exit(0); 
        }

        printf("addresses of pointers:\n");
        printf("ptr1:%d-ptr2:%d-ptr3:%d-ptr4:%d-ptr5:%d\n",
                ptr1, ptr2, ptr3, ptr4, ptr5);
        printf("\n");

        myfree(ptr2);   //freeing ptr
        myfree(ptr4);   //freeing ptr2

        ptr6 = mymalloc(17);
        if (ptr6 == NULL) { 
            printf("Memory not allocated.\n"); 
            exit(0); 
        }

        
        printf("after freeing ptr2 and ptr4\n"
               "the new pointers (ptr6) adress\n"
               "is using ptr4's old one\n");
        printf("ptr1:%d-ptr3:%d-ptr5:%d-ptr6:%d\n", 
                ptr1, ptr3, ptr5, ptr6);

        printf("\n");
        printf("END OF TEST\n");
        return fitStrategy;
    }
    else if(fitStrategy == worstFit){
        printf("WORST FIT STRATEGY\n");
        printf("\n");
        int *ptr1, *ptr2, *ptr3, *ptr4, *ptr5, *ptr6;
        ptr1 = mymalloc(24);
        if (ptr1 == NULL) { 
            printf("Memory not allocated.\n"); 
            exit(0); 
        }
        ptr2 = mymalloc(100); 
        if (ptr2 == NULL) { 
            printf("Memory not allocated.\n"); 
            exit(0); 
        }
        ptr3 = mymalloc(32);
        if (ptr3 == NULL) { 
            printf("Memory not allocated.\n"); 
            exit(0); 
        }
        ptr4 = mymalloc(21);
        if (ptr4 == NULL) { 
            printf("Memory not allocated.\n"); 
            exit(0); 
        }
        ptr5 = mymalloc(700);
        if (ptr5 == NULL) { 
            printf("Memory not allocated.\n"); 
            exit(0); 
        }

        printf("addresses of pointers:\n");
        printf("ptr1:%d-ptr2:%d-ptr3:%d-ptr4:%d-ptr5:%d\n",
                ptr1, ptr2, ptr3, ptr4, ptr5);
        printf("\n");

        myfree(ptr2);   //freeing ptr4
        myfree(ptr4);   //freeing ptr2

        ptr6 = mymalloc(17);
        if (ptr6 == NULL) { 
            printf("Memory not allocated.\n"); 
            exit(0); 
        }

        
        printf("after freeing ptr2 and ptr4\n"
               "the new pointers (ptr6) adress\n"
               "is using ptr2's old one\n");
        printf("ptr1:%d-ptr3:%d-ptr5:%d-ptr6:%d\n", 
                ptr1, ptr3, ptr5, ptr6);

        printf("\n");
        printf("END OF TEST\n");
        return fitStrategy;
    }
    else if(fitStrategy == firstFit){
        printf("FIRST FIT STRATEGY\n");
        printf("\n");
        int *ptr1, *ptr2, *ptr3, *ptr4, *ptr5;
        ptr1 = mymalloc(24);
        if (ptr1 == NULL) { 
            printf("Memory not allocated.\n"); 
            exit(0); 
        }
        ptr2 = mymalloc(47); 
        if (ptr2 == NULL) { 
            printf("Memory not allocated.\n"); 
            exit(0); 
        }
        ptr3 = mymalloc(32);
        if (ptr3 == NULL) { 
            printf("Memory not allocated.\n"); 
            exit(0); 
        }

        printf("addresses of pointers:\n");
        printf("ptr1:%d-ptr2:%d-ptr3:%d\n", ptr1,ptr2,ptr3);
        printf("\n");

        myfree(ptr3);   //freeing ptr3
        myfree(ptr2);   //freeing ptr2

        ptr4 = mymalloc(21);
        if (ptr4 == NULL) { 
            printf("Memory not allocated.\n"); 
            exit(0); 
        }
        ptr5 = mymalloc(35);
        if (ptr5 == NULL) { 
            printf("Memory not allocated.\n"); 
            exit(0); 
        }
        
        printf("after freeing ptr3 and ptr2\n"
               "the new pointers (ptr4 and ptr5)\n"
               "addresses are using ptr2 and ptr3s old ones\n");
        printf("ptr1:%d-ptr4:%d-ptr5:%d\n", 
                ptr1, ptr4, ptr5);

        printf("\n");
        printf("END OF TEST\n");
        return fitStrategy;
    }
    else{
        return -1;
    }
}

int main(){ 
    printf("strategy's number we use for the test:%d\n", test(strategy));
    return 0; 
}