#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "hash_table.h"

HashTable* HashTableCreate(size_t size, size_t (*HashFunc)(size_t hash_count, const char* word))
{
    assert(HashFunc);

    HashTable* table = (HashTable*)calloc(1, sizeof(HashTable));
    if(!table) return NULL;

    table->size = size;

    table->table = (List*)calloc(size, sizeof(List));
    if(!table->table)
    {
        free(table);
        return NULL;
    }

    for(size_t i = 0; i < size; i++)
    {
        ListInit(&table->table[i]);
    }

    table->HashFunc = HashFunc;

    return table;
}


void HashTableDestroy(HashTable* table)
{
    if(!table) return;

    for(size_t i = 0; i < table->size; i++)
    {
        ListClear(&table->table[i]);
    }
    free(table->table);
    free(table);
}

void HashTableRun(HashTable* table, size_t count, char** words)
{
    assert(table);
    assert(words);

    for(size_t i = 0; i < count; i++)
    {
        size_t hash_value = table->HashFunc(table->size, words[i]);
        ListAddElem(&table->table[hash_value], words[i]);
    }
}

void HashTableStat(HashTable* table, const char* filename)
{
    assert(table);
    assert(filename);

    FILE* file = fopen(filename, "w+");
    if(!file) return;

    for(size_t i = 0; i < table->size; i++)
    {
        fprintf(file, "%lu;", table->table[i].count);
    }

    fclose(file);
}