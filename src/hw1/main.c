#include "printer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    free(line);
    int s = 0;
    for (int i = 0; i < count; i++) {
        s += maxs[i];
    }
    if (s == 0) {
        free(maxs);
        return -1;
    }

    rewind(input);
    l = myGetline(&line, input);
    while (l != -1) {
        char** words = getWords(line, l, count);
        char* text = (char*)malloc(sizeof(char) * (s + 3 * count + 1));
        if (text == NULL) {
            free(maxs);
            return -1;
        }
        int pos = 0;
        for (int i = 0; i < count; i++) {
            int d = maxs[i] - (int)strlen(words[i]);
            if (isNumber(words[i])) {
                pos += sprintf(text + pos, "| ");
                for (int j = 0; j < d; j++) {
                    pos += sprintf(text + pos, " ");
                }
                pos += sprintf(text + pos, "%s ", words[i]);
            } else {
                pos += sprintf(text + pos, "| ");
                pos += sprintf(text + pos, "%s ", words[i]);
                for (int j = 0; j < d; j++) {
                    pos += sprintf(text + pos, " ");
                }
            }
        }
        sprintf(text + pos, "|");
        printf("%s", text);
        printf("\n");
        freeWords(words, count);
        free(line);
        l = myGetline(&line, input);
        free(text);
    }
    free(maxs);
    free(line);
    fclose(input);
    return 0;
}