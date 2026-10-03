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
    char name[100];
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

