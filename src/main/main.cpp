#include <stdio.h>
#include <assert.h>
#include <ctype.h>

#include "list.h"
#include "hash_table.h"
#include "hash_funcs.h"

static const size_t TABLE_SIZE = 4153;
static const char* const INPUT_FILENAME = "sources/file.fl";
static const char* const STAT_FILENAME = "sources/stats_crc_32";
static size_t (*HASH_FUNC)(size_t hash_count, const char* word) = HashCRC_32;

size_t FileLen(FILE* file);
size_t WordCount(const char* buffer);
char** ParseIntoWords(char* buffer, size_t count);

char* CreateBufferFromFile(const char* filename);

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
    size_t count = WordCount(buffer);

    printf("Words count: %lu\n", count);

    char** words = ParseIntoWords(buffer, count);

    HashTableRun(table, count, words);
    HashTableStat(table, STAT_FILENAME);

    free(words);
    free(buffer);
    HashTableDestroy(table);

    printf("Hash table ending...\n");
    return 0;
}

size_t FileLen(FILE* file)
{
    assert(file);

    fseek(file, 0L, SEEK_END);
    size_t size = (size_t)ftell(file);
    fseek(file, 0L, SEEK_SET);

    return size;
}

size_t WordCount(const char* buffer)
{
    assert(buffer);

    size_t count = 0;

    bool word = false;

    while(*buffer != '\0')
    {
        if(isalpha(*buffer))
        {
            if(!word)
            {
                word = true;
                count++;
            }
        }
        else
        {
            word = false;
        }

        buffer++;
    }

    return count;
}

char* CreateBufferFromFile(const char* filename)
{
    assert(filename);

    FILE* file = fopen(filename, "rb");
    if(!file)
    {
        return NULL;
    }

    size_t size = FileLen(file);

    char* buffer = (char*)calloc(size + 1, sizeof(char));
    fread(buffer, size + 1, 1, file);

    fclose(file);

    return buffer;
}

char** ParseIntoWords(char* buffer, size_t count)
{
    assert(buffer);

    char** words = (char**)calloc(count, sizeof(char*));
    if(!words) return NULL;

    size_t counter = 0;

    bool word = false;

    while(*buffer != '\0')
    {
        if(isalpha(*buffer))
        {
            if(!word)
            {
                word = true;
                words[counter++] = buffer;
            }
        }
        else
        {
            word = false;
            *buffer = '\0';
        }

        buffer++;
    }

    return words;
}