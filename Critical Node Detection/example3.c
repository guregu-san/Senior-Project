#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAXN 10000

// ---------------- Graph ----------------
typedef struct Node {
    int* neighbours;
    int neighbourCount;
    int deleted;
    int impact;
} Node;

typedef struct Graph {
    Node* nodes[MAXN];
    int nodeCount;
} Graph;

// ---------------- f(N) ----------------
int f(int N) {
    return (N * (N - 1)) / 2;
}

// ---------------- Priority Queue ----------------
typedef struct {
    int v;
    int impact;
} HeapNode;

HeapNode heap[MAXN];
int heapSize = 0;

void swap(HeapNode* a, HeapNode* b) {
    HeapNode t = *a; *a = *b; *b = t;
}

void heapPush(int v, int impact) {
    heap[heapSize] = (HeapNode){v, impact};
    int i = heapSize++;

    while (i > 0) {
        int p = (i - 1) / 2;
        if (heap[p].impact <= heap[i].impact) break;
        swap(&heap[p], &heap[i]);
        i = p;
    }
}

HeapNode heapPop() {
    HeapNode res = heap[0];
    heap[0] = heap[--heapSize];

    int i = 0;
    while (1) {
        int l = 2*i+1, r = 2*i+2, s = i;
        if (l < heapSize && heap[l].impact < heap[s].impact) s = l;
        if (r < heapSize && heap[r].impact < heap[s].impact) s = r;
        if (s == i) break;
        swap(&heap[i], &heap[s]);
        i = s;
    }
    return res;
}

// ---------------- EVALUATE (Algorithm 3) ----------------
int evaluate(Graph* graph, int root) {
    int n = graph->nodeCount;

    int DFN[MAXN] = {0}, LOW[MAXN] = {0}, PARENT[MAXN];
    int ST_SIZE[MAXN], CUT_SIZE[MAXN];
    int visited[MAXN] = {0}, counted[MAXN] = {0}, isArt[MAXN] = {0};

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
    visited[root] = 1;
    DFN[root] = LOW[root] = time++;

    while (top > 0) {
        int v = stack[top - 1];
        Node* node = graph->nodes[v];

        int pushed = 0;

        for (int i = 0; i < node->neighbourCount; i++) {
            int y = node->neighbours[i];

            if (graph->nodes[y]->deleted) continue;

            if (!visited[y]) {
                visited[y] = 1;
                stack[top++] = y;

                DFN[y] = LOW[y] = time++;
                PARENT[y] = v;
                ST_SIZE[y] = 1;

                pushed = 1;
                break;
            }
            else if (y != PARENT[v]) {
                if (DFN[y] < LOW[v])
                    LOW[v] = DFN[y];
            }
        }

        if (pushed) continue;

        // -------- backtrack --------
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

        // root articulation
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

    int best = -1;
    int bestImpact = INT_MAX;

    for (int v = 0; v < n; v++) {
        if (!visited[v] || graph->nodes[v]->deleted)
            continue;

        if (isArt[v])
            graph->nodes[v]->impact += f(compSize - 1 - CUT_SIZE[v]);
        else
            graph->nodes[v]->impact += f(compSize - 1);

        if (graph->nodes[v]->impact < bestImpact) {
            bestImpact = graph->nodes[v]->impact;
            best = v;
        }
    }

    return best;
}

// ---------------- GREEDY CNDP (Algorithm 2) ----------------
void fastRemoval(Graph* graph, int k) {
    printf("\nCritical Node Removal Order:\n");

    int q = evaluate(graph, 0);
    if (q != -1)
        heapPush(q, graph->nodes[q]->impact);

    int removed = 0;

    while (removed < k && heapSize > 0) {
        HeapNode cur = heapPop();
        int v = cur.v;

        if (graph->nodes[v]->deleted)
            continue;

        graph->nodes[v]->deleted = 1;
        removed++;

        printf("Step %d: remove node %d (impact=%d)\n",
               removed, v, cur.impact);

        // Only evaluate neighbors (key optimization!)
        for (int i = 0; i < graph->nodes[v]->neighbourCount; i++) {
            int b = graph->nodes[v]->neighbours[i];

            if (!graph->nodes[b]->deleted) {
                int w = evaluate(graph, b);
                if (w != -1)
                    heapPush(w, graph->nodes[w]->impact);
            }
        }
    }
}

// ---------------- Example ----------------
Node* createNode(int deg) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->neighbours = (int*)malloc(sizeof(int) * deg);
    n->neighbourCount = 0;
    n->deleted = 0;
    n->impact = 0;
    return n;
}

void addEdge(Graph* g, int u, int v) {
    g->nodes[u]->neighbours[g->nodes[u]->neighbourCount++] = v;
    g->nodes[v]->neighbours[g->nodes[v]->neighbourCount++] = u;
}

int main() {
    Graph g;
    g.nodeCount = 6;

    for (int i = 0; i < 6; i++)
        g.nodes[i] = createNode(10);

    addEdge(&g,0,1);
    addEdge(&g,1,2);
    addEdge(&g,1,3);
    addEdge(&g,3,4);
    addEdge(&g,3,5);

    fastRemoval(&g, 3);

    return 0;
}