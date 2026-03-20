#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void prefixFunc(const char* s, int* pi, size_t n)
{
    pi[0] = 0;

    for (size_t i = 1; i < n; i++) {
        int j = pi[i - 1];
        while ((j > 0) && (s[i] != s[j])) {
            j = pi[j - 1];
        }
        if (s[i] == s[j]) {
            j++;
        }
        pi[i] = j;
    }
}

void kmpSearch(char* text, char* pattern)
{
    size_t n = strlen(text);
    size_t m = strlen(pattern);

    int* pi = (int*)malloc(m * sizeof(int));
    if (!pi) {
        return;
    }

    prefixFunc(pattern, pi, m);

    int j = 0;

    for (size_t i = 0; i < n; i++) {
        while (j > 0 && text[i] != pattern[j])
            j = pi[j - 1];

        if (text[i] == pattern[j]) {
            j++;
        }

        if (j == m) {
            printf("  Найдено в позиции %zu (индекс с 0)\n", i - m + 1);
            j = pi[j - 1];
        }
    }

    free(pi);
}