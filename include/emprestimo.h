#ifndef EMPRESTIMO_H
#define EMPRESTIMO_H

#include "usuario.h"
#include "catalogo.h"
#include <time.h>

typedef struct Emprestimo {
    unsigned int user_id;
    char isbn[14];
    int prazo;
    time_t data_emprestimo;
    time_t data_devolucao;
    struct Emprestimo *prox;
} Emprestimo;

Emprestimo *criar_emprestimo(void);

int emprestar_livro(Emprestimo *cab, UserConj *usuarios, Livro *book, unsigned int id, const char *isbn);

int devolver_livro(Emprestimo *cab, UserConj *usuarios, Livro *book, unsigned int id);

int aumentar_prazo(Emprestimo *cab, unsigned int id,int dias);

int aplicar_penalidade(Emprestimo *usuario, int dias_atraso);

void remover_emprestimo(Emprestimo *cab);

/**
 * @brief Imprime os empréstimos salvos do programa.
 * @param emprestimos Ponteiro para o empréstimo sentinela, 
 * emprestimos->prox é o primeiro empréstimo
 * @pre O ponteiro emprestimos não é NULL.
 */
void imprime_emprestimos(Emprestimo* emprestimos);

/**
 * @brief Libera toda a memória dos empréstimos.
 * @param emprestimos Ponteiro para o empréstimo sentinela,
 * emprestimos->prox é o primeiro empréstimo
 * @pre Todos os nós foram alocados dinamicamente.
 * @post Todos os nós da árvore são liberados. O ponteiro emprestimos fica inválido, 
 * então é recomendado atribuir NULL a ele depois da função ser chamada.
 */
void remove_emprestimos(Emprestimo *emprestimos);

#endif