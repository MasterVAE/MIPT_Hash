#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "list.h"
#include "optimizations.h"

static ListElem* ListElemCreate();


void ListInit(List* list)
{
    assert(list);

    list->count = 0;
    list->start = NULL;
#ifdef LIST_OPT
    list->array = (ListElem*)calloc(100, sizeof(ListElem));
#endif
}

void ListDestroy(List* list)
{
    if(!list) return;


#ifdef LIST_OPT
    free(list->array);
#else
    ListElem* next = list->start;
    while(next)
    {
        ListElem* curr = next;
        next = next->next;

        free(curr);
    }
#endif
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
        free(curr->word);
        free(curr);
    }
}

void ListAddElem(List* list, char* string)
{
    assert(list);
    assert(string);


#ifdef LIST_OPT
    for(size_t i = 0; i < list->count; i++)
    {
        if(!strcmp(list->array[i].buffer, string + (32 - (size_t)string % 32)))
        {
            list->array[i].count++;
            return;
        }
    }

    ListElem* elem = list->array + list->count;

    strcpy(elem->buffer, string + (32 - (size_t)string % 32));

    elem->next = NULL;
    elem->count = 1;

    list->count++;

#else
    ListElem* elem = ListElemCreate();
    if(!elem) return;

    elem->word = string;
    elem->word_aligned = string + (32 - (size_t)string % 32);
    elem->next = NULL;
    elem->count = 1;


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
        if(!strcmp(curr->word_aligned, string + (32 - (size_t)string % 32)))
        {
            free(string);
            free(elem);
            curr->count++;
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    list->count++;
    prev->next = elem;

#endif
}

static ListElem* ListElemCreate()
{
    ListElem* elem = (ListElem*)calloc(1, sizeof(ListElem));
    if(!elem) return NULL;

    elem->next = NULL;
    elem->word = NULL;

    return elem;
}