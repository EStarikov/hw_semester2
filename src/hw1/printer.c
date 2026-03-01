#include "printer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

char** getWords(const char* line, int len, int count)
{
    char** words = (char**)malloc(sizeof(char*) * count);
    if (words == NULL) {
        return NULL;
    }
    char* word;
    if (len > 0) {
        word = (char*)malloc(sizeof(char) * len);
    } else {
        word = (char*)malloc(1);
    }
    if (word == NULL) {
        free(words);
        return NULL;
    }
    bool quotes = 0;
    int j = 0;
    int k = 0;
    for (int i = 0; i < len; i++) {
        if (k < count) {
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

void freeWords(char** words, int count)
{
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
}

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

void maxWord(char** words, int* maxLen, int count)
{
    for (int i = 0; i < count; i++) {
        int l = (int)strlen(words[i]);
        if (l > maxLen[i]) {
            maxLen[i] = l;
        }
    }
}
