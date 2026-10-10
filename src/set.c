#include "../include/set.h"

// Usando o (usigned int) para gerar o hash dos elementos que sera um inteiro positivo
// Retorna o hash do elemento do set
static unsigned int hash_int(const int k){
    return (unsigned int)k;
}
// Compara dois elementos do set
static int cmp_int(const int a, const int b){ 
    // Retorna 1 se a > b, -1 se a < b e 0 se iguais
    return (a > b) - (a < b); 
}

// Redimensionamento do set
static void set_resize(Set* set){
    // Aloca vetor de ponteiros para os nós com o dobro da capacidade
    size_t nova_capacidade = set->capacidade * 2;
    Node** novo_vetor = (Node**)calloc(nova_capacidade, sizeof(Node*));
    
    // Caso falhe a alocacao
    if (!novo_vetor) return;

    for (size_t i=0; i < set->capacidade; i++){
        Node* atual = set->vetor[i];
        while(atual){
            Node* prox = atual->prox;
            unsigned int idx = hash_int(atual->elemento) % nova_capacidade;
            atual->prox = novo_vetor[idx]; 
            novo_vetor[idx] = atual;
            atual = prox;
        }
    }

    // Libera vetor antigo e atualiza set
    free(set->vetor);
    set->vetor = novo_vetor;
    set->capacidade = nova_capacidade;
}

// Metodos da interface publica
Set* set_create(Tipo t){ 
    Set* set = (Set*)malloc(sizeof(Set));

    // Caso alocacao de erro, retorna NULL
    if (!set) return NULL; 

    set->capacidade = CAPACIDADE_MIN;
    set->tam = 0;
    set->tipo = t;

    set->vetor = (Node**)calloc(set->capacidade,sizeof(Node*));
    if (!set->vetor){
        free(set); // Limpa o set se a alocacao do vetor falhar
        return NULL;
    }
    return set;
}

int set_contains(const Set* set, int x){
    // Se o set for NULL, retorna 0
    if (!set) return 0; 

    // Calcula hash do elemento
    unsigned int idx = hash_int(x) % set->capacidade;
    
    Node* atual = set->vetor[idx];
    while(atual) {
        // Se os elementos forem iguais, nao precisa adicionar
        if (cmp_int(atual->elemento, x) == 0) return 1;
        atual = atual->prox;
    }

    // Retorna 0 se não contem o elemento
    return 0;
}

int set_insert(Set* set, int x){
    // Se o set for NULL ou se o valor já existir no set, retorna 0
    if (!set || set_contains(set,x)) return 0; 

    // Aumenta a capacidade ao ultrapassar 75% da capacidade atual
    // Evitar que vire uma busca dentro do vetor
    if (set->tam >= set->capacidade * 0.75){
        set_resize(set);
    }

    // Calcula hash do elemento
    unsigned int idx = hash_int(x) % set->capacidade;
    
    // Aloca novo nó
    Node* novo_no = (Node*) malloc(sizeof(Node));
    if (!novo_no) return 0; // Caso alocacao falhe
    novo_no -> elemento=x;
    novo_no->prox = set->vetor[idx];
    set->vetor[idx] = novo_no;
    set->tam++;
    
    return 1; // Retorna 1 se tudo der certo
}

// Falta implementar
int set_remove(Set* set, int x){
    return 0;
}

size_t set_size(Set* set){
    return 0;
}

int set_empty(Set* set){
    return 0;
}