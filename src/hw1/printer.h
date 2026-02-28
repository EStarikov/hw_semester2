#pragma once
#include <stdio.h>
int myGetline(char** line, FILE* file);
int countWords(const char* line, int len);
char** getWords(const char* line, int len, int count);
