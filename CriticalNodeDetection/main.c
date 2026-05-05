#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "graph.h"
#include "cnd.h"

#define DEFAULT_SIZE 20
#define CRIT_NODES 3

void printMenu() {
    printf("1. Print graph\n2. Detect critical nodes\n3. Add node\n4. Remove node\n");
    printf("Enter option: ");
}


int main(int argc, char* argv[]) {
    if (argc > 3)
    {
        printf("Wrong number of arguments");
        return 1;
    }

    //Seed randomizer.
    srand((int)time(NULL));

    FILE* log = fopen("log.csv","w");
    if (!log)
    {
        printf("Log file couldn't be opened");
        return 2;
    }
    fprintf(log, "Node,Neighbours");

    int graphSize = DEFAULT_SIZE;
    int criticalNodes = CRIT_NODES;

    if (argc > 1)
        graphSize = atoi(argv[1]);

    if (argc > 2)
        criticalNodes = atoi(argv[2]);

    Graph* graph = createGraph(graphSize);
    if (!graph)
    {
        printf("Allocation for graph failed");
        return 3;
    }

    //struct Graph reducedGraph = *graph;
    //fastRemoval(&reducedGraph, criticalNodes);
    printGraph(graph, log);
    //printGraph(&reducedGraph, log);

    freeGraph(graph);
    fclose(log);

    return 0;
}
