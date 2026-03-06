#include "printer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int myGetline(char** line, FILE* file)
{
    int n = 128;
    int i = 0;
    *line = (char*)malloc(sizeof(char) * n);
    if (*line == NULL) {
        return -1;
    }
    int c = fgetc(file);
    while (c != '\n' && c != EOF) {
        (*line)[i] = (char)c;
        i++;
        c = fgetc(file);
        if (i == n - 2) {
            n *= 2;
            *line = (char*)realloc(*line, sizeof(char) * n);
            if (*line == NULL) {
                return -1;
            }
        }
    }
    (*line)[i] = '\0';
    if (c == EOF) {
        return -1;
    }
    return i + 1;
}

int countWords(const char* line, int len)
{
    bool quotes = 0;
    int count = 1;
    for (int i = 0; i < len; i++) {
        if (line[i] == ',' && !quotes) {
            count++;
        }
        if (line[i] == '"') {
            quotes = !quotes;
        }
    }
    return count;
}

char** getWords(const char* line, int len, int numOfColumns)
{
    char** words = (char**)calloc(numOfColumns, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    char* word;
    if (len <= 0) {
        words[0] = (char*)malloc(1);
        if (words[0] != NULL) {
            words[0][0] = '\0';
            return words;
        } else {
            free(words[0]);
            free(words);
            return NULL;
        }
    } else {
        word = (char*)malloc(sizeof(char) * len);
    }
    if (word == NULL) {
        free(words);
        return NULL;
    }
    bool quotes = 0;
    int j = 0;
    int k = 0;
    for (int i = 0; i < len; i++) {
        if (k < numOfColumns) {
            if ((line[i] == ',' && quotes == 0) || line[i] == '\0') {
                if (j != 0) {
                    words[k] = (char*)malloc(sizeof(char) * (j + 1));
                    if (words[k] != NULL && word != NULL) {
                        strncpy(words[k], word, j);
                        words[k][j] = '\0';
                    } else {
                        for (int l = 0; l < k; l++) {
                            free(words[l]);
                            free(words);
                            free(word);
                            return NULL;
                        }
                    }
                } else {
                    words[k] = (char*)malloc(1);
                    if (words[k] != NULL) {
                        words[k][0] = '\0';
                    } else {
                        for (int l = 0; l < k; l++) {
                            free(words[l]);
                            free(words);
                            free(word);
                            return NULL;
                        }
                    }
                }
                j = 0;
                k++;
                continue;
            }
            if (line[i] == '"') {
                quotes = !quotes;
                continue;
            }
            word[j] = line[i];
            j++;
        }
    }
    free(word);
    return words;
}

void freeWords(char** words, int numOfColumns)
{
    for (int i = 0; i < numOfColumns; i++) {
        free(words[i]);
    }
    free(words);
}

void maxWord(char** words, int* maxs, int numOfColumns)
{
    if (words == NULL) {
        return;
    }
    for (int i = 0; i < numOfColumns; i++) {
        if (words[i] == NULL) {
            continue;
        }
        int l = (int)strlen(words[i]);
        if (l > maxs[i]) {
            maxs[i] = l;
        }
    }
}

bool isNumber(char* word)
{
    char* endptr;
    strtod(word, &endptr);
    return *endptr == '\0';
}

int addToTable(char**** table, int** maxs, FILE* file, int* numOfColumns)
{
    char* line = NULL;
    int l = myGetline(&line, file);
    *numOfColumns = countWords(line, l);
    *maxs = (int*)calloc(*numOfColumns, sizeof(int));
    if (*maxs == NULL) {
        free(line);
        line = NULL;
        return -1;
    }
    int numOfLines = 128;
    *table = (char***)malloc(sizeof(char**) * numOfLines);
    if (table == NULL) {
        free(line);
        free(*maxs);
        line = NULL;
        *maxs = NULL;
        return -1;
    }
    int numOfThisLine = 0;
    while (l != -1) {
        (*table)[numOfThisLine] = getWords(line, l, *numOfColumns);
        if ((*table)[numOfThisLine] != NULL) {
            maxWord((*table)[numOfThisLine], *maxs, *numOfColumns);
        } else {
            free(line);
            free(*maxs);
            line = NULL;
            *maxs = NULL;
            return -1;
        }
        numOfThisLine += 1;
        if (numOfThisLine == numOfLines - 1) {
            numOfLines *= 2;
            *table = (char***)realloc(*table, sizeof(char**) * numOfLines);
            if (*table == NULL) {
                return -1;
            }
        }
        free(line);
        l = myGetline(&line, file);
    }
    free(line);
    return numOfThisLine;
}

void freeTable(char*** table, int numOfColumns, int numOfLines)
{
    for (int i = 0; i < numOfLines; i++) {
        freeWords(table[i], numOfColumns);
    }
    free(table);
}

int printGraphic(const int* maxs, int len, int numOfColumns, char s, FILE* file)
{
    int l = len + 3 * numOfColumns + 1;
    char* text = (char*)malloc(l + 1);
    if (text == NULL) {
        return -1;
    }
    int k = 0;
    int lp = maxs[k] + 3;
    for (int i = 1; i < l - 1; i++) {
        if (lp == i) {
            text[i] = '+';
            k++;
            lp += maxs[k] + 3;
        } else {
            text[i] = s;
        }
    }
    text[0] = '+';
    text[l - 1] = '+';
    text[l] = '\0';
    fputs(text, file);
    fputs("\n", file);
    free(text);
    return 0;
}

int printToFile(char*** table, const int* maxs, int numOfColumns, int numOfLines, FILE* file)
{
    int s = 0;
    for (int i = 0; i < numOfColumns; i++) {
        s += maxs[i];
    }
    if (s == 0) {
        return -1;
    }
    for (int i = 0; i < numOfLines; i++) {
        char* text = (char*)malloc(s + 3 * (numOfColumns + 1));
        if (text == NULL) {
            return -1;
        }
        int pos = 0;
        for (int j = 0; j < numOfColumns; j++) {
            int d = maxs[j] - (int)strlen(table[i][j]);
            if (isNumber(table[i][j])) {
                pos += sprintf(text + pos, "|");
                for (int k = 0; k < d; k++) {
                    pos += sprintf(text + pos, " ");
                }
                pos += sprintf(text + pos, " %s ", table[i][j]);
            } else {
                pos += sprintf(text + pos, "| ");
                pos += sprintf(text + pos, "%s ", table[i][j]);
                for (int k = 0; k < d; k++) {
                    pos += sprintf(text + pos, " ");
                }
            }
        }
        sprintf(text + pos, "|");
        if (i == 0) {
            printGraphic(maxs, s, numOfColumns, '=', file);
            fputs(text, file);
            fputs("\n", file);
            printGraphic(maxs, s, numOfColumns, '=', file);
        } else {
            fputs(text, file);
            fputs("\n", file);
            printGraphic(maxs, s, numOfColumns, '-', file);
        }
        free(text);
    }
    return 0;
}
