#include "printer.h"
#include <stdio.h>
#include <string.h>

void testCountWords()
{
    char lineWithoutQuotes[] = "fsfs, fsafds, fds";
    char lineWithQuotes[] = "fsfsd\"fdfs,dsf\"fd, fds";
    char lineWithEmptyWord[] = "fsd,,fs";
    if (3 == countWords(lineWithoutQuotes, (int)strlen(lineWithoutQuotes))) {
        printf("Simple line: passed\n");
    } else {
        printf("Simple line: not passed\n");
    }
    if (2 == countWords(lineWithQuotes, (int)strlen(lineWithQuotes))) {
        printf("Line with quotes: passed\n");
    } else {
        printf("Line with quotes: not passed\n");
    }
    if (3 == countWords(lineWithEmptyWord, (int)strlen(lineWithEmptyWord))) {
        printf("Line with empty word: passed\n");
    } else {
        printf("Line with empty word: not passed\n");
    }
}

void testGetWorld()
{
    char simpleLine[] = "fsfs,fsf,df";
    char lineWithQuotes[] = "\"fsf, s\",dfs";
    char lineWithEmptyWord[] = "fds,,fsfs";
    char** wordsSimpleLine = getWords(simpleLine, (int)strlen(simpleLine) + 1, 3);
    char** wordsLineWithQuotes = getWords(lineWithQuotes, (int)(strlen(lineWithQuotes)) + 1, 2);
    char** wordsLineWithEmptyWord = getWords(lineWithEmptyWord, (int)strlen(lineWithEmptyWord) + 1, 3);
    if (strcmp(wordsSimpleLine[0], "fsfs") == 0 && strcmp(wordsSimpleLine[1], "fsf") == 0 && strcmp(wordsSimpleLine[2], "df") == 0) {
        printf("Simple line: passed\n");
    } else {
        printf("Simple line: not passed\n");
    }
    if (strcmp(wordsLineWithQuotes[0], "fsf, s") == 0 && strcmp(wordsLineWithQuotes[1], "dfs") == 0) {
        printf("Line with quotes: passed\n");
    } else {
        printf("Line with quotes: not passed\n");
    }
    if (strcmp(wordsLineWithEmptyWord[0], "fds") == 0 && strcmp(wordsLineWithEmptyWord[1], "") == 0 && strcmp(wordsLineWithEmptyWord[2], "fsfs") == 0) {
        printf("Line with empty word: passed\n");
    } else {
        printf("Line with empty word: not passed\n");
    }
    freeWords(wordsSimpleLine, 3);
    freeWords(wordsLineWithQuotes, 2);
    freeWords(wordsLineWithEmptyWord, 3);
}

void testMax()
{
    int maxs[3] = { 0, 0, 0 };
    char* table[3][3] = {
        { "rrrr", "2134", "" },
        { "er", "eds", "longgggg" },
        { "423", "fs", "fwff" }
    };
    for (int i = 0; i < 3; i++) {
        maxWord(table[i], maxs, 3);
    }
    if (maxs[0] == 4 && maxs[1] == 4 && maxs[2] == 8) {
        printf("Find length of max word: passed\n");
    } else {
        printf("Find length of max word: not passed\n");
    }
}

int main()
{
    printf("Test countWords\n");
    testCountWords();
    printf("Test getWorld\n");
    testGetWorld();
    printf("Test maxWord\n");
    testMax();
    return 0;
}