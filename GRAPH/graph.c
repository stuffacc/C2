#include "graph.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Graph {
    int vertexCount;
    int** matrix;
} Graph;

Graph* mallocGraph(int vertexCount)
{
    Graph* graph = malloc(sizeof(Graph));

    int* array = malloc(sizeof(int) * vertexCount * vertexCount);

    int** matrix = malloc(sizeof(int*) * vertexCount);
    for (int i = 0; i < vertexCount; i++) {
        matrix[i] = array + (vertexCount * i);
    }

    for (int i = 0; i < vertexCount; i++) {
        for (int j = 0; j < vertexCount; j++) {
            if (i == j) {
                matrix[i][j] = 0;
            }

            else {
                matrix[i][j] = INT_MAX;
            }
        }
    }

    graph->matrix = matrix;
    graph->vertexCount = vertexCount;

    return graph;
}

void addRoad(Graph* graph, int city1, int city2, int len)
{
    graph->matrix[city1][city2] = len;
    graph->matrix[city2][city1] = len;
}

MinFreeCity findMinNearCity(Graph* graph, int currentVertex, int* cities, int* hasFreeCities)
{
    int vertexCount = graph->vertexCount;
    int** matrix = graph->matrix;

    *hasFreeCities = 0;

    for (int i = 0; i < vertexCount; i++) {
        if (cities[i] == -1) {
            *hasFreeCities = 1;
            break;
        }
    }

    int* nearCity = matrix[currentVertex];

    MinFreeCity minFreeCity = { INT_MAX, -1 };

    for (int i = 0; i < vertexCount; i++) {
        int cityIsFree = (cities[i] == -1);

        int distToCity = nearCity[i];

        if (cityIsFree && (distToCity != INT_MAX)) {
            if (distToCity < minFreeCity.dist) {
                minFreeCity.dist = distToCity;
                minFreeCity.vertex = i;
            }
        }
    }

    return minFreeCity;
}

void printGraph(Graph* graph)
{
    int** matrix = graph->matrix;
    int vertexCount = graph->vertexCount;

    for (int i = 0; i < vertexCount; i++) {
        for (int j = 0; j < vertexCount; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void freeGraph(Graph** graph)
{
    free((*graph)->matrix[0]);
    free((*graph)->matrix);
    free(*graph);

    *graph = NULL;
}