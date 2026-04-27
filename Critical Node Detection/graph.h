#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>

typedef struct Node {
    int id;
    int neighbourCount;
    int* neighbours;
    int deleted;
    int impact;
} Node;

typedef struct Graph {
    int nodeCount;
    Node** nodes;
} Graph;

Graph* createGraph(int size);
void addNodeToGraph(Node* node, Graph* graph);
void removeNodeFromGraph(Node* node, Graph* graph);
void printGraph(Graph* graph, FILE* log);
void freeGraph(Graph* graph);

#endif
