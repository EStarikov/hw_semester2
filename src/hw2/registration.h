#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef struct Node Node;
typedef struct AVL {
    Node* root;
} AVL;

void split(char* s, char* code, char* name, size_t len, char separator);
AVL* readFileToAVL(char* filename);
Node* deleteAVL(Node* node, char* code, bool* err);
Node* insertAVL(char* code, char* name, Node* node, bool* err);
const char* contains(Node* node, char* code);
void freeAVL(AVL* tree);
int saveAVLtoFILE(char* filename, AVL* tree);
