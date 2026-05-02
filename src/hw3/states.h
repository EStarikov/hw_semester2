#pragma once
#include <stddef.h>

typedef struct Heap Heap;

// освобождает граф
void freeGraph(Heap*** graph, size_t len);

// ситывает граф из файла
Heap** readFromFile(char* filename, int** states, unsigned** capitals, size_t* n, size_t* k);

// сопоставляет города с столицами
void matchCityWithCapital(Heap** graph, int* states, const unsigned* capitals, size_t n, size_t k);

// печатает государства
void printStates(const int* states, const unsigned* capitals, size_t n, size_t k);
