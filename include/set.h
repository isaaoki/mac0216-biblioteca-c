#ifndef SET_H
#define SET_H
#define CAPACIDADE_MIN 16

#include <stdlib.h>

typedef enum {
    INT,
    FLOAT,
    DOUBLE,
    CHAR,
    STRING
} Tipo;

typedef struct Node{
    int elemento;
    struct Node* prox;
} Node;

typedef struct Set{
    Node** vetor;
    size_t capacidade; 
    size_t tam;
    Tipo tipo;
} Set;

Set* set_create(Tipo t);
int set_contains(const Set* set, int x);
int set_insert(Set* set, int x);
int set_remove(Set* set, int x);
size_t set_size(Set* set);
int set_empty(Set* set);

#endif