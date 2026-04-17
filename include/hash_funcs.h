#ifndef HASH_FUNCS_H
#define HASH_FUNCS_H

#include <stdlib.h>

size_t HashZero(size_t hash_count, const char* word);
size_t HashFirst(size_t hash_count, const char* word);
size_t HashLen(size_t hash_count, const char* word);
size_t HashSumm(size_t hash_count, const char* word);
size_t HashRoll(size_t hash_count, const char* word);
size_t HashCRC_32(size_t hash_count, const char* word);


#endif // HASH_FUNCS_H