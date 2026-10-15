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
 * @param capacidade Capacidade inicial da tabela hash. Se igual a 0, utiliza o valor padrão 16.
 * @returns Ponteiro para o conjunto criado, ou NULL em caso de falha na alocação de memória.
 * @post O conjunto de usuários é alocado, com tamanho 0 e next_id inicializado como 1.
 * @note Caso ocorra falha ao alocar memória para o conjunto ou para a tabela, a função retorna NULL.
 */
UserConj* cria_conj_usuarios(size_t capacidade);

/**
 * @brief Exclui o conjunto de usuários e libera a memória.
 * @param conjunto Ponteiro para o conjunto de usuários que será excluído.
 * @returns 1 se teve sucesso, 0 caso contrário.
 * @pre conjunto aponta para um UserConj válido. Caso for NULL, a função retorna 0.
 * @post Toda a memória alocada para os nós, os usuários e a tabela hash é liberada.
 * @note Se o conjunto for NULL, a função não realiza nenhuma operação e retorna 0.
 */
int remove_conj_usuarios(UserConj* conjunto);

/**
 * @brief Registra um novo usuário no conjunto.
 * @param conjunto Ponteiro para o conjunto de usuários.
 * @param nome Nome do usuário (1 a 99 caracteres, sem ';' nem '\n').
 * @param status Status inicial do usuário (USER_OK ou USER_BLOCKED).
 * @returns O ID único gerado para o novo usuário, ou 0 em caso de erro.
 * @pre conjunto e nome devem ser válidos (diferentes de NULL) e nome deve ser limitado por 100 caracteres.
 * @post Um novo usuário é criado com ID, inserido na tabela e o tamanho do conjunto é incrementado.
 * @note Caso o tamanho atinja 75% da capacidade, o conjunto é redimensionado automaticamente. 
 * @note Caso conjunto ou nome forem NULL, a função retorna 0. 
 * @note Caso o nome for maior que 99 caracteres, a função retorna 0.
 * @note Caso haja falha de alocação de memória, a função retorna 0 (no redimensionamento e ao criar um novo usuário).
 */
unsigned int cria_usuario(UserConj* conjunto, const char* nome, UserStatus status);

/**
 * @brief Registra um novo usuário no conjunto, criando junto do ID (uso para carregamento de arquivos).
 * @param conjunto Ponteiro para o conjunto de usuários.
 * @param id Id do usuário a ser criado (>= 1 e < UINT_MAX; não pode já existir no conjunto).
 * @param nome Nome do novo usuário (1 a 99 caracteres, sem ';' nem '\n').
 * @param status Status inicial do usuário (USER_OK ou USER_BLOCKED).
 * @returns O id do usuário, ou 0 em caso de erro.
 * @pre conjunto e nome não são NULL.
 * @post Um novo usuário é criado com ID, inserido na tabela e o tamanho do conjunto é incrementado. next_id passa a ser maior que id (se já não era).
 * @note O conjunto é redimensionado automaticamente ao atingir 75% da capacidade.
 * @note Retorna 0 se conjunto ou nome forem NULL, se id for 0 ou UINT_MAX, se já existir
 * usuário com esse id, se o nome ou o status forem inválidos ou se houver falha de alocação.
 */
unsigned int cria_usuario_com_id(UserConj *conjunto, unsigned int id, const char *nome, UserStatus status);

/**
 * @brief Remove um usuário do conjunto com base no seu ID.
 * @param conjunto Ponteiro para o conjunto de usuários.
 * @param user_id Id do usuário a ser removido.
 * @returns 1 se a remoção foi bem-sucedida. 0 caso conjunto seja NULL ou user_id for 0, se o usuário estiver bloqueado ou se o usuário não existir.
 * @pre conjunto aponta para um UserConj válido e user_id > 0.
 * @post O usuário é removido da tabela hash e sua memória é liberada, caso não possua nenhuma pendência.
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
 * @warning O ponteiro retornado pertence ao conjunto e não deve ser liberado pelo chamador. Ele permanece válido até que o usuário seja removido com a função remove_usuario ou o conjunto seja destruído com remove_conj_usuarios.
 */
User* busca_usuario(UserConj* conjunto, unsigned int user_id);

/**
 * @brief Retorna o nome de um usuário.
 * @param user Ponteiro para User.
 * @returns String contendo o nome do usuário, ou NULL se o ponteiro for inválido.
 * @pre user aponta para um User válido ou é NULL.
 */
const char* obtem_nome_usuario(const User* user);

/**
 * @brief Retorna o Id de um usuário.
 * @param user Ponteiro para User.
 * @returns O Id do usuário, ou 0 em caso de ponteiro inválido.
 * @pre user aponta para um User válido ou é NULL.
 */
unsigned int obtem_id_usuario(const User* user);

/**
 * @brief Retorna o status de um usuário.
 * @param user Ponteiro para User.
 * @returns O status atual do usuário, ou USER_BLOCKED se o usuário for NULL.
 * @pre user aponta para um User válido ou é NULL.
 */
UserStatus obtem_status_usuario(const User* user);

/**
 * @brief Verifica se o usuário pode realizar um empréstimo.
 * @param user Ponteiro para User.
 * @returns 1 se o usuário puder pegar um livro (status USER_OK), 0 se não puder.
 * @pre user aponta para um User válido ou é NULL.
 * @note Cada usuário possui o limite de 1 empréstimo por vez.
 */
int usuario_pode_emprestar(const User* user);

/**
 * @brief Atualiza o status do usuário
 * @param user Ponteiro para User.
 * @param status Status a ser setado (USER_OK ou USER_BLOCKED).
 * @returns 1 se teve sucesso, 0 se não teve sucesso.
 * @pre user aponta para um usuário válido ou é NULL.
 * @pre status é USER_OK ou USER_BLOCKED.
 * @post o status do usuário é atualizado.
 */
int atualiza_status_usuario(User* user, UserStatus status);

/**
 * @brief Imprime os usuários salvos do programa.
 * @param usuarios Ponteiro para o conjunto de usuários;
 * @note Se usuarios for NULL, nada é impresso.
 * @note A ordem não é determinística.
 */
void imprime_usuarios(UserConj *usuarios);

#endif