#include "printer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Table {
    int* maxs;
    char*** tableOfWords;
    int numOfColumns;
    int numOfLines;
};

Table* createTable()
{
    return (Table*)calloc(1, sizeof(Table));
}
static int myGetline(char** line, FILE* file)
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
            char* tmp = (char*)realloc(*line, sizeof(char) * n);
            if (tmp == NULL) {
                free(*line);
                return -1;
            }
            *line = tmp;
        }
    }
    (*line)[i] = '\0';
    if (c == EOF) {
        return -1;
    }
    return i + 1;
}

static int countWords(const char* line, int len)
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

static char** getWords(const char* line, int len, int numOfColumns)
{
    char** words = (char**)calloc(numOfColumns, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
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
    }
    char* word = (char*)malloc(sizeof(char) * len);
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
                        }
                        return NULL;
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
                        }
                        return NULL;
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

static void freeWords(char** words, int numOfColumns)
{
    for (int i = 0; i < numOfColumns; i++) {
        free(words[i]);
    }
    free(words);
}

static void maxWord(char** words, int* maxs, int numOfColumns)
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

static bool isNumber(char* word)
{
    char* endptr;
    strtod(word, &endptr);
    return *endptr == '\0';
}

void addToTable(Table* myTable, FILE* file)
{
    char* line = NULL;
    int l = myGetline(&line, file);
    myTable->numOfColumns = countWords(line, l);
    myTable->maxs = (int*)calloc(myTable->numOfColumns, sizeof(int));
    if (myTable->maxs == NULL) {
        free(line);
        myTable->numOfLines = -1;
        return;
    }
    int numOfLines = 128;
    myTable->tableOfWords = (char***)malloc(sizeof(char**) * numOfLines);
    if (myTable->tableOfWords == NULL) {
        free(line);
        free(myTable->maxs);
        myTable->maxs = NULL;
        myTable->numOfLines = -1;
        return;
    }
    int numOfThisLine = 0;
    while (l != -1) {
        (myTable->tableOfWords)[numOfThisLine] = getWords(line, l, myTable->numOfColumns);
        if ((myTable->tableOfWords)[numOfThisLine] != NULL) {
            maxWord((myTable->tableOfWords)[numOfThisLine], myTable->maxs, myTable->numOfColumns);
        } else {
            free(line);
            free(myTable->maxs);
            myTable->maxs = NULL;
            myTable->numOfLines = -1;
            return;
        }
        numOfThisLine += 1;
        if (numOfThisLine == numOfLines - 1) {
            numOfLines *= 2;
            char*** tmp = (char***)realloc(myTable->tableOfWords, sizeof(char**) * numOfLines);
            if (tmp == NULL) {
                myTable->numOfLines = -1;
                free(myTable->tableOfWords);
                return;
            }
            myTable->tableOfWords = tmp;
        }
        free(line);
        l = myGetline(&line, file);
    }
    free(line);
    myTable->numOfLines = numOfThisLine;
    return;
}

void freeTable(Table** myTable)
{
    free((*myTable)->maxs);
    for (int i = 0; i < (*myTable)->numOfLines; i++) {
        freeWords((*myTable)->tableOfWords[i], (*myTable)->numOfColumns);
    }
    free((*myTable)->tableOfWords);
    free(*myTable);
}

static int printGraphic(const int* maxs, int len, int numOfColumns, char s, FILE* file)
{
    int lengthOfOutputLine = len + 3 * numOfColumns + 1;
    char* text = (char*)malloc(lengthOfOutputLine + 1);
    if (text == NULL) {
        return -1;
    }
    int currentWord = 0;
    int lenOfCurrentWord = maxs[currentWord] + 3;
    for (int i = 1; i < lengthOfOutputLine - 1; i++) {
        if (lenOfCurrentWord == i) {
            text[i] = '+';
            currentWord++;
            lenOfCurrentWord += maxs[currentWord] + 3;
        } else {
            text[i] = s;
        }
    }
    text[0] = '+';
    text[lengthOfOutputLine - 1] = '+';
    text[lengthOfOutputLine] = '\0';
    fputs(text, file);
    fputs("\n", file);
    free(text);
    return 0;
}

int printToFile(Table* myTable, FILE* file)
{
    int s = 0;
    for (int i = 0; i < myTable->numOfColumns; i++) {
        s += myTable->maxs[i];
    }
    if (s == 0) {
        return -1;
    }
    for (int i = 0; i < myTable->numOfLines; i++) {
        char* text = (char*)malloc(s + 3 * (myTable->numOfColumns + 1));
        if (text == NULL) {
            return -1;
        }
        int pos = 0;
        for (int j = 0; j < myTable->numOfColumns; j++) {
            int d = myTable->maxs[j] - (int)strlen(myTable->tableOfWords[i][j]);
            if (isNumber(myTable->tableOfWords[i][j])) {
                pos += sprintf(text + pos, "|");
                for (int k = 0; k < d; k++) {
                    pos += sprintf(text + pos, " ");
                }
                pos += sprintf(text + pos, " %s ", myTable->tableOfWords[i][j]);
            } else {
                pos += sprintf(text + pos, "| ");
                pos += sprintf(text + pos, "%s ", myTable->tableOfWords[i][j]);
                for (int k = 0; k < d; k++) {
                    pos += sprintf(text + pos, " ");
                }
            }
        }
        sprintf(text + pos, "|");
        if (i == 0) {
            printGraphic(myTable->maxs, s, myTable->numOfColumns, '=', file);
            fputs(text, file);
            fputs("\n", file);
            printGraphic(myTable->maxs, s, myTable->numOfColumns, '=', file);
        } else {
            fputs(text, file);
            fputs("\n", file);
            printGraphic(myTable->maxs, s, myTable->numOfColumns, '-', file);
        }
        free(text);
    }
    return 0;
}
