#include "../include/usuario.h"
#include <stdlib.h>
#include <string.h>
/*
Ideia:
Usuario: nome, id, status(pode ou nao pegar um livro)
(so pode fazer 1 emprestimo por vez)
Conjunto de Usuarios: usuarios, tamanho do conjunto, capacidade max

user_conj_create: criar o conjunto com capacidade inicial c ou 16 por padrao
user_conj_destroy: excluir o conjunto e liberar a memoria

user_register:registrar um novo usuario(nome),cria um id e atualiza o status.
user_delete: exclui um usuario apenas se ele nao esteja com status pendente
user_find: encontra o Usuario pelo id

user_get_name: retorna o nome
user_get_id: retorna o id
user_get_status: retorna o status

user_emprestimo: retorna se o cliente pode fazer um emprestimo (apenas se ele nao estiver com um pendente)
user_set_status: para alterar o estado do usuario, USER_OK se nao tem pendente, USER_BLOCKED se estiver com um livro
*/



//Usuario
struct User {
    unsigned int id;
    char nome[100];
    UserStatus status;
};
//Conjunto dos usuarios
struct UserConj{
    User** users;
    size_t tam;
    size_t capacidade;
    unsigned int next_id;
};

UserConj* user_conj_create(size_t c){
    if (c==0) c=16;
    UserConj* conjunto= (UserConj*) malloc(sizeof(UserConj));
    if (!conjunto) return NULL; //caso a alocacao de errado
    conjunto->users= (User**) malloc(sizeof(User*)*c);
    if (!conjunto->users) {// caso alocacao de errado
        free(conjunto);
        return NULL;
    }
    conjunto->tam=0;
    conjunto->capacidade=c;
    return conjunto;
}
void user_conj_destroy(UserConj* c){
    if (!c) return ;
   for (size_t i; i<c->tam;i++){
        if (c->users[i]){
            free(c->users[i]);
        }
   }
   free(c->users);
   free(c);
}

unsigned int user_register(UserConj* conj, const char* s){
    if (!conj || !s) return 0;
    //caso o vetor atingiu a capacidade
    if (conj->tam >= conj->capacidade){
        size_t c=conj->capacidade*2;
        User** novo_users=(User**) realloc(conj->users, sizeof(User*)*c);
        if (!novo_users) return 0; //caso falhe a alocacao
        conj->users=novo_users;
        conj->capacidade=c;
    }
    User* u = (User*) malloc(sizeof(User));
    if (!u) return 0;
    u->id=conj->next_id;
    conj->next_id++;
    strcpy(u->nome, s);
    u->status = USER_OK;
    conj->users[conj->tam] = u;
    conj->tam++;
    return u->id;
}


int user_delete(UserConj* conj, unsigned int id){
   if (!conj) return 0;
   for (size_t i=0;i<conj->tam;i++){
        User* u=conj->users[i];
        if (u->id == id && u->status != USER_BLOCKED){
            free(conj->users[i]);
            for (size_t j = i; j < conj->tam - 1; j++) {
                conj->users[j] = conj->users[j + 1];
            }
            conj->tam--;
            return 1;
        }
   }
   return 0;
}

User* user_find(UserConj* conj, unsigned int id){
    if (!conj) return NULL;
    for (size_t i;i<conj->tam;i++){
        User* u=conj->users[i];
        if (u->id==id) return u;
    }
    return NULL;
}
