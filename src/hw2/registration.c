#include "registration.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    char* code;
    char* name;
    int diff; // h(left) - h(right)
    Node* left;
    Node* right;
};

static int myGetline(char** line, FILE* file)
{
    size_t n = 128;
    int i = 0;
    *line = (char*)malloc(sizeof(char) * n);
    if (*line == NULL) {
        return -1;
    }
    int c = fgetc(file);
    while (c != '\n' && c != EOF) {
        (*line)[i] = (char)c;
        i++;
        c = fgetc(file);
        if (i == n - 2) {
            n *= 2;
            *line = (char*)realloc(*line, sizeof(char) * n);
            if (*line == NULL) {
                return -1;
            }
        }
    }
    (*line)[i] = '\0';
    if (c == EOF) {
        return -1;
    }
    return i + 1;
}

void split(char* s, char* code, char* name, size_t len, char separator)
{
    for (size_t i = 0; i < len; i++) {
        if (s[i] == separator) {
            strncpy(code, s, i);
            code[i] = '\0';
            strncpy(name, s + i + 1, len - i - 1);
            name[len - i - 1] = '\0';
            return;
        }
    }
}

static AVL* initAVL()
{
    AVL* tree = malloc(sizeof(AVL));
    if (tree != NULL) {
        tree->root = NULL;
    }
    return tree;
}

static void cleanTree(Node* node)
{
    if (node == NULL) {
        return;
    }

    cleanTree(node->left);
    cleanTree(node->right);
    free(node->code);
    free(node->name);
    free(node);
}
void freeAVL(AVL* tree)
{
    cleanTree(tree->root);
    free(tree);
}

static Node* rotateLeft(Node* a)
{
    Node* b = a->right;
    a->right = b->left;
    b->left = a;
    if (b->diff == -1) {
        a->diff = 0;
        b->diff = 0;
    } else {
        a->diff = -1;
        b->diff = 1;
    }
    return b;
}

static Node* rotateRight(Node* a)
{
    Node* b = a->left;
    a->left = b->right;
    b->right = a;
    if (b->diff == 1) {
        a->diff = 0;
        b->diff = 0;
    } else {
        a->diff = 1;
        b->diff = -1;
    }
    return b;
}

static Node* bigRotateLeft(Node* a)
{
    Node* b = a->right;
    Node* c = b->left;
    a->right = c->left;
    b->left = c->right;
    c->left = a;
    c->right = b;
    if (b->diff == 1 && c->diff == 1) {
        a->diff = 0;
        b->diff = -1;
        c->diff = 0;
    } else if (b->diff == 1 && c->diff == -1) {
        a->diff = 1;
        b->diff = 0;
        c->diff = 0;
    } else {
        a->diff = 0;
        b->diff = 0;
        c->diff = 0;
    }
    return c;
}

static Node* bigRotateRight(Node* a)
{
    Node* b = a->left;
    Node* c = b->right;
    a->left = c->right;
    b->right = c->left;
    c->left = b;
    c->right = a;
    if (b->diff == -1 && c->diff == 1) {
        a->diff = -1;
        b->diff = 0;
        c->diff = 0;
    } else if (b->diff == -1 && c->diff == -1) {
        a->diff = 0;
        b->diff = 1;
        c->diff = 0;
    } else {
        a->diff = 0;
        b->diff = 0;
        c->diff = 0;
    }
    return c;
}

static Node* balance(Node* node)
{
    if (node == NULL) {
        return node;
    }
    if (node->diff == -2) {
        if (node->right == NULL) {
            return node;
        }
        if (node->right->diff <= 0) {
            return rotateLeft(node);
        } else {
            return bigRotateLeft(node);
        }
    } else if (node->diff == 2) {
        if (node->left == NULL) {
            return node;
        }
        if (node->left->diff >= 0) {
            return rotateRight(node);
        } else {
            return bigRotateRight(node);
        }
    }
    return node;
}

