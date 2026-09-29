//ideia de funcoes e interface
/*
Set* meu_set = set_create(10); //criar um set de tamanho 10
set_insert(meu_set, x); 
set_remove(meu_set, x);
set_contains(meu_set, x);
set_size(meu_set);
set_is_empty(meu_set);
SetIterator it = set_begin(meu_set);
SetIterator it = set_iterator(meu_set);
*/
#include "set.h"
#include <stdlib.h>

typedef struct Node{
    void* elemento;
    struct Node* prox;
}Node;

struct Set{
    Node** vetor;
    size_t capacidade;
    size_t tam;
};


