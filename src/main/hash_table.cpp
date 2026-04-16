#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <immintrin.h>

#include "hash_table.h"

int strncmp_fast(const char* s1, const char* s2);


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

void HashTableLoad(HashTable* table, size_t count, char** words)
{
    assert(table);
    assert(words);

    for(size_t i = 0; i < count; i++)
    {
        size_t hash_value = table->HashFunc(table->size, words[i]);
        char* buffer = (char*)calloc(MAX_WORD_LENGHT + 1, sizeof(char));
        strcpy(buffer, words[i]);
        ListAddElem(&table->table[hash_value], buffer);
    }
}

size_t HashTableRun(HashTable* table, const char* word)
{
    assert(table);
    assert(word);

    size_t hash_value = table->HashFunc(table->size, word);
    List* list = &table->table[hash_value];

    ListElem* elem = list->start;
    while(elem)
    {
        //printf("word: %p\n", word);
        //printf("word in table %p\n", elem->word);
        if(!strncmp_fast(elem->word, word)) return elem->count;
        //if(!strncmp(elem->word, word, MAX_WORD_LENGHT)) return elem->count;
        elem = elem->next;
    }

    return 0;
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

inline int strncmp_fast(const char* s1, const char* s2)
{
    assert(s1);
    assert(s2);

    __m256i v1 = _mm256_loadu_si256((const __m256i*)s1);
    __m256i v2 = _mm256_loadu_si256((const __m256i*)s2);

    __m256i cmp = _mm256_cmpeq_epi8(v1, v2);
    int mask = _mm256_movemask_epi8(cmp);

    if (mask == 0xFFFFFFFF) return 0;

    return 1;
}