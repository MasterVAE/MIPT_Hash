#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <immintrin.h>

#include "hash_table.h"
#include "hash_funcs.h"


#include "optimizations.h"


extern "C" size_t Hash_ASM(size_t hash_count, const char* word);

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
        //size_t hash_value = table->HashFunc(table->size, words[i]);

        #ifdef CRC32_OPT
        size_t hash_value = Hash_ASM(table->size, words[i]);
        #else
        size_t hash_value = HashCRC_32(table->size, words[i]);
        #endif
        
        char* buffer = (char*)calloc(MAX_WORD_LENGHT * 2 + 1, sizeof(char));
        strcpy(buffer + (32 - (size_t)buffer % 32), words[i]);
        ListAddElem(&table->table[hash_value], buffer);
    }
}

size_t HashTableRun(HashTable* table, const char* word)
{
    assert(table);
    assert(word);

    //size_t hash_value = table->HashFunc(table->size, word);

#ifdef CRC32_OPT
    size_t hash_value = Hash_ASM(table->size, word);
#else
    size_t hash_value = HashCRC_32(table->size, word);
#endif
    List* list = &table->table[hash_value];


#ifdef LIST_OPT
    ListElem* elem = list->array;
    for(size_t i = 0; i < list->count; i++)
    {
        if(!strncmp(elem->buffer, word, MAX_WORD_LENGHT)) return elem->count;
        elem++;
    }
#else
    ListElem* elem = list->start;
    while(elem)
    {
        if(!strncmp(elem->word_aligned, word, MAX_WORD_LENGHT)) return elem->count;
        elem = elem->next;
    }
#endif

    return 0;
}

size_t HashTableRun_Fast(HashTable* table, const char* word)
{
    assert(table);
    assert(word);

    #ifdef CRC32_OPT
    size_t hash_value = Hash_ASM(table->size, word);
    #else
    size_t hash_value = HashCRC_32(table->size, word);
    #endif
    List* list = &table->table[hash_value];

    __m256i* arr1 = (__m256i*)word;


#ifdef LIST_OPT
    ListElem* elem = list->array;
    for(size_t i = 0; i < list->count; i++)
    {
        __m256i* arr2 = (__m256i*)elem->buffer;

        __m256i cmp = _mm256_cmpeq_epi8(*arr1, *arr2);
        int mask = _mm256_movemask_epi8(cmp);

        if (mask == 0xFFFFFFFF) return elem->count;

        elem++;
    }
#else
    ListElem* elem = list->start;
    while(elem)
    { 
        ListElem* next = elem->next;     

        {
            __m256i* arr2 = (__m256i*)elem->word_aligned;

            __m256i cmp = _mm256_cmpeq_epi8(*arr1, *arr2);
            int mask = _mm256_movemask_epi8(cmp);

            if (mask == 0xFFFFFFFF) return elem->count;
        }

        if(!next) break;
        {
            __m256i* arr2 = (__m256i*)next->word_aligned;

            __m256i cmp = _mm256_cmpeq_epi8(*arr1, *arr2);
            int mask = _mm256_movemask_epi8(cmp);

            if (mask == 0xFFFFFFFF) return next->count;
        }

        elem = next->next;
    }
#endif

    return 0;
}

size_t HashTableRun_Middle(HashTable* table, const char* word)
{
    assert(table);
    assert(word);

    #ifdef CRC32_OPT
    size_t hash_value = Hash_ASM(table->size, word);
    #else
    size_t hash_value = HashCRC_32(table->size, word);
    #endif
    List* list = &table->table[hash_value];

    u_int64_t* arr1 = (u_int64_t*)word;

#ifdef LIST_OPT

    ListElem* elem = list->array;
    for(size_t i = 0; i < list->count; i++)
    {
        u_int64_t* arr2 = (u_int64_t*)elem->buffer;

        bool equal = true;
        for(size_t i = 0; i < 4; i++)
        {
            if(arr1[i] != arr2[i])
            {
                equal = false;
                break;
            }
        }

        if(equal) return elem->count;

        elem++;
    }
#else
    ListElem* elem = list->start;
    while(elem)
    {
        u_int64_t* arr2 = (u_int64_t*)elem->word_aligned;

        bool equal = true;
        for(size_t i = 0; i < 4; i++)
        {
            if(arr1[i] != arr2[i])
            {
                equal = false;
                break;
            }
        }

        if(equal) return elem->count;

        elem = elem->next;
    }

#endif
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