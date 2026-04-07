#include <assert.h>
#include <string.h>

#include "hash_funcs.h"

size_t HashZero(size_t hash_count, const char* word)
{
    assert(word);

    return 0;
}

size_t HashFirst(size_t hash_count, const char* word)
{
    assert(word);

    return (size_t)word[0]%hash_count;
}

size_t HashLen(size_t hash_count, const char* word)
{
    assert(word);

    return strlen(word)%hash_count;
}

size_t HashSumm(size_t hash_count, const char* word)
{
    assert(word);

    size_t len = strlen(word);
    size_t summ = 0;
    for(size_t i = 0; i < len; i++)
    {
        summ += (size_t)word[i];
    }

    return summ%hash_count;
}
size_t HashRoll(size_t hash_count, const char* word)
{
    assert(word);

    size_t value = 0;

    size_t len = strlen(word);
    for(size_t i = 0; i < len; i++)
    {
        value ^= (size_t)word[i];
        value = (value << 1) | (value >> (sizeof(size_t) * 8 - 1));
    }

    return value%hash_count;

}
size_t HashCRC_32(size_t hash_count, const char* word)
{
    assert(word);

    size_t value = 5381;

    size_t len = strlen(word);
    for(size_t i = 0; i < len; i++)
    {
        value ^= word[i];
        value = (value << 1) | (value >> (sizeof(size_t) * 8 - 1));
    }

    return value%hash_count;
}