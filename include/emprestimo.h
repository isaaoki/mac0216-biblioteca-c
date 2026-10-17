#ifndef EMPRESTIMO_H
#define EMPRESTIMO_H

#include "usuario.h"
#include "catalogo.h"
#include <time.h>
/** 
 * @brief Dados de um emprestimo, e o próximo nó para quem está apontando na lista encadeada
*/

typedef struct Emprestimo {
    unsigned int user_id;
    char isbn[14];
    int prazo;
    time_t data_emprestimo;
    time_t data_devolucao;
    struct Emprestimo *prox;
} Emprestimo;


/**
 * @brief Cria a sentinela da lista de emprestimos que não guarda nenhuma informação, mas aponta para o início da lista
 * @post Nó sentinela da lista encadeada aponta para NULL
 * @returns nó sentinela da lista encadeada de empréstimos
 * @note Se não for possivel alocar memória para a sentinela, devolve uma mensagem de erro 
 */
Emprestimo *cria_lista_emprestimo(void);

/**
 * @brief Realiza o empréstimo de um livro para um usuário, verificando se ele pode emprestar e se há cópias disponíveis.
 * @param cab Ponteiro para o nó sentinela da lista de empréstimos.
 * @param usuarios Ponteiro para o conjunto de usuários.
 * @param book Ponteiro para o livro.
 * @param id id do usuario.
 * @param isbn ISBN do livro.
 * @returns 1 se operação realizada com sucesso, 0 se não.
 * @pre Os ponteiros cab, usuarios, book e isbn devem ser válidos, e book deve corresponder ao ISBN informado.
 * @post (Se não houver erros) O empréstimo será inserido na lista de empréstimos, o número de cópias disponíveis
 *  do livro é diminuido e o status do usuário será atualizado para USER_BLOCKED.
 */
int empresta_livro(Emprestimo *cab, UserConj *usuarios, Livro *book, unsigned int id, const char *isbn);

/**
 * @brief Faz devolução de um livro emprestado, calcula a multa por atraso, se houver, e atualiza o status do usuário.
 * @param cab Ponteiro para o nó sentinela da lista de empréstimos.
 * @param usuarios Ponteiro para o conjunto de usuários.
 * @param book Ponteiro para o livro.
 * @param id id do usuário.
 * @param multa Ponteiro para a variável que receberá o valor da multa.
 * @returns 1 se operação realizada com sucesso, 0 se não.
 * @pre Os ponteiros cab, usuarios e book devem ser válidos, e deve existir um empréstimo que corresponda ao id 
 * do usuário e ao ISBN do livro.
 * @post Se a devolução for realizada com sucesso, o número de cópias disponíveis do livro será incrementado, 
 * o status do usuário será atualizado para USER_OK e o empréstimo será removido da lista. Se multa não for NULL, 
 * receberá o valor da multa calculada. 
 */
int devolve_livro(Emprestimo *cab, UserConj *usuarios, Livro *book, unsigned int id, int *multa);

/**
 * @brief Aumenta o prazo de devolução de um empréstimo.
 * @param cab Ponteiro para o nó sentinela da lista de empréstimos.
 * @param id id do usuário.
 * @param dias Quantidade de dias a serem adicionados ao prazo de devolução.
 * @returns 1 se operação realizada com sucesso, 0 se não.
 * @pre O ponteiro cab deve ser válido e dias deve ser maior ou igual a zero.
 * @post Se a operação for realizada com sucesso, o prazo do empréstimo e a data de devolução serão atualizados
 *  com os dias adicionados.
 */
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