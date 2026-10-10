#ifndef USUARIO_H
#define USUARIO_H

#include <stdlib.h>

/**
 * @brief Enumeração com os tipos de status que o usuário pode obter.
 */
typedef enum {
    USER_BLOCKED, /**< Usuário possui um empréstimo pendente e não pode pegar novos livros. */
    USER_OK /**< Usuário pode realizar um novo empréstimo. */
} UserStatus;

/**
 * @brief Representa um usuário.
 */
typedef struct User {
    unsigned int id; /**< Id único do usuário. */
    char nome[100]; /**< Nome do usuário. */
    UserStatus status;  /**< Status atual do usuário. */
} User;

/**
 * @brief Nó da lista encadeada para tabela hash.
 */
typedef struct UserNode {
    User* user; /**< Ponteiro para o usuário armazenado no nó. */
    struct UserNode* prox; /**< Ponteiro para o próximo nó. */
} UserNode;

/**
 * @brief Representa o conjunto de usuários, armazenando-os em uma tabela hash.
 */
typedef struct UserConj{
    UserNode** users; /**< Vetor de ponteiros para as listas encadeadas (tabela hash). */
    size_t tam; /**< Número atual de usuários cadastrados. */
    size_t capacidade; /**< Capacidade máxima atual da tabela hash. */
    unsigned int next_id; /**< Próximo ID a ser atribuído a um novo usuário. */
} UserConj;

/**
 * @brief Cria um novo conjunto de usuários com capacidade inicial especificada.
 * @param capacidade Capacidade inicial da tabela hash. Se 0, utiliza o valor padrão 16.
 * @returns Ponteiro para o conjunto criado, ou NULL em caso de falha na alocação de memória.
 * @pre capacidade >= 0.
 * @post O conjunto de usuários é alocado, com tamanho 0 e next_id inicializado como 1.
 * @note Caso ocorra falha ao alocar memória para o conjunto ou para a tabela, a função retorna NULL.
 */
UserConj* cria_conj_usuarios(size_t capacidade);

/**
 * @brief Exclui o conjunto de usuários e libera a memoria
 * @param conjunto Ponteiro para o conjunto de usuários que será excluido.
 * @returns 1 se teve sucesso, 0 caso contrario
 * @pre conjunto aponta para um UserConj válido.
 * @post Toda a memória alocada para os nós, os usuários e a tabela hash é liberada.
 * @note Se o conjunto for NULL, a função não realiza nenhuma operação.
 */
int remove_conj_usuarios(UserConj* conjunto);

/**
 * @brief Redimensiona a capacidade do conjunto de usuários, dobrando seu tamanho.
 * @param conjunto Ponteiro para o conjunto de usuários a ser redimensionado.
 * @returns 1 se teve sucesso, 0 caso contrario
 * @pre conjunto aponta para um UserConj válido.
 * @post A capacidade da tabela é duplicada e todos os usuários existentes são reindexados.
 * @note Função interno para gerenciar o conjunto.
 */
int redimensiona_conj_usuarios(UserConj* conjunto);

/**
 * @brief Registra um novo usuário no conjunto.
 * @param conjunto Ponteiro para o conjunto de usuários.
 * @param nome Nome do novo usuário.
 * @param status Status inicial do usuário (USER_OK ou USER_BLOCKED).
 * @returns O ID único gerado para o novo usuário, ou 0 em caso de erro.
 * @pre conjunto e nome devem ser válidos (diferentes de NULL).
 * @post Um novo usuário é criado com ID, inserido na tabela e o tamanho do conjunto é incrementado.
 * @note Caso o tamanho atinja 75% da capacidade, o conjunto é redimensionado automaticamente.
 */
unsigned int cria_usuario(UserConj* conjunto, const char* nome, UserStatus status);

/**
 * @brief Remove um usuário do conjunto com base no seu ID.
 * @param conjunto Ponteiro para o conjunto de usuários.
 * @param user_id Id do usuário a ser removido.
 * @returns 1 se a remoção foi bem-sucedida, 0 caso contrário ou se o usuário estiver bloqueado.
 * @pre conjunto aponta para um UserConj válido e user_id > 0.
 * @post O usuário é removido da tabela hash e sua memória é liberada, caso não possua nenhuma pendencia.
 * @note Usuários com status USER_BLOCKED não podem ser excluídos.
 */
int remove_usuario(UserConj* conjunto, unsigned int user_id);

/**
 * @brief Busca e retorna um usuário pelo seu ID.
 * @param conjunto Ponteiro para o conjunto de usuários.
 * @param user_id Id do usuário a ser buscado.
 * @returns Ponteiro para o User encontrado, ou NULL se o usuário não existir.
 * @pre conjunto aponta para um UserConj válido e user_id > 0.
 * @post O estado do conjunto permanece inalterado.
 */
User* busca_usuario(UserConj* conjunto, unsigned int user_id);

/**
 * @brief Retorna o nome de um usuário.
 * @param user Ponteiro para User.
 * @returns String contendo o nome do usuário, ou NULL se o ponteiro for inválido.
 * @pre user aponta para um User válido ou é NULL.
 */
const char* obter_nome_usuario(const User* user);

/**
 * @brief Retorna o Id de um usuário.
 * @param user Ponteiro para User.
 * @returns O Id do usuário, ou 0 em caso de ponteiro inválido.
 * @pre user aponta para um User válido ou é NULL.
 */
unsigned int obter_id_usuario(const User* user);

/**
 * @brief Retorna o status de um usuário.
 * @param user Ponteiro para User.
 * @returns O status atual do usuário, ou USER_BLOCKED se o usuário for NULL.
 * @pre user aponta para um User válido ou é NULL.
 */
UserStatus obter_status_usuario(const User* user);

/**
 * @brief Verifica se o usuário pode realizar um empréstimo.
 * @param user Ponteiro para User.
 * @returns 1 se o usuário puder pegar um livro (status USER_OK), 0 se não puder.
 * @pre user aponta para um User válido ou é NULL.
 * @note Cada usuário possui o limite de 1 empréstimo por vez.
 */
int verifica_emprestimo(const User* user);

/**
 * @brief Atualiza o status do usuario
 * @param user Ponteiro para User.
 * @param status Status a ser setado.
 * @returns 1 se teve sucesso, 0 se não teve sucesso.
 * @pre user aponta para um User válido ou é NULL.
 * @pre status é USER_OK ou USER_BLOCKED
 * @post o status do usuario é atualizado
 */
int atualiza_status_usuario(User* user, UserStatus status);

#endif