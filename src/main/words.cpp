#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>

#include "words.h"

size_t FileLen(FILE* file)
{
    assert(file);

    fseek(file, 0L, SEEK_END);
    size_t size = (size_t)ftell(file);
    fseek(file, 0L, SEEK_SET);

    return size;
}

Info WordInfo(const char* buffer)
{
    assert(buffer);

    size_t count = 0;
    size_t max_lenght = 0;
    size_t lenght = 0;
    bool word = false;

    while(*buffer != '\0')
    {
        if(*buffer >= 'a' && *buffer <= 'z')
        {
            if(!word)
            {
                lenght = 1;
                word = true;
                count++;
            }
            lenght++;
        }
        else
        {
            if(lenght > max_lenght) max_lenght = lenght;
            word = false;
        }

        buffer++;
    }

    if(lenght > max_lenght) max_lenght = lenght;

    return {count, max_lenght};
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
        if(*buffer <= 'z' && *buffer >= 'a')
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