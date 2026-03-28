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
    FILE* output = fopen("output.txt", "w");
    if (output == NULL) {
        fclose(input);
        return -1;
    }
    int* maxs;
    char*** tableOfWords;
    int numOfColumns;
    int numOfLines = addToTable(&tableOfWords, &maxs, input, &numOfColumns);
    printToFile(tableOfWords, maxs, numOfColumns, numOfLines, output);
    free(maxs);
    freeTable(tableOfWords, numOfColumns, numOfLines);
    fclose(input);
    fclose(output);
    return 0;
}