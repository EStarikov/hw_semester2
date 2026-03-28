#include "states.h"
#include <stdio.h>
#include <stdlib.h>

struct Road {
    int city;
    size_t len;
};
struct Heap {
    Road** roads;
    size_t size;
};

static Heap* initHeap()
{
    Heap* heap = calloc(1, sizeof(Heap));
    return heap;
}

static void swap(Road* r1, Road* r2)
{
    Road temp = *r1;
    *r1 = *r2;
    *r2 = temp;
}

static void siftDown(Heap* heap, size_t i)
{
    while (2 * i + 1 < heap->size) {
        size_t left = 2 * i + 1;
        size_t right = 2 * i + 2;
        size_t j = left;
        if (right < heap->size && (heap->roads[right])->len < (heap->roads[left])->len) {
            j = right;
        }
        if ((heap->roads[i])->len <= (heap->roads[j])->len) {
            break;
        }
        swap(heap->roads[i], heap->roads[j]);
        i = j;
    }
}

static void siftUp(Heap* heap, size_t i)
{
    if (i == 0) {
        return;
    }
    while ((heap->roads[i])->len < (heap->roads[(i - 1) / 2])->len) {
        swap(heap->roads[i], heap->roads[(i - 1) / 2]);
        i = (i - 1) / 2;
        if (i == 0) {
            return;
        }
    }
}

static Road* extractMin(Heap* heap)
{
    if (heap->size == 0) {
        return NULL;
    }
    Road* min = heap->roads[0];
    heap->roads[0] = heap->roads[heap->size - 1];
    heap->size--;
    siftDown(heap, 0);
    return min;
}

static int insert(Heap* heap, int city, size_t len)
{
    Road* newRoad = malloc(sizeof(Road));
    if (newRoad == NULL) {
        return -1;
    }
    newRoad->city = city;
    newRoad->len = len;
    ++heap->size;
    heap->roads = realloc(heap->roads, heap->size * sizeof(Road*));
    if (heap->roads == NULL) {
        free(newRoad);
        return -1;
    }
    heap->roads[heap->size - 1] = newRoad;
    siftUp(heap, heap->size - 1);
    return 0;
}

void freeGraph(Heap*** graph, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        for (size_t j = 0; j < ((*graph)[i])->size; ++j) {
            free(((*graph)[i])->roads[j]);
        }
        free(((*graph)[i])->roads);
        free((*graph)[i]);
    }
    free(*graph);
}

Heap** readFromFile(char* filename, int** states, unsigned** capitals, size_t* n, size_t* k)
{
    FILE* input = fopen(filename, "r");
    if (input == NULL) {
        printf("file");
        return NULL;
    }

    fscanf(input, "%zu", n);
    Heap** graph = malloc(sizeof(Heap*) * (*n));
    if (graph == NULL) {
        printf("init");
        fclose(input);
        return NULL;
    }
    *states = malloc(sizeof(int) * (*n));
    if (states == NULL) {
        free(graph);
        fclose(input);
        printf("states");
        return NULL;
    }
    for (size_t i = 0; i < (*n); ++i) {
        graph[i] = initHeap();
        (*states)[i] = -1;
        if (graph[i] == NULL) {
            freeGraph(&graph, i);
            fclose(input);
            return NULL;
        }
    }

    unsigned m;
    fscanf(input, "%u", &m);
    int i, j;
    size_t len;
    int err = 0;
    for (int l = 0; l < m; ++l) {
        fscanf(input, "%d", &i);
        fscanf(input, "%d", &j);
        fscanf(input, "%zu", &len);
        err = insert(graph[i], j, len);
        if (err != 0) {
            freeGraph(&graph, (*n));
            fclose(input);
            return NULL;
        }
        err = insert(graph[j], i, len);
        if (err != 0) {
            freeGraph(&graph, (*n));
            fclose(input);
            return NULL;
        }
    }

    int num;
    fscanf(input, "%zu", k);
    *capitals = malloc(sizeof(unsigned) * (*k));
    if (capitals == NULL) {
        freeGraph(&graph, (*n));
        fclose(input);
        printf("capitals");
        return NULL;
    }
    for (size_t l = 0; l < (*k); ++l) {
        fscanf(input, "%d", &num);
        (*states)[num] = num;
        (*capitals)[l] = num;
    }

    fclose(input);
    return graph;
}

void matchCityWithCapital(Heap** graph, int* states, unsigned* capitals, size_t n, size_t k)
{
    size_t numOfUnmatchedCities = n - k;
    while (numOfUnmatchedCities != 0) {
        for (size_t i = 0; i < k; ++i) {
            unsigned cap = capitals[i];
            Road* road = extractMin(graph[cap]);
            while (road != NULL) {
                int city = road->city;
                free(road);
                road = NULL;
                if (states[city] == -1) {
                    states[city] = cap;
                    numOfUnmatchedCities--;
                    while ((graph[city])->size != 0) {
                        road = extractMin(graph[city]);
                        if (states[road->city] != -1) {
                            insert(graph[cap], road->city, road->len);
                        }
                        free(road);
                        road = NULL;
                    }
                    break;
                } else {
                    road = extractMin(graph[cap]);
                }
            }
        }
    }
}

void printStates(int* states, unsigned* capitals, size_t n, size_t k)
{
    for (size_t i = 0; i < k; ++i) {
        printf("%u: ", capitals[i]);
        for (size_t j = 0; j < n; ++j) {
            if (states[j] == capitals[i]) {
                printf("%zu, ", j);
            }
        }
        printf("\n");
    }
}