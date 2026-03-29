#include "graph.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    char* filename = NULL;

    for (int i = 1; i < argc; i++) {
        filename = argv[i];
    }

    if (filename == NULL) {
        printf("Имя файла не передано\n");
        return -1;
    }

    char* filePath = malloc(sizeof(char) * (strlen(filename) + 9));

    sprintf(filePath, "../data/%s", filename);

    FILE* file = fopen(filePath, "r");
    free(filePath);

    if (file == NULL) {
        printf("Файл не найден\n");
        return -1;
    }

    int cityCount = 0;
    fscanf(file, "%d\n", &cityCount);

    int roadCount = 0;
    fscanf(file, "%d\n", &roadCount);

    Graph* graph = mallocGraph(cityCount);

    for (int i = 0; i < roadCount; i++) {
        int num1 = 0;
        int num2 = 0;
        int len = 0;

        fscanf(file, "%d %d %d\n", &num1, &num2, &len);

        addRoad(graph, num1, num2, len);
    }

    int capitalCount = 0;
    fscanf(file, "%d\n", &capitalCount);

    int* cities = malloc(sizeof(int) * cityCount);
    for (int i = 0; i < cityCount; i++) {
        cities[i] = -1;
    }

    int* capitals = malloc(sizeof(int) * capitalCount);

    for (int i = 0; i < capitalCount; i++) {
        int capital = -1;

        fscanf(file, "%d\n", &capital);

        capitals[i] = capital;
        cities[capital] = capital;
    }

    int hasFreeCities = 1;

    while (hasFreeCities) {
        for (int capitalNum = 0; capitalNum < capitalCount; capitalNum++) {
            if (!hasFreeCities) {
                break;
            }

            int capital = capitals[capitalNum];

            MinFreeCity minFreeCity = { INT_MAX, -1 };
            for (int cityNum = 0; cityNum < cityCount; cityNum++) {
                if (capital == cities[cityNum]) {
                    MinFreeCity minFreeCityTemp = findMinNearCity(graph, cityNum, cities, &hasFreeCities);
                    if (minFreeCityTemp.dist < minFreeCity.dist) {
                        minFreeCity.dist = minFreeCityTemp.dist;
                        minFreeCity.vertex = minFreeCityTemp.vertex;
                    }
                }
            }

            if (minFreeCity.dist != INT_MAX) {
                cities[minFreeCity.vertex] = capital;
            }
        }
    }

    for (int capitalNum = 0; capitalNum < capitalCount; capitalNum++) {
        int capital = capitals[capitalNum];

        printf("Страна %d, города: ", capital);

        for (int cityNum = 0; cityNum < cityCount; cityNum++) {
            if (capital == cities[cityNum]) {
                printf("%d ", cityNum);
            }
        }

        printf("\n");
    }

    free(cities);
    free(capitals);
    fclose(file);

    freeGraph(&graph);

    return 0;
}