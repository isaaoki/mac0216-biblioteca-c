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

Emprestimo *cria_lista_emprestimo(void);

int empresta_livro(Emprestimo *cab, UserConj *usuarios, Livro *book, unsigned int id, const char *isbn);

int devolve_livro(Emprestimo *cab, UserConj *usuarios, Livro *book, unsigned int id, int *multa);

int aumenta_prazo(Emprestimo *cab, unsigned int id, int dias);

int calcula_multa(const Emprestimo *usuario, int dias_totais);

/**
 * @brief Imprime os empréstimos salvos do programa.
 * @param emprestimos Ponteiro para o empréstimo sentinela, emprestimos->prox é o primeiro empréstimo
 * @note Se emprestimos for NULL, nada é impresso.
 */
void imprime_emprestimos(Emprestimo* emprestimos);

/**
 * @brief Libera toda a memória da lista de empréstimos, inclusive o sentinela.
 * @param emprestimos Ponteiro para o empréstimo sentinela, emprestimos->prox é o primeiro empréstimo.
 * @pre Todos os nós foram alocados dinamicamente.
 * @post Todos os nós da lista são liberados. O ponteiro emprestimos fica inválido, 
 * então é recomendado atribuir NULL a ele depois da função ser chamada. O sentinela é liberado.
 */
void libera_lista_emprestimos(Emprestimo *cab);

/**
 * @brief Busca empréstimo pelo id do usuário e pelo isbn do livro.
 * @param emprestimos Ponteiro para o empréstimo sentinela.
 * @param id Id do usuário.
 * @param isbn Isbn do livro.
 * @returns Ponteiro para o empréstimo encontrado ou NULL se não encontrou ou se emprestimos ou isbn forem NULL.
 * @warning O ponteiro retornado pertence a lista e não deve ser liberado pelo chamador.
 */
Emprestimo *busca_emprestimo(Emprestimo *emprestimos, unsigned int id, const char *isbn);

#endif