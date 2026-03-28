#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl_tree.h"
#include "file.h"

typedef enum Command {
    ADD,
    FIND,
    DELETE,
    SAVE,
    QUITE,
    UNKNOWN,
    EMPTY
} Command;

int quite = 0;
char* filename = NULL;

void parseAdd(char* input, AVLTree* AVLTree)
{
    AiroportsData* airoportsData = toAiroportsData(input);
    if (airoportsData == NULL) {
        printf("Неверный формат команды 'add'\nadd CODE:NAME\n\n");
        return;
    }

    insertToAVLTree(AVLTree, airoportsData);
    printf("Аэропорт '%s' добавлен в базу.\n", airoportsData->code);
}

void parseFind(char* input, AVLTree* AVLTree)
{
    AiroportsData* airoportsData = findByCodeAVLTree(AVLTree, input);
    if (airoportsData == NULL) {
        printf("Аэропорт с кодом '%s' не найден в базе.\n", input);
        return;
    }

    printf("%s → %s\n", airoportsData->code, airoportsData->name);
}

void parseDelete(char* input, AVLTree* AVLTree)
{
    deleteByCodeAVLTree(AVLTree, input);
    printf("Аэропорт '%s' удалён из базы, если там был.\n", input);
}

void save(AVLTree* AVLTree)
{
    FILE* file = fopen(filename, "w");
    int count = saveDataToFile(AVLTree, file);
    printf("База сохранена: %d аэропортов.\n", count);
    fclose(file);
}

void parseCommand(char* input, AVLTree* AVLTree)
{
    char* startInput = input;

    Command command = EMPTY;
    while (*input != '\0') {
        char inputLast = *(input + 1);
        *(input + 1) = '\0';
        if (strcmp(startInput, "add") == 0) {
            command = ADD;
        } else if (strcmp(startInput, "find") == 0) {
            command = FIND;
        }

        else if (strcmp(startInput, "delete") == 0) {
            command = DELETE;
        }

        else if (strcmp(startInput, "save") == 0) {
            command = SAVE;
        }

        else if (strcmp(startInput, "quite") == 0) {
            command = QUITE;
        }

        else {
            command = EMPTY;
        }

        *(input + 1) = inputLast;
        if (command != EMPTY) {
            input += 2;
            break;
        }

        input++;
    }

    if (command == ADD) {
        parseAdd(input, AVLTree);
    }

    else if (command == FIND) {
        parseFind(input, AVLTree);
    }

    else if (command == DELETE) {
        parseDelete(input, AVLTree);
    }

    else if (command == SAVE) {
        save(AVLTree);
    }

    else if (command == QUITE) {
        quite = 1;
    }

    else {
        printf("Команда не существует\n\n");
    }
}

int main(int argc, char** argv)
{

    for (int i = 1; i < argc; i++) {
        filename = argv[i];
    }

    if (filename == NULL) {
        printf("FILE NAME IS NULL\n");
        return -1;
    }

    FILE* file = NULL;

    file = fopen(filename, "r");

    if (file == NULL) {
        printf("FILE NOT FOUND\n");
        return -1;
    }

    // load data from file
    int bufferSize = 16;
    char* buffer = malloc(sizeof(char) * bufferSize);

    AVLTree* AVLTree = mallocAVLTree();

    char* nextLine = fileReadLine(file, &buffer, &bufferSize);
    int count = 0;
    while (nextLine[0] != '\0') {
        AiroportsData* airoportsData = toAiroportsData(nextLine);
        insertToAVLTree(AVLTree, airoportsData);
        count++;

        nextLine = fileReadLine(file, &buffer, &bufferSize);
    }

    fclose(file);

    // printAVLTree(AVLTree);

    printf("Загружено %d аэропортов. Система готова к работе.\n\n", count);

    size_t inputBuffSize = bufferSize * 2;
    char* input = malloc(sizeof(char) * (inputBuffSize));

    int nread = 0;

    while ((nread = getline(&input, &inputBuffSize, stdin)) != -1) {
        input[nread - 1] = '\0';
        parseCommand(input, AVLTree);

        if (quite) {
            free(buffer);
            free(input);
            freeAVLTree(&AVLTree);

            return 0;
        }
    }

    return 0;
}