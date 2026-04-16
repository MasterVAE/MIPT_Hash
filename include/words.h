#ifndef WORDS_H
#define WORDS_H

struct Info
{
    size_t words_count;
    size_t max_lenght;
};

size_t FileLen(FILE* file);
Info WordInfo(const char* buffer);
char** ParseIntoWords(char* buffer, size_t count);
char* CreateBufferFromFile(const char* filename);

#endif // WORDS_H