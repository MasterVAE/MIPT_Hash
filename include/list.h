#ifndef LIST_H
#define LIST_H

#include "optimizations.h"

#include <stdlib.h>

struct ListElem
{
    ListElem* next;
    char* word;
    char* word_aligned;
    size_t count;
    
#ifdef LIST_OPT
    char buffer[32];
#endif
};

struct List
{
    size_t count;
    ListElem* start;

    #ifdef LIST_OPT
    ListElem* array;
    #endif
};

void ListInit(List* list);
void ListDestroy(List* list);
void ListClear(List* list);
void ListAddElem(List* list, char* string);

#endif // LIST_H