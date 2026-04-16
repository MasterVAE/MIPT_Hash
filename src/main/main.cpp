#include <stdio.h>

#include "list.h"
#include "hash_table.h"
#include "hash_funcs.h"
#include "words.h"

static const size_t TABLE_SIZE = 4153;
static const char* const INPUT_FILENAME = "sources/file.fl";
static const char* const STAT_FILENAME = "sources/stats_crc32";
static size_t (*HASH_FUNC)(size_t hash_count, const char* word) = HashCRC_32;

static const size_t CYCLES = 100;

int main()
{   
    printf("Hash table starting...\n");

    HashTable* table = HashTableCreate(TABLE_SIZE, HASH_FUNC);
    if(!table)
    {
        printf("Failied to create hash table\n");
        return 1;
    }

    char* buffer = CreateBufferFromFile(INPUT_FILENAME);
    if(!buffer)
    {
        printf("Failed to create buffer\n");
        HashTableDestroy(table);
        return 1;
    }
    Info info = WordInfo(buffer);
    size_t word_count = info.words_count;
    size_t max_lenght = info.max_lenght;

    printf("Words count: %lu\nMax lenght: %lu\n", word_count, max_lenght);

    char** words = ParseIntoWords(buffer, word_count);

    HashTableLoad(table, word_count, words);
    //HashTableStat(table, STAT_FILENAME);

    size_t sum = 0;

    for(size_t i = 0; i < CYCLES; i++)
    {
        for(size_t j = 0; j < word_count; j++)
        {
            sum += HashTableRun(table, words[j]);
        }
    }

    printf("Summary: %lu\n", sum);

    free(words);
    free(buffer);
    HashTableDestroy(table);

    printf("Hash table ending...\n");
    return 0;
}