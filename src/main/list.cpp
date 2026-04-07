#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "list.h"

static ListElem* ListElemCreate();

List* ListCreate()
{
    List* list = (List*)calloc(1, sizeof(List));
    if(!list) return NULL;

    list->count = 0;
    list->start = NULL;

    return list;
}

void ListInit(List* list)
{
    assert(list);

    list->count = 0;
    list->start = NULL;
}

void ListDestroy(List* list)
{
    if(!list) return;

    ListElem* next = list->start;
    while(next)
    {
        ListElem* curr = next;
        next = next->next;

        free(curr);
    }

    free(list);
}

void ListClear(List* list)
{
    if(!list) return;

    ListElem* next = list->start;
    while(next)
    {
        ListElem* curr = next;
        next = next->next;

        free(curr);
    }
}

void ListAddElem(List* list, const char* string)
{
    assert(list);
    assert(string);

    ListElem* elem = ListElemCreate();
    if(!elem) return;

    elem->word = string;
    elem->next = NULL;


    if(!list->start)
    {
        list->start = elem;
        list->count++;
        return;
    }
    ListElem* curr = list->start;
    ListElem* prev = NULL;
    while(curr)
    {
        if(!strcmp(curr->word, string))
        {
            free(elem);
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    list->count++;
    prev->next = elem;
}

static ListElem* ListElemCreate()
{
    ListElem* elem = (ListElem*)calloc(1, sizeof(ListElem));
    if(!elem) return NULL;

    elem->next = NULL;
    elem->word = NULL;

    return elem;
}