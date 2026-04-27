#include "cnd.h"

/*
Used to measure connectivity of the graph.
Calculates the number of possible node pairs in a graph of N size.
f(N) = (N * (N - 1)) / 2 = f(N-1) + N - 1
*/
int f(int N) {
    return (N * (N - 1)) / 2;
}

//Standard DFS used to compute the size of a connected component.
int dfsSize(Graph* graph, int v, int visited[]) {
    visited[v] = 1;

    int size = 1;

    Node* node = graph->nodes[v];

    for (int i = 0; i < node->neighbourCount; i++)
    {
        int neighbourID = node->neighbours[i];

        //Skip deleted nodes.
        if (!visited[neighbourID] && !graph->nodes[neighbourID]->deleted)

            size += dfsSize(graph, neighbourID, visited);
    }

    return size;
}

//Computes the impact of removing a single node.
int evaluateNode(Graph* graph, int nodeID) {
    Node* node = graph->nodes[nodeID];

    if (node->deleted)
        return -1;

    //Temporarily remove the node
    node->deleted = 1;

    int visited[graph->nodeCount];

    for (int i = 0; i < graph->nodeCount; i++)
        visited[i] = 0;

    int impact = 0;

    //Compute all connected components
    for (int i = 0; i < graph->nodeCount; i++)
    {
        if (!visited[i] && !graph->nodes[i]->deleted)
        {
            int size = dfsSize(graph, i, visited);

            impact += f(size);
        }
    }

    //Restore the node
    node->deleted = 0;

    return impact;
}

void evaluateGraph(Graph* graph) {
    for (int i = 0; i < graph->nodeCount; i++)
        graph->nodes[i]->impact = evaluateNode(graph, i);
}

void fastRemoval(Graph* graph, int k) {
    printf("\nCritical Node Removal Order:\n");

    for (int step = 0; step < k; step++)
    {
        //Compute impact for all nodes.
        evaluateGraph(graph);

        int bestNode = -1;

        int bestImpact = -1;

        //Choose node with largest impact.
        for (int i = 0; i < graph->nodeCount; i++)
        {
            Node* n = graph->nodes[i];

            if (!n->deleted && n->impact > bestImpact)
            {
                bestImpact = n->impact;

                bestNode = i;
            }
        }

        if (bestNode == -1)
            break;

        //Permanently remove node
        graph->nodes[bestNode]->deleted = 1;

        printf("Step %d: remove node %d (impact=%d)\n",
               step + 1, bestNode, bestImpact);
    }
}
