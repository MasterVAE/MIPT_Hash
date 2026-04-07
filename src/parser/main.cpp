#include <stdio.h>
#include <ctype.h>
#include <string.h>

static const char* const OUT_FILENAME = "sources/file.fl";

int main(int argc, char* argv[])
{
    FILE* output = fopen(OUT_FILENAME, "w+");
    if(!output) return 1;

    for(size_t i = 1; i < argc; i++)
    {
        FILE* input = fopen(argv[i], "r+");
        if(!input)
        {
            fprintf(stderr, "Failed to open file: %s\n", argv[i]);
            fclose(output);
            return 1;
        }
        bool word = false;

        char letter = '\0';
        while((letter = fgetc(input)) != '\0' && letter != EOF)
        {
            if(isalpha(letter))
            {
                if(!word)
                {
                    word = true;
                }
                fputc(tolower(letter), output);
            }
            else
            {
                if(word)
                {
                    word = false;
                    fputc(' ', output);
                }
            }
        }

        fclose(input);
    }

    fclose(output);

    return 0;
}