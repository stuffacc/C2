#pragma once

typedef struct Graph Graph;

typedef struct MinFreeCity {
    int dist;
    int vertex;
} MinFreeCity;

Graph* mallocGraph(int vertexCount);

void addRoad(Graph* graph, int city1, int city2, int len);

MinFreeCity findMinNearCity(Graph* graph, int currentVertex, int* cities, int* hasFreeCities);

void printGraph(Graph* graph);

void freeGraph(Graph** graph);