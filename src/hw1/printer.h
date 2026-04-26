#pragma once

#include <stdbool.h>
#include <stdio.h>

typedef struct Table Table;
Table* createTable();
void addToTable(Table* myTable, FILE* file); // считывает из файла и добавляет в таблицу
void freeTable(Table** myTable); // освобождает таблицу
int printToFile(Table* myTable, FILE* file); // печатает из таблицы в файл
