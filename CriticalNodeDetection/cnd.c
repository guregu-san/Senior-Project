#include "cnd.h"
#define MAXN 1000 //TODO: Make it work without this

int f(int N) {
    return (N * (N - 1)) / 2;
}

void evaluateNode(Graph* graph, int root, int globalVisited[], int* bestNode, int* bestGain) {
    int n = graph->nodeCount;

    // Initialize attributes for DFS
    int DFN[MAXN] = {0}, LOW[MAXN] = {0}, PARENT[MAXN];
    int ST_SIZE[MAXN], CUT_SIZE[MAXN];
    int localVisited[MAXN] = {0}, counted[MAXN] = {0}, isArt[MAXN] = {0};

    for (int i = 0; i < n; i++) {
        PARENT[i] = -1;
        ST_SIZE[i] = 1;
        CUT_SIZE[i] = 0;
        graph->nodes[i]->impact = 0;
    }

    int stack[MAXN];
    int top = 0;
    int time = 1;

    stack[top++] = root;
    localVisited[root] = 1;
    globalVisited[root] = 1;
    DFN[root] = LOW[root] = time++;

    while (top > 0) {
        int v = stack[top - 1];
        Node* node = graph->nodes[v];

        int pushed = 0;

        for (int i = 0; i < node->neighbourCount; i++) {
            int y = node->neighbours[i];

            if (graph->nodes[y]->deleted) continue;

            if (!localVisited[y]) {
                localVisited[y] = 1;
                globalVisited[y] = 1;
                stack[top++] = y;

                DFN[y] = LOW[y] = time++;
                PARENT[y] = v;
                ST_SIZE[y] = 1;

                pushed = 1;
                break;
            }
            else if (y != PARENT[v] && DFN[y] < LOW[v]) {
                LOW[v] = DFN[y];
            }
        }

        if (pushed) continue;

        // Backtrack
        top--;

        for (int i = 0; i < node->neighbourCount; i++) {
            int w = node->neighbours[i];

            if (graph->nodes[w]->deleted) continue;

            if (PARENT[w] == v) {

                if (LOW[w] >= DFN[v] && PARENT[v] != -1) {
                    isArt[v] = 1;
                    CUT_SIZE[v] += ST_SIZE[w];
                    graph->nodes[v]->impact += f(ST_SIZE[w]);
                }

                if (!counted[w]) {
                    counted[w] = 1;
                    ST_SIZE[v] += ST_SIZE[w];
                }

                if (LOW[w] < LOW[v])
                    LOW[v] = LOW[w];
            }
        }

        // Check if the node is articulation point,
        // if it's not the root in this evaluation.
        if (PARENT[v] == -1) {
            int children = 0;
            for (int i = 0; i < node->neighbourCount; i++) {
                int w = node->neighbours[i];
                if (PARENT[w] == v)
                    children++;
            }
            if (children > 1)
                isArt[v] = 1;
        }
    }

    int compSize = time - 1;

    for (int v = 0; v < n; v++) {
        if (!localVisited[v] || graph->nodes[v]->deleted)
            continue;

        if (isArt[v])
            graph->nodes[v]->impact += f(compSize - 1 - CUT_SIZE[v]);
        else
            graph->nodes[v]->impact += f(compSize - 1);

        int gain = f(compSize) - graph->nodes[v]->impact;

        // Update global best
        if (gain > *bestGain) {
            *bestGain = gain;
            *bestNode = v;
        }
    }
}

void evaluateGraph(Graph* graph, int* bestNode, int* bestGain) {
    int visited[MAXN] = {0};

    for (int i = 0; i < graph->nodeCount; i++) {
        if (!graph->nodes[i]->deleted && !visited[i]) {
            evaluateNode(graph, i, visited, bestNode, bestGain);
        }
    }
}

// Greedy CNDP solution
void fastRemoval(Graph* graph, int k) {
    printf("\nCritical Node Removal Order:\n");

    int removed = 0;

    while (removed < k) {

        int bestNode = -1;
        int bestGain = -1;

        evaluateGraph(graph, &bestNode, &bestGain);

        if (bestNode == -1)
            break;

        graph->nodes[bestNode]->deleted = 1;
        removed++;

        printf("Step %d: remove node %d (gain=%d)\n",
               removed, bestNode, bestGain);
    }
}