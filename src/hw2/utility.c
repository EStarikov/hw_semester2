#include "registration.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main()
{
    char userCom[100];
    char com[100];
    char code[100];
    AVL* tree = readFileToAVL("airports.txt");
    if (tree == NULL) {
        return -1;
    }
    fgets(userCom, sizeof(userCom), stdin);
    userCom[strcspn(userCom, "\n")] = '\0';
    split(userCom, com, code, sizeof(userCom), " ");
    while (strcmp(com, "quit") != 0) {
        if (strcmp(com, "find") == 0) {
            const char* name = contains(tree->root, code);
            if (name == NULL) {
                printf("Аэропорт с кодом '%s' не найден в базе.\n", code);
            } else {
                printf("%s → %s\n", code, name);
            }
        } else if (strcmp(com, "add") == 0) {
            char realCode[100];
            char realName[100];
            split(code, realCode, realName, sizeof(code), ":");
            bool err = false;
            tree->root = insertAVL(realCode, realName, tree->root, &err);
            if (err == true) {
                freeAVL(tree);
                return -1;
            }
            printf("Аэропорт '%s' добавлен в базу.\n", realCode);
        } else if (strcmp(com, "delete") == 0) {
            bool err = false;
            tree->root = deleteAVL(tree->root, code, &err);
            if (err == true) {
                freeAVL(tree);
                return -1;
            }
            printf("Аэропорт '%s' удалён из базы.\n", code);
        } else if (strcmp(com, "save") == 0) {
            int n = saveAVLtoFILE("airports.txt", tree);
            printf("База сохранена: %d аэропортов.\n", n);
        }
        fgets(userCom, sizeof(userCom), stdin);
        userCom[strcspn(userCom, "\n")] = '\0';
        split(userCom, com, code, sizeof(userCom), " ");
    }
    freeAVL(tree);
    return 0;
}
