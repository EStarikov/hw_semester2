#include "registration.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main()
{
    char userCommand[100];
    char command[100];
    char code[100];
    Dictionary* dictionary = readFileToDictionary("airports.txt");
    if (dictionary == NULL) {
        return -1;
    }
    fgets(userCommand, sizeof(userCommand), stdin);
    userCommand[strcspn(userCommand, "\n")] = '\0';
    split(userCommand, command, code, sizeof(userCommand), ' ');
    while (strcmp(command, "quit") != 0) {
        if (strcmp(command, "find") == 0) {
            const char* name = contains(dictionary, code);
            if (name == NULL) {
                printf("Аэропорт с кодом '%s' не найден в базе.\n", code);
            } else {
                printf("%s → %s\n", code, name);
            }
        } else if (strcmp(command, "add") == 0) {
            char realCode[100];
            char realName[100];
            split(code, realCode, realName, sizeof(code), ':');
            bool err = false;
            dictionary = insert(realCode, realName, dictionary, &err);
            if (err == true) {
                freeDictionary(dictionary);
                return -1;
            }
            printf("Аэропорт '%s' добавлен в базу.\n", realCode);
        } else if (strcmp(command, "delete") == 0) {
            bool err = false;
            dictionary = delete (dictionary, code, &err);
            if (err == true) {
                freeDictionary(dictionary);
                return -1;
            }
            printf("Аэропорт '%s' удалён из базы.\n", code);
        } else if (strcmp(command, "save") == 0) {
            int n = saveDictionarytoFile("airports.txt", dictionary);
            printf("База сохранена: %d аэропортов.\n", n);
        }
        fgets(userCommand, sizeof(userCommand), stdin);
        userCommand[strcspn(userCommand, "\n")] = '\0';
        split(userCommand, command, code, sizeof(userCommand), ' ');
    }
    freeDictionary(dictionary);
    return 0;
}
