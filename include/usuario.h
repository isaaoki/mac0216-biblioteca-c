#ifndef USUARIO_H
#define USUARIO_H

#include <stdlib.h>

typedef enum {
    USER_BLOCKED,
    USER_OK
} UserStatus;

// Usuario
typedef struct User {
    unsigned int id;
    char nome[100];
    UserStatus status;
} User;

// Tabela Hash
typedef struct UserNode {
    User* user;
    struct UserNode* prox;
} UserNode;

// Conjunto dos usuarios
typedef struct UserConj{
    UserNode** users;
    size_t tam;
    size_t capacidade;
    unsigned int next_id;
} UserConj;

UserConj* cria_conj_usuarios(size_t capacidade);
void remove_conj_usuarios(UserConj* conjunto);
void redimensiona_conj_usuarios(UserConj* conjunto);

unsigned int cria_usuario(UserConj* conjunto, const char* nome, UserStatus status);
int remove_usuario(UserConj* conjunto, unsigned int user_id);
User* busca_usuario(UserConj* conjunto, unsigned int user_id);

const char* obter_nome_usuario(const User* user);
unsigned int obter_id_usuario(const User* user);
UserStatus obter_status_usuario(const User* user);

//Retorna 1 se o user puder pegar um livro 
//Retorna 0 se o user nao puder pegar um livro (ja tem um emprestimo ativo)
int verifica_emprestimo(const User* user);

//Atualiza o status o usuario
int atualiza_status_usuario(User* user, UserStatus status);

#endif