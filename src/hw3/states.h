#pragma once

typedef struct Road Road;
typedef struct Heap Heap;

void freeGraph(Heap*** graph, size_t len);
Heap** readFromFile(char* filename, int* states, unsigned* capitals, size_t n, size_t k);
void matchCityWithCapital(Heap** graph, int* states, unsigned* capitals, size_t n, size_t k);
void printStates(int* states, unsigned* capitals, size_t n, size_t k);
