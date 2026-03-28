#pragma once
#include <stdbool.h>
#include <stdio.h>
int myGetline(char** line, FILE* file);
int countWords(const char* line, int len);
char** getWords(const char* line, int len, int numOfColumns);
void maxWord(char** words, int* maxs, int numOfColumns);
void freeWords(char** words, int numOfColumns);
bool isNumber(char* word);
int addToTable(char**** table, int** maxs, FILE* file, int* numOfColumns);
void freeTable(char*** table, int numOfColumns, int numOfLines);
int printGraphic(const int* maxs, int len, int numOfColumns, char s, FILE* file);
int printToFile(char*** table, const int* maxs, int numOfColumns, int numOfLines, FILE* file);
