#pragma once
#ifndef CND_H
#define CND_H

#include "graph.h"

/*
Used to measure connectivity of the graph.
Calculates the number of possible node pairs in a graph of N size.
f(N) = (N * (N - 1)) / 2 = f(N-1) + N - 1
*/
int f(int N);

//Computes the impact of removing a single node.
void evaluateNode(Graph* graph, int root, int globalVisited[], int* bestNode, int* bestGain);
void evaluateGraph(Graph* graph, int* bestNode, int* bestGain);
void fastRemoval(Graph* graph, int k);

#endif
