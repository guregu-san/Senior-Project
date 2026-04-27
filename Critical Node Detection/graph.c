#include <stdlib.h>
#include "graph.h"
#define CONNECTION_CHANCE 30

Graph* createGraph(int size) {
    if (size < 2)
        return NULL;

    Graph* graph = (Graph*)malloc(sizeof(Graph));
    printf("Allocated %d bytes for graph struct\n", (int)sizeof(*graph));

    graph->nodes = (Node**)malloc(size * sizeof(Node*));
    printf("Allocated %d bytes for node array\n", (int)sizeof(*(graph->nodes)));
    Node** nodes = graph->nodes;

    //Initialize first node.
    nodes[0] = malloc(sizeof(Node));
    nodes[0]->neighbours = malloc(sizeof(int));
    nodes[0]->id = 0;
    nodes[0]->neighbourCount = 0;
    nodes[0]->deleted = 0;
    graph->nodeCount = 1;

    //Initialize other nodes.
    for (int i = 1; i < size; i++)
    {
        //Allocate memory for node.
        nodes[i] = malloc(sizeof(Node));
        Node* currentNode = nodes[i];

        //Set node ID and allocate memory for neighbour list.
        currentNode->id = i;
        currentNode->deleted = 0;
        currentNode->neighbours = malloc(sizeof(int));

        //Randomly choose the first neighbour for the node.
        int firstNeighbourID = rand() % graph->nodeCount;
        Node* neighbourNode = nodes[firstNeighbourID];

        currentNode->neighbours[0] = firstNeighbourID;
        currentNode->neighbourCount = 1;

        //Resize the neighbour list of this first neighbour.
        neighbourNode->neighbours = realloc(neighbourNode->neighbours, (neighbourNode->neighbourCount+1) * sizeof(int));
        neighbourNode->neighbours[neighbourNode->neighbourCount] = currentNode->id;
        neighbourNode->neighbourCount++;

        //Randomly add more edges to the node.
        for (int k = 0; k < graph->nodeCount; k++)
        {
            int duplicate = 0;
            for (int j = 0; j < currentNode->neighbourCount; j++)
            {
                if (currentNode->neighbours[j] == k)
                    duplicate = 1;
            }

            if (rand() % 100 >= CONNECTION_CHANCE || currentNode->id == k || duplicate)
                continue;

            currentNode->neighbours = realloc(currentNode->neighbours, (currentNode->neighbourCount+1) * sizeof(int));
            currentNode->neighbours[currentNode->neighbourCount] = k;
            currentNode->neighbourCount++;

            neighbourNode = nodes[k];
            neighbourNode->neighbours = realloc(graph->nodes[k]->neighbours, (graph->nodes[k]->neighbourCount+1) * sizeof(int));
            neighbourNode->neighbours[graph->nodes[k]->neighbourCount] = currentNode->id;
            neighbourNode->neighbourCount++;
        }

        graph->nodeCount++;
    }
    return graph;
}

void addNodeToGraph(Node* node, Graph* graph) {
    //graph.nodes[graph.nodes.size()] = node;
}

void removeNodeFromGraph(Node* node, Graph* graph) {
   // graph.nodes[graph.nodes.size()] = node;
}

void printGraph(Graph* graph, FILE* log) {
    printf("\n");
    fprintf(log, "\n");

    for (int i = 0; i < graph->nodeCount; i++)
    {
        printf("Node %d: [%d", graph->nodes[i]->id, graph->nodes[i]->neighbours[0]);
        fprintf(log, "%d,\"%d", graph->nodes[i]->id, graph->nodes[i]->neighbours[0]);

        for (int k = 1; k < graph->nodes[i]->neighbourCount; k++)
        {
            printf(" %d", graph->nodes[i]->neighbours[k]);
            fprintf(log, ",%d", graph->nodes[i]->neighbours[k]);
        }

        printf("]\n");
        fprintf(log, "\"\n");
    }
}

void freeGraph(Graph* graph) {
    for (int i = 0; i < graph->nodeCount; i++)
    {
        free(graph->nodes[i]->neighbours);
        free(graph->nodes[i]);
    }

    free(graph->nodes);
    free(graph);
}
