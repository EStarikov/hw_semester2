#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef struct Dictionary Dictionary;

// разделитель строк
void split(char* s, char* code, char* name, size_t len, char separator);

// считывает файл в словарь
Dictionary* readFileToDictionary(char* filename);

// удаляет элемент из словаря
Dictionary* delete(Dictionary* dictionary, char* code, bool* err);

// добавляет элемент в словарь
Dictionary* insert(char* code, char* name, Dictionary* dictionary, bool* err);

// проверяет наличие элемента в словаре
const char* contains(Dictionary* dictionary, char* code);

// освобождает словарь
void freeDictionary(Dictionary* dictionary);

// сохраняет словарь в файл
int saveDictionarytoFile(char* filename, Dictionary* dictionary);
