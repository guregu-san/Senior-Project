#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

typedef struct {
    int id;
    int* neighbours;
    int neighbourCount;
    int deleted;
    int impact;
} Node;

typedef struct {
    Node** nodes;
    int nodeCount;
} Graph;

Graph* createGraph(int size);
void printGraph(Graph* graph, FILE* log);
void freeGraph(Graph* graph);

#endif
