#include "file.h"

#include <stdlib.h>

char* fileReadLine(FILE* file, char** buffer, int* bufferSize)
{
    int ch = fgetc(file);

    int readChars = 0;

    while (ch != '\n' && ch != EOF) {
        if (readChars == (*bufferSize * 3 / 4)) {
            *buffer = reallocBuffer(*buffer, bufferSize);
        }

        (*buffer)[readChars] = ch;

        ch = fgetc(file);
        readChars++;
    }

    (*buffer)[readChars] = '\0';

    return *buffer;
}

char* reallocBuffer(char* buffer, int* bufferSize)
{
    *bufferSize *= 2;
    return realloc(buffer, *bufferSize);
}
