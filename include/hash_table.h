#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdlib.h>

#include <list.h>

static const size_t MAX_WORD_LENGHT = 32;

struct HashTable
{
    size_t size;
    List* table;

    size_t (*HashFunc)(size_t hash_count, const char* word);
};

HashTable* HashTableCreate(size_t size, size_t (*HashFunc)(size_t hash_count, const char* word));
void HashTableDestroy(HashTable* table);
void HashTableLoad(HashTable* table, size_t count, char** words);
void HashTableStat(HashTable* table, const char* filename);

size_t HashTableRun(HashTable* table, const char* word);
size_t HashTableRun_Middle(HashTable* table, const char* word);
size_t HashTableRun_Fast(HashTable* table, const char* word);

#endif // HASH_TABLE_H