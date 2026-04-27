#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define DEFAULT_SIZE 20
#define CONNECTION_CHANCE 30

/*************** Data Structures ***************/

struct Node {
    int id;
    int neighbourNum;
    int* neighbours;
    int deleted;
    int impact;
};

struct Graph {
    int nodeNum;
    struct Node** nodes;
};

/*************** Objective Function ***************/

int f(int N)
{
    return (N * (N - 1)) / 2;
}

/*************** Graph Creation ***************/

struct Graph* createGraph(int size)
{
    if (size < 2)
        return NULL;

    struct Graph* graph = malloc(sizeof(struct Graph));
    graph->nodes = malloc(size * sizeof(struct Node*));

    struct Node** nodes = graph->nodes;

    nodes[0] = malloc(sizeof(struct Node));
    nodes[0]->neighbours = malloc(sizeof(int));
    nodes[0]->id = 0;
    nodes[0]->neighbourNum = 0;
    nodes[0]->deleted = 0;

    graph->nodeNum = 1;

    for (int i = 1; i < size; i++)
    {
        nodes[i] = malloc(sizeof(struct Node));
        struct Node* current = nodes[i];

        current->id = i;
        current->deleted = 0;
        current->neighbours = malloc(sizeof(int));

        int first = rand() % graph->nodeNum;
        struct Node* neighbor = nodes[first];

        current->neighbours[0] = first;
        current->neighbourNum = 1;

        neighbor->neighbours = realloc(neighbor->neighbours,
            (neighbor->neighbourNum + 1) * sizeof(int));

        neighbor->neighbours[neighbor->neighbourNum++] = current->id;

        for (int k = 0; k < graph->nodeNum; k++)
        {
            int duplicate = 0;

            for (int j = 0; j < current->neighbourNum; j++)
                if (current->neighbours[j] == k)
                    duplicate = 1;

            if (rand() % 100 >= CONNECTION_CHANCE || current->id == k || duplicate)
                continue;

            current->neighbours = realloc(current->neighbours,
                (current->neighbourNum + 1) * sizeof(int));

            current->neighbours[current->neighbourNum++] = k;

            neighbor = nodes[k];

            neighbor->neighbours = realloc(neighbor->neighbours,
                (neighbor->neighbourNum + 1) * sizeof(int));

            neighbor->neighbours[neighbor->neighbourNum++] = current->id;
        }

        graph->nodeNum++;
    }

    return graph;
}

/*************** Print Graph ***************/

void printGraph(struct Graph* graph)
{
    printf("\nGraph:\n");

    for (int i = 0; i < graph->nodeNum; i++)
    {
        printf("Node %d: [ ", graph->nodes[i]->id);

        for (int j = 0; j < graph->nodes[i]->neighbourNum; j++)
            printf("%d ", graph->nodes[i]->neighbours[j]);

        printf("]\n");
    }
}

/*************** DFS Component Size ***************/

int dfsSize(struct Graph* graph, int v, int visited[])
{
    visited[v] = 1;

    int size = 1;

    struct Node* node = graph->nodes[v];

    for (int i = 0; i < node->neighbourNum; i++)
    {
        int n = node->neighbours[i];

        if (!visited[n] && !graph->nodes[n]->deleted)
            size += dfsSize(graph, n, visited);
    }

    return size;
}

/*************** Evaluate Node Impact ***************/

int evaluateNode(struct Graph* graph, int nodeID)
{
    struct Node* node = graph->nodes[nodeID];

    if (node->deleted)
        return -1;

    node->deleted = 1;

    int visited[graph->nodeNum];

    for (int i = 0; i < graph->nodeNum; i++)
        visited[i] = 0;

    int impact = 0;

    for (int i = 0; i < graph->nodeNum; i++)
    {
        if (!visited[i] && !graph->nodes[i]->deleted)
        {
            int size = dfsSize(graph, i, visited);
            impact += f(size);
        }
    }

    node->deleted = 0;

    return impact;
}

/*************** Evaluate Entire Graph ***************/

void evaluateGraph(struct Graph* graph)
{
    for (int i = 0; i < graph->nodeNum; i++)
        graph->nodes[i]->impact = evaluateNode(graph, i);
}

/*************** Fast Removal (CNDP) ***************/

void fastRemoval(struct Graph* graph, int k)
{
    printf("\nCritical Node Removal Order:\n");

    for (int step = 0; step < k; step++)
    {
        evaluateGraph(graph);

        int bestNode = -1;
        int bestImpact = -1;

        for (int i = 0; i < graph->nodeNum; i++)
        {
            struct Node* n = graph->nodes[i];

            if (!n->deleted && n->impact > bestImpact)
            {
                bestImpact = n->impact;
                bestNode = i;
            }
        }

        if (bestNode == -1)
            break;

        graph->nodes[bestNode]->deleted = 1;

        printf("Step %d: remove node %d (impact=%d)\n",
               step + 1, bestNode, bestImpact);
    }
}

/*************** Memory Cleanup ***************/

void freeGraph(struct Graph* graph)
{
    for (int i = 0; i < graph->nodeNum; i++)
    {
        free(graph->nodes[i]->neighbours);
        free(graph->nodes[i]);
    }

    free(graph->nodes);
    free(graph);
}

/*************** Main ***************/

int main(int argc, char* argv[])
{
    srand(time(NULL));

    int size = DEFAULT_SIZE;
    int k = 3;

    if (argc >= 2)
        size = atoi(argv[1]);

    if (argc >= 3)
        k = atoi(argv[2]);

    struct Graph* graph = createGraph(size);

    if (!graph)
    {
        printf("Graph allocation failed\n");
        return 1;
    }

    printGraph(graph);

    fastRemoval(graph, k);

    freeGraph(graph);

    return 0;
}