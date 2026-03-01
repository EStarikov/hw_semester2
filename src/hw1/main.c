#include "printer.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE* input = fopen("input.csv", "r");
    if (input == NULL) {
        return -1;
    }
    char* line = NULL;
    int l = myGetline(&line, input);
    int count = countWords(line, l);
    int* maxs = (int*)calloc(count, sizeof(int));
    while (l != -1) {
        char** words = getWords(line, l, count);
        maxWord(words, maxs, count);
        freeWords(words, count);
        free(line);
        l = myGetline(&line, input);
    }
    for (int i = 0; i < count; i++) {
        printf("%d\t", maxs[i]);
    }
    free(maxs);
    free(line);
    return 0;
}