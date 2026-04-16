#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

struct ListElem
{
    ListElem* next;
    char* word;
    size_t count;
};

struct List
{
    size_t count;
    ListElem* start;
};

List* ListCreate();
void ListInit(List* list);
void ListDestroy(List* list);
void ListClear(List* list);
void ListAddElem(List* list, char* string);

#endif // LIST_H