#pragma once

#include <stdio.h>

char* fileReadLine(FILE* file, char** buffer, int* bufferSize);

char* reallocBuffer(char* buffer, int* bufferSize);