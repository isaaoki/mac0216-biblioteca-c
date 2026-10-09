//ideia de funcoes e interface
/*
Set* meu_set = set_create(INT); //criar um set do tipo int (a biblioteca vai suportar (int, float, double, char, char[]) decidir sobre)
set_insert(meu_set, x); 
set_remove(meu_set, x);
set_contains(meu_set, x);
set_size(meu_set);
set_is_empty(meu_set);
SetIterator it = set_begin(meu_set); //retorna o ponteiro que aponta pro comeco do set
SetIterator it = set_end(meu_set); //retorna o ponteiro que aponta para o proximo do ultimo
*/
#include "../include/set.h"
#include <stdlib.h>

#define CAPACIDADE_MIN 16

typedef struct Node{
    int elemento;
    struct Node* prox;
}Node;

struct Set{
    Node** vetor;
    size_t capacidade; 
    size_t tam;
    Tipo tipo;
};
//estou usando o (usigned int) para gerar o hash dos elementos que sera um inteiro positivo
static unsigned int hash_int(const int k){ //retorna o hash do elemento do set
    return (unsigned int)k;
}

static int cmp_int(const int a, const int b){ //compara dois elementos do set
    return (a > b) - (a < b); // retorna 1 se a>b, -1 se a<b e 0 se iguais
}

static void set_resize(Set* set){
    size_t nova_capacidade = set-> capacidade*2;
    Node** novo_vetor= (Node**)calloc(nova_capacidade, sizeof(Node*));
    if (!novo_vetor) return; //caso falhe a alocacao
    for (size_t i=0;i<set->capacidade;i++){
        Node* atual=set->vetor[i];
        while(atual){
            Node* prox=atual->prox;
            unsigned int idx= hash_int(atual->elemento)%nova_capacidade;
            atual->prox = novo_vetor[idx]; 
            novo_vetor[idx]=atual;
            atual=prox;
        }
    }
    free(set->vetor);
    set->vetor=novo_vetor;
    set->capacidade=nova_capacidade;
}

//INTERFACE PUBLICA
Set* set_create(Tipo t){ 
    Set* set=(Set*)malloc(sizeof(Set));
    if (!set) return NULL;//caso alocacao da errado

    set->capacidade=CAPACIDADE_MIN;
    set->tam=0;
    set->tipo=t;
    set->vetor=(Node**)calloc(set->capacidade,sizeof(Node*));
    if (!set->vetor){
        free(set);
        return NULL;
    }
    return set;
}

int set_countains(const Set* set, int x){
    if (!set ) return 0; //se o set for NULL retorna 0
    unsigned int idx=hash_int(x) % set->capacidade;
    Node* atual=set->vetor[idx];
    while(atual){
        if (cmp_int(atual->elemento, x)==0)return 1;//se os elementos forem iguais, nao precisa adicionar
        atual=atual->prox;
    }
    return 0;
}

int set_insert(Set* set, int x){
    if (!set  || set_countains(set,x)) return 0; //se o set for NULL ou se o valor ja existir no set retorna 0
    // aumenta a capacidade ao ultrapassar 75% da capacidade atual
    //evitar q vire uma busca dentro do vetor
    if (set->tam >=set->capacidade * 0.75){
        set_resize(set);
    }
    unsigned int idx=hash_int(x)%set->capacidade; //indice eh o hash do elemento
    Node* novo_no = (Node*) malloc(sizeof(Node));
    if (!novo_no) return 0;//caso alocacao falhe
    novo_no -> elemento=x;
    novo_no->prox = set->vetor[idx];
    set->vetor[idx] = novo_no;
    set->tam++;
    
    return 1; //retorna 1 se tudo der certo
} 
int set_remove(Set* set, int x){
    return 0;
}
size_t set_size(Set* set){
    return 0;
}
int set_empty(Set* set){
    return 0;
}