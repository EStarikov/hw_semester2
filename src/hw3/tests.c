#include "states.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* create_test_file(const char* content)
{
    char* filename = strdup("/tmp/test_states_XXXXXX");
    int fd = mkstemp(filename);
    assert(fd != -1);

    FILE* f = fdopen(fd, "w");
    assert(f != NULL);

    fprintf(f, "%s", content);
    fclose(f);

    return filename;
}

bool check_city_in_state(int* states, int city, int expected_state)
{
    return states[city] == expected_state;
}

void test_simple_two_cities()
{
    printf("Два города, одна дорога, одна столица: ");

    const char* content = "2 1\n"
                          "0 1 10\n"
                          "1\n"
                          "0\n";

    char* filename = create_test_file(content);

    int* states = NULL;
    unsigned* capitals = NULL;
    size_t n = 0, k = 0;

    Heap** graph = readFromFile(filename, &states, &capitals, &n, &k);
    assert(graph != NULL);
    assert(n == 2);
    assert(k == 1);
    assert(capitals[0] == 0);
    assert(states[0] == 0);
    assert(states[1] == -1);

    matchCityWithCapital(graph, states, capitals, n, k);

    assert(states[0] == 0);
    assert(states[1] == 0);

    freeGraph(&graph, n);
    free(states);
    free(capitals);
    remove(filename);
    free(filename);

    printf("Тест пройден\n");
}

void test_two_states_one_city()
{
    printf("Два государства, один город между столицами: ");

    const char* content = "3 2\n"
                          "0 2 5\n"
                          "1 2 10\n"
                          "2\n"
                          "0 1\n";

    char* filename = create_test_file(content);

    int* states = NULL;
    unsigned* capitals = NULL;
    size_t n = 0, k = 0;

    Heap** graph = readFromFile(filename, &states, &capitals, &n, &k);
    assert(graph != NULL);
    assert(n == 3);
    assert(k == 2);
    assert(capitals[0] == 0);
    assert(capitals[1] == 1);
    assert(states[0] == 0);
    assert(states[1] == 1);
    assert(states[2] == -1);

    matchCityWithCapital(graph, states, capitals, n, k);

    assert(states[2] == 0);

    freeGraph(&graph, n);
    free(states);
    free(capitals);
    remove(filename);
    free(filename);

    printf("Тест пройден\n");
}

void test_alternating_distribution()
{
    printf("Поочередное распределение городов: ");

    const char* content = "5 4\n"
                          "0 2 3\n"
                          "0 3 4\n"
                          "1 4 5\n"
                          "2 4 2\n"
                          "2\n"
                          "0 1\n";

    char* filename = create_test_file(content);

    int* states = NULL;
    unsigned* capitals = NULL;
    size_t n = 0, k = 0;

    Heap** graph = readFromFile(filename, &states, &capitals, &n, &k);
    assert(graph != NULL);

    matchCityWithCapital(graph, states, capitals, n, k);

    assert(check_city_in_state(states, 0, 0) == true);
    assert(check_city_in_state(states, 2, 0) == true);
    assert(check_city_in_state(states, 3, 0) == true);
    assert(check_city_in_state(states, 1, 1) == true);
    assert(check_city_in_state(states, 4, 1) == true);

    freeGraph(&graph, n);
    free(states);
    free(capitals);
    remove(filename);
    free(filename);

    printf("Тест пройден\n");
}

void test_complex_graph()
{
    printf("Сложный граф с несколькими путями: ");

    const char* content = "6 7\n"
                          "0 1 2\n"
                          "0 2 3\n"
                          "1 3 4\n"
                          "2 3 1\n"
                          "3 4 5\n"
                          "3 5 2\n"
                          "4 5 3\n"
                          "3\n"
                          "0 2 4\n";

    char* filename = create_test_file(content);

    int* states = NULL;
    unsigned* capitals = NULL;
    size_t n = 0, k = 0;

    Heap** graph = readFromFile(filename, &states, &capitals, &n, &k);
    assert(graph != NULL);

    matchCityWithCapital(graph, states, capitals, n, k);

    assert(check_city_in_state(states, 0, 0) == true);
    assert(check_city_in_state(states, 1, 0) == true);
    assert(check_city_in_state(states, 2, 2) == true);
    assert(check_city_in_state(states, 3, 2) == true);
    assert(check_city_in_state(states, 4, 4) == true);
    assert(check_city_in_state(states, 5, 4) == true);

    freeGraph(&graph, n);
    free(states);
    free(capitals);
    remove(filename);
    free(filename);

    printf("Тест пройден\n");
}

void test_all_capitals()
{
    printf("Все города являются столицами: ");

    const char* content = "4 3\n"
                          "0 1 5\n"
                          "1 2 5\n"
                          "2 3 5\n"
                          "4\n"
                          "0 1 2 3\n";

    char* filename = create_test_file(content);

    int* states = NULL;
    unsigned* capitals = NULL;
    size_t n = 0, k = 0;

    Heap** graph = readFromFile(filename, &states, &capitals, &n, &k);
    assert(graph != NULL);
    assert(k == 4);
    assert(n == 4);

    matchCityWithCapital(graph, states, capitals, n, k);

    for (size_t i = 0; i < n; ++i) {
        assert(states[i] == i);
    }

    freeGraph(&graph, n);
    free(states);
    free(capitals);
    remove(filename);
    free(filename);

    printf("Тест пройден\n");
}

void test_complete_graph()
{
    printf("Полносвязный граф: ");

    const char* content = "5 10\n"
                          "0 1 10\n"
                          "0 2 20\n"
                          "0 3 30\n"
                          "1 2 15\n"
                          "1 3 25\n"
                          "2 3 5\n"
                          "0 4 27\n"
                          "1 4 16\n"
                          "2 4 22\n"
                          "3 4 11\n"
                          "2\n"
                          "0 1\n";

    char* filename = create_test_file(content);

    int* states = NULL;
    unsigned* capitals = NULL;
    size_t n = 0, k = 0;

    Heap** graph = readFromFile(filename, &states, &capitals, &n, &k);
    assert(graph != NULL);

    matchCityWithCapital(graph, states, capitals, n, k);

    assert(check_city_in_state(states, 0, 0) == true);
    assert(check_city_in_state(states, 2, 0) == true);
    assert(check_city_in_state(states, 4, 1) == true);
    assert(check_city_in_state(states, 1, 1) == true);
    assert(check_city_in_state(states, 3, 0) == true);

    freeGraph(&graph, n);
    free(states);
    free(capitals);
    remove(filename);
    free(filename);

    printf("Тест пройден\n");
}

int main()
{
    test_simple_two_cities();
    test_two_states_one_city();
    test_alternating_distribution();
    test_complex_graph();
    test_all_capitals();
    test_complete_graph();

    return 0;
}