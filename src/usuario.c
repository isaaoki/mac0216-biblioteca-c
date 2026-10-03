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
user_conj_resize: aumenta a capacidade da tabela hash

user_register:registrar um novo usuario(nome),cria um id e atualiza o status.
user_delete: exclui um usuario apenas se ele nao esteja com status pendente
user_find: encontra o Usuario pelo id

user_get_name: retorna o nome
user_get_id: retorna o id
user_get_status: retorna o status

user_emprestimo: retorna se o cliente pode fazer um emprestimo (apenas se ele nao estiver com um pendente)
user_set_status: para alterar o estado do usuario, USER_OK se nao tem pendente, USER_BLOCKED se estiver com um livro
*/

//Tabela Hash
typedef struct UserNode {
    User* user;
    struct UserNode* prox;
} UserNode;
//Usuario
struct User {
    unsigned int id;
    char nome[100];
    UserStatus status;
};
//Conjunto dos usuarios
struct UserConj{
    UserNode** users;
    size_t tam;
    size_t capacidade;
    unsigned int next_id;
};

static unsigned int hash_id(unsigned int id, size_t capacidade) {
    return id % capacidade;
}

UserConj* user_conj_create(size_t c){
    if (c==0) c=16;
    UserConj* conjunto= (UserConj*) malloc(sizeof(UserConj));
    if (!conjunto) return NULL; //caso a alocacao de errado
    conjunto->users= (UserNode**) calloc(c,sizeof(UserNode*));
    if (!conjunto->users) {// caso alocacao de errado
        free(conjunto);
        return NULL;
    }
    conjunto->tam=0;
    conjunto->capacidade=c;
    conjunto -> next_id=1;
    return conjunto;
}
void user_conj_destroy(UserConj* c){
    if (!c) return ;
    if (c->users){
        for (size_t i=0; i<c->capacidade;i++){
            UserNode* atual=c->users[i];
            while (atual){
                UserNode* aux=atual;
                atual=atual->prox;
                if (aux->user) free(aux->user);
                free(aux);
            }
        }
        free(c->users);
    }
    free(c);
}
void user_conj_resize(UserConj* conj){
    size_t c=conj->capacidade*2;
    UserNode** novo=(UserNode**) calloc(c, sizeof(UserNode*));
    if (!novo) return;
    for (size_t i=0;i<conj->capacidade;i++){
        UserNode* atual=conj->users[i];
        while (atual){
            UserNode* prox=atual->prox;
            unsigned int idx=hash_id(atual->user->id, c);
            atual->prox=novo[idx];
            novo[idx]=atual;
            atual=prox;
        }
    }
    free(conj->users);
    conj->users=novo;
    conj->capacidade=c;


}
unsigned int user_register(UserConj* conj, const char* s){
    if (!conj || !s) return 0;    
    if (conj->tam >= conj->capacidade * 0.75) {
        user_conj_resize(conj);
    }
    User* u = (User*) malloc(sizeof(User));
    if (!u) return 0;
    u->id=conj->next_id;
    conj->next_id++;
    strcpy(u->nome, s);
    u->status = USER_OK;
    unsigned int idx=hash_id(u->id, conj->capacidade);
    UserNode* novo=(UserNode*) malloc(sizeof(UserNode));
    if (!novo){
        free(u);
        return 0;
    }
    novo->user=u;
    novo->prox=conj->users[idx];
    conj->users[idx]=novo;
    conj->tam++;
    return u->id;
}


int user_delete(UserConj* conj, unsigned int id){
   if (!conj || id==0) return 0;
   unsigned int idx=hash_id(id, conj->capacidade);
   UserNode* atual=conj->users[idx];
   UserNode* prev=NULL;
   while (atual){
        User* u=atual->user;
        if (u->id==id){
            if (u->status == USER_BLOCKED) return 0; //nao deleta se estiver com emprestimo pendente
            if (prev){
                prev->prox=atual->prox;
            } else{
                conj->users[idx]=atual->prox;
            }
            free(u);
            free(atual);
            conj->tam--;
            return 1;
        }
        prev=atual;
        atual=atual->prox;
   }
   
   return 0;
}

User* user_find(UserConj* conj, unsigned int id){
    if (!conj || id==0) return NULL;
    unsigned int idx=hash_id(id, conj->capacidade);
    UserNode* atual=conj->users[idx];
    while(atual){
        User* u=atual->user;
        if (u->id == id){
            return u;
        }
        atual=atual->prox;
    }
    return NULL;
}

const char* user_get_name(const User* u){
    if (!u) return NULL;
    return u->nome;
}
unsigned int user_get_id(const User* u){
    if (!u) return 0;
    return u->id;
}
UserStatus user_get_status(const User* u){
    if (!u) return USER_BLOCKED;
    return u->status;
}

int user_emprestimo(const User* u){
    if (!u) return 0;
    return (u->status==USER_OK);
}

int user_set_status(User* u, UserStatus status){
    if (!u) return 0;
    u->status=status;
    return 1;
}