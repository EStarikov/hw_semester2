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
    while (l != -1) {
        int count = countWords(line, l);
        printf("%d %d\n", l, count);
        free(line);
        l = myGetline(&line, input);
    }
    free(line);
    line = NULL;
    fclose(input);
    return 0;
}