static Node* minimum(Node* node)
{
    if (node->left == NULL) {
        return node;
    }
    return minimum(node->left);
}
Node* deleteAVL(Node* node, char* code, bool* err)
{
    if (node == NULL) {
        return node;
    }

    int cmp = strcmp(code, node->code);

    if (cmp < 0) {
        int oldLeftDiff = node->left->diff;
        Node* newLeft = deleteAVL(node->left, code, err);
        if (*err) {
            return node;
        }
        if (node->left != NULL && (newLeft == NULL || oldLeftDiff != newLeft->diff)) {
            node->diff--;
        }
        node->left = newLeft;
    } else if (cmp > 0) {
        int oldRightDiff = node->right->diff;
        Node* newRight = deleteAVL(node->right, code, err);
        if (*err) {
            return node;
        }
        if (node->right != NULL && (newRight == NULL || oldRightDiff != newRight->diff)) {
            node->diff++;
        }
        node->right = newRight;
    } else if (node->left != NULL && node->right != NULL) {
        Node* min = minimum(node->right);
        free(node->code);
        node->code = NULL;
        node->code = malloc(strlen(min->code) + 1);
        if (node->code == NULL) {
            *err = true;
            return node;
        }
        strcpy(node->code, min->code);
        free(node->name);
        node->name = NULL;
        node->name = malloc(strlen(min->name) + 1);
        if (node->name == NULL) {
            *err = true;
            return node;
        }
        strcpy(node->name, min->name);
        int oldRightDiff = node->right->diff;
        Node* newRight = deleteAVL(node->right, min->code, err);
        if (*err) {
            return node;
        }
        if (node->right != NULL && (newRight == NULL || oldRightDiff != newRight->diff)) {
            node->diff++;
        }
        node->right = newRight;
    } else {
        if (node->left != NULL) {
            Node* newNode = node->left;
            free(node->code);
            free(node->name);
            free(node);
            node = NULL;
            return newNode;
        } else if (node->right != NULL) {
            Node* newNode = node->right;
            free(node->code);
            free(node->name);
            free(node);
            node = NULL;
            return newNode;
        } else {
            free(node->code);
            free(node->name);
            free(node);
            node = NULL;
            return NULL;
        }
    }
    return balance(node);
}

Node* insertAVL(char* code, char* name, Node* node, bool* err)
{
    if (node == NULL) {
        Node* newNode = calloc(1, sizeof(Node));
        if (newNode == NULL) {
            *err = true;
            return node;
        }
        newNode->code = code;
        newNode->name = name;
        newNode->diff = 0;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    int cmp = strcmp(code, node->code);
    if (cmp < 0) {
        Node* newLeft = insertAVL(code, name, node->left, err);
        if (*err) {
            return node;
        }
        if (node->left == NULL || node->left->diff != newLeft->diff) {
            node->diff++;
        }
        node->left = newLeft;
    } else if (cmp > 0) {
        Node* newRight = insertAVL(code, name, node->right, err);
        if (*err) {
            return node;
        }
        if (node->right == NULL || node->right->diff != newRight->diff) {
            node->diff--;
        }
        node->right = newRight;
    } else {
        return node;
    }

    return balance(node);
}

AVL* readFileToAVL(char* filename)
{
    FILE* input = fopen(filename, "r");
    if (input == NULL) {
        return NULL;
    }
    AVL* tree = initAVL();
    if (tree == NULL) {
        fclose(input);
        return NULL;
    }
    char* line = NULL;
    int l = myGetline(&line, input);
    while (l != -1) {
        char* code = malloc(l + 1);
        if (code == NULL) {
            fclose(input);
            freeAVL(tree);
            free(line);
            return NULL;
        }
        char* name = malloc(l + 1);
        if (name == NULL) {
            fclose(input);
            freeAVL(tree);
            free(code);
            free(line);
            return NULL;
        }
        split(line, code, name, l, ':');
        bool err = false;
        tree->root = insertAVL(code, name, tree->root, &err);
        if (err) {
            fclose(input);
            freeAVL(tree);
            free(code);
            free(name);
            free(line);
            return NULL;
        }
        code = NULL;
        name = NULL;
        free(line);
        line = NULL;
        l = myGetline(&line, input);
    }
    free(line);
    fclose(input);
    return tree;
}

const char* contains(Node* node, char* code)
{
    if (node == NULL) {
        return NULL;
    }

    int cmp = strcmp(code, node->code);
    if (cmp < 0) {
        return contains(node->left, code);
    } else if (cmp > 0) {
        return contains(node->right, code);
    } else {
        return node->name;
    }
}

static void inOrder(Node* node, FILE* output, bool* err, unsigned* n)
{
    if (node == NULL) {
        return;
    }

    inOrder(node->left, output, err, n);
    char* text = malloc(strlen(node->code) + strlen(node->name) + 4);
    if (text == NULL) {
        *err = true;
        return;
    }
    *n += 1;
    sprintf(text, "%s: %s", node->code, node->name);
    fputs(text, output);
    fputs("\n", output);
    free(text);
    inOrder(node->right, output, err, n);
}

int saveAVLtoFILE(char* filename, AVL* tree)
{
    FILE* output = fopen(filename, "w");
    if (output == NULL) {
        return -1;
    }

    bool err = false;
    unsigned n = 0;
    inOrder(tree->root, output, &err, &n);

    fclose(output);

    if (err == true) {
        return -1;
    }
    return n;
}