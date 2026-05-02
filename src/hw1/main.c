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
    Table* table = createTable();
    if (table == NULL) {
        fclose(input);
        fclose(output);
        return 1;
    }
    addToTable(table, input);
    printToFile(table, output);
    freeTable(&table);
    fclose(input);
    fclose(output);
    return 0;
}