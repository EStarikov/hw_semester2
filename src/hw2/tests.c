#include "registration.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_empty_tree()
{
    printf("Тест 1: Пустое дерево... ");

    AVL* tree = readFileToAVL("nonexistent.txt");
    assert(tree == NULL);

    FILE* f = fopen("empty.txt", "w");
    fclose(f);

    tree = readFileToAVL("empty.txt");
    assert(tree != NULL);

    assert(contains(tree->root, "TEST") == NULL);

    freeAVL(tree);
    remove("empty.txt");
    printf("OK\n");
}

void test_single_insert()
{
    printf("Тест 2: Вставка одного элемента... ");

    AVL* tree = malloc(sizeof(AVL));
    tree->root = NULL;
    bool err = false;

    char* code = strdup("SVO");
    char* name = strdup("Москва");
    tree->root = insertAVL(code, name, tree->root, &err);
    free(code);
    code = NULL;
    free(name);
    name = NULL;

    assert(!err);
    assert(tree->root != NULL);

    const char* result = contains(tree->root, "SVO");
    assert(result != NULL);
    assert(strcmp(result, "Москва") == 0);

    assert(contains(tree->root, "XXX") == NULL);

    freeAVL(tree);
    printf("OK\n");
}

void test_multiple_insert()
{
    printf("Тест 3: Вставка нескольких элементов... ");

    AVL* tree = malloc(sizeof(AVL));
    tree->root = NULL;
    bool err = false;

    char* codes[] = { "SVO", "LED", "KZN", "JFK", "LHR", "CDG", "FRA" };
    char* names[] = { "Москва", "Санкт-Петербург", "Казань", "Нью-Йорк",
        "Лондон", "Париж", "Франкфурт" };
    int n = 7;

    for (int i = 0; i < n; i++) {
        char* code = strdup(codes[i]);
        char* name = strdup(names[i]);
        tree->root = insertAVL(code, name, tree->root, &err);
        assert(!err);
        free(code);
        code = NULL;
        free(name);
        name = NULL;
    }

    for (int i = 0; i < n; i++) {
        const char* result = contains(tree->root, codes[i]);
        assert(result != NULL);
        assert(strcmp(result, names[i]) == 0);
    }

    assert(contains(tree->root, "XXX") == NULL);

    freeAVL(tree);
    printf("OK\n");
}

void test_delete()
{
    printf("Тест 4: Удаление элемента... ");

    AVL* tree = malloc(sizeof(AVL));
    tree->root = NULL;
    bool err = false;

    char* codes[] = { "SVO", "LED", "KZN" };
    char* names[] = { "Москва", "Санкт-Петербург", "Казань" };

    for (int i = 0; i < 3; i++) {
        char* code = strdup(codes[i]);
        char* name = strdup(names[i]);
        tree->root = insertAVL(code, name, tree->root, &err);
        assert(!err);
        free(code);
        code = NULL;
        free(name);
        name = NULL;
    }

    assert(contains(tree->root, "SVO") != NULL);
    assert(contains(tree->root, "LED") != NULL);
    assert(contains(tree->root, "KZN") != NULL);

    tree->root = deleteAVL(tree->root, "KZN", &err);
    assert(!err);

    assert(contains(tree->root, "KZN") == NULL);
    assert(contains(tree->root, "SVO") != NULL);
    assert(contains(tree->root, "LED") != NULL);

    tree->root = deleteAVL(tree->root, "SVO", &err);
    assert(!err);
    assert(contains(tree->root, "SVO") == NULL);
    assert(contains(tree->root, "LED") != NULL);

    freeAVL(tree);
    printf("OK\n");
}

void test_insert_duplicate()
{
    printf("Тест 5: Вставка дубликата... ");

    AVL* tree = malloc(sizeof(AVL));
    tree->root = NULL;
    bool err = false;

    char* code1 = strdup("SVO");
    char* name1 = strdup("Москва");
    tree->root = insertAVL(code1, name1, tree->root, &err);
    assert(!err);
    free(code1);
    code1 = NULL;
    free(name1);
    name1 = NULL;

    char* code2 = strdup("SVO");
    char* name2 = strdup("Шереметьево");
    tree->root = insertAVL(code2, name2, tree->root, &err);
    assert(!err);
    free(code2);
    code2 = NULL;
    free(name2);
    name2 = NULL;

    const char* result = contains(tree->root, "SVO");
    assert(strcmp(result, "Москва") == 0);

    freeAVL(tree);
    printf("OK\n");
}

void test_delete_nonexistent()
{
    printf("Тест 6: Удаление несуществующего элемента... ");

    AVL* tree = malloc(sizeof(AVL));
    assert(tree != NULL);
    tree->root = NULL;
    bool err = false;

    char* code = strdup("SVO");
    char* name = strdup("Москва");
    tree->root = insertAVL(code, name, tree->root, &err);
    assert(!err);
    free(code);
    code = NULL;
    free(name);
    name = NULL;

    tree->root = deleteAVL(tree->root, "XXX", &err);
    assert(!err);

    assert(contains(tree->root, "SVO") != NULL);

    freeAVL(tree);
    printf("OK\n");
}

void test_delete_empty()
{
    printf("Тест 7: Удаление из пустого дерева... ");

    AVL* tree = malloc(sizeof(AVL));
    tree->root = NULL;
    bool err = false;

    tree->root = deleteAVL(tree->root, "XXX", &err);
    assert(!err);
    assert(tree->root == NULL);

    freeAVL(tree);
    printf("OK\n");
}

void test_save_and_load()
{
    printf("Тест 8: Сохранение в файл и чтение... ");

    AVL* tree1 = malloc(sizeof(AVL));
    tree1->root = NULL;
    bool err = false;

    char* codes[] = { "SVO", "LED", "KZN" };
    char* names[] = { "Москва", "Санкт-Петербург", "Казань" };
    int n = 3;

    for (int i = 0; i < n; i++) {
        char* code = strdup(codes[i]);
        char* name = strdup(names[i]);
        tree1->root = insertAVL(code, name, tree1->root, &err);
        assert(!err);
        free(code);
        code = NULL;
        free(name);
        name = NULL;
    }

    int result = saveAVLtoFILE("test_airports.txt", tree1);
    assert(result == n);

    AVL* tree2 = readFileToAVL("test_airports.txt");
    assert(tree2 != NULL);

    for (int i = 0; i < n; i++) {
        const char* name = contains(tree2->root, codes[i]);
        assert(name != NULL);
        assert(strcmp(name, names[i]) == 0);
    }

    freeAVL(tree1);
    freeAVL(tree2);
    remove("test_airports.txt");
    printf("OK\n");
}

void test_split_function()
{
    printf("Тест 9: Функция split... ");

    char s1[] = "SVO:Москва";
    char code1[100];
    char name1[100];
    split(s1, code1, name1, sizeof(s1), ':');
    assert(strcmp(code1, "SVO") == 0);
    assert(strcmp(name1, "Москва") == 0);

    char s2[] = "find SVO";
    char com[100];
    char arg[100];
    split(s2, com, arg, sizeof(s2), ' ');
    assert(strcmp(com, "find") == 0);
    assert(strcmp(arg, "SVO") == 0);

    char s3[] = "add SVO:Москва";
    char com2[100];
    char arg2[100];
    split(s3, com2, arg2, sizeof(s3), ' ');
    assert(strcmp(com2, "add") == 0);
    assert(strcmp(arg2, "SVO:Москва") == 0);

    printf("OK\n");
}

void test_read_nonexistent_file()
{
    printf("Тест 10: Чтение из несуществующего файла... ");

    AVL* tree = readFileToAVL("file_that_does_not_exist_12345.txt");
    assert(tree == NULL);

    printf("OK\n");
}

void test_save_empty_tree()
{
    printf("Тест 11: Сохранение пустого дерева... ");

    AVL* tree = malloc(sizeof(AVL));
    tree->root = NULL;

    int result = saveAVLtoFILE("test_empty.txt", tree);
    assert(result == 0);

    AVL* tree2 = readFileToAVL("test_empty.txt");
    assert(tree2 != NULL);

    assert(contains(tree2->root, "ANY") == NULL);

    freeAVL(tree);
    freeAVL(tree2);
    remove("test_empty.txt");
    printf("OK\n");
}

void test_many_operations()
{
    printf("Тест 12: Большое количество операций... ");

    AVL* tree = malloc(sizeof(AVL));
    tree->root = NULL;
    bool err = false;

    int n = 100;
    char codes[100][20];

    for (int i = 0; i < n; i++) {
        sprintf(codes[i], "CODE%03d", i);
        char* code = strdup(codes[i]);
        char* name = strdup("Test");
        tree->root = insertAVL(code, name, tree->root, &err);
        assert(!err);
        free(code);
        code = NULL;
        free(name);
        name = NULL;
    }

    for (int i = 0; i < n; i++) {
        const char* result = contains(tree->root, codes[i]);
        assert(result != NULL);
    }

    for (int i = 0; i < n; i += 2) {
        tree->root = deleteAVL(tree->root, codes[i], &err);
        assert(!err);
    }

    for (int i = 0; i < n; i += 2) {
        assert(contains(tree->root, codes[i]) == NULL);
    }

    for (int i = 1; i < n; i += 2) {
        assert(contains(tree->root, codes[i]) != NULL);
    }

    freeAVL(tree);
    printf("OK\n");
}

int main()
{
    printf("\n=== Тестирование AVL-дерева ===\n\n");

    test_empty_tree();
    test_single_insert();
    test_multiple_insert();
    test_delete();
    test_insert_duplicate();
    test_delete_nonexistent();
    test_delete_empty();
    test_save_and_load();
    test_split_function();
    test_read_nonexistent_file();
    test_save_empty_tree();
    test_many_operations();

    printf("\n=== Все тесты пройдены успешно! ===\n");
    return 0;
}
