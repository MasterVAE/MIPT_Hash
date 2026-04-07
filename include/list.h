#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

struct ListElem
{
    ListElem* next;
    const char* word;
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
void ListAddElem(List* list, const char* string);

#endif // LIST_H