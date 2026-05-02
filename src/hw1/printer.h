#pragma once

#include <stdbool.h>
#include <stdio.h>

typedef struct Table Table;

// создает таблицу
Table* createTable();

// считывает из файла и добавляет в таблицу
void addToTable(Table* myTable, FILE* file);

// освобождает таблицу
void freeTable(Table** myTable);

// печатает из таблицы в файл
int printToFile(Table* myTable, FILE* file);
