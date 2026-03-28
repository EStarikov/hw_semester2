#include "states.h"
#include <stdlib.h>

int main()
{
    int* states = NULL;
    unsigned* capitals = NULL;
    size_t n;
    size_t k;

    Heap** graph = readFromFile("graph.txt", &states, &capitals, &n, &k);
    if (graph == NULL) {
        return -1;
    }
    matchCityWithCapital(graph, states, capitals, n, k);
    printStates(states, capitals, n, k);
    freeGraph(&graph, n);
    free(states);
    free(capitals);
    return 0;
}