/*
    Zeynep Ülkü Kılıç
    18120205011
*/

/*External fragmentationun worst-fit stratejisinde
daha fazla olduğunu söylemek mümkün. Çünkü onda 
sürekli olarak en büyük size a sahip alanı bulup o
alanı split edip kullanıyoruz.*/

#include <stdio.h>

typedef enum {bestFit, worstFit, firstFit} Strategy;

Strategy strategy = bestFit;


typedef struct{
    unsigned int size;
    unsigned int isfree;
}Info;

typedef struct{
   Info info;           /*number of  16 byte blocks*/
   struct block *next;  /*next free*/
   struct block *prev;
   char data[0];        /*start of the allocated memory*/
 }Block;
/*Block: |4byte(size)|4byte(isfree)|8-byte next|0-byte(data)|*/

static Block *free_list = NULL;     /*start of the free list*/
static Block *heap_start = NULL;    /*head of allocated memory from sbrk */
static Block* end = NULL;           /*end of the allocated memory from sbrk*/

void *mymalloc(size_t size);
void *myfree(void *p);
Block *split(Block *b, size_t size);
int test(int fitStrategy);
