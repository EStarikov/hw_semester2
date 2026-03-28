#include "states.h"
#include <stddef.h>

int main()
{
    int* states;
    unsigned* capitals;
    size_t n;
    size_t k;

    Heap** graph = eadFromFile("graph.txt", states, capitals, &n, &k);
    matchCityWithCapital(graph, states, capitals, n, k);
    printStates(states, capitals, n, k);
    freeGraph(graph, n);
    free(states);
    free(capitals);
    return 0;
}