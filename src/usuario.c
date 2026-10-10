#include "../include/usuario.h"
#include <string.h>


static unsigned int hash_id(unsigned int id, size_t capacidade) {
    return id % capacidade;
}

UserConj* cria_conj_usuarios(size_t c){
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

int remove_conj_usuarios(UserConj* c){
    if (!c) return 0;
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
    return 1;
}

int redimensiona_conj_usuarios(UserConj* conj){
    if (!conj) return 0;
    size_t c=conj->capacidade*2;
    UserNode** novo=(UserNode**) calloc(c, sizeof(UserNode*));
    if (!novo) return 0;
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
    return 1;
}

unsigned int cria_usuario(UserConj* conj, const char* s, UserStatus status){
    if (!conj || !s) return 0;    
    if (conj->tam >= conj->capacidade * 0.75) {
        redimensiona_conj_usuarios(conj);
    }
    User* u = (User*) malloc(sizeof(User));
    if (!u) return 0;
    u->id=conj->next_id;
    conj->next_id++;
    strcpy(u->nome, s);
    u->status = status;
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


int remove_usuario(UserConj* conj, unsigned int id){
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

User* busca_usuario(UserConj* conj, unsigned int id){
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

const char* obter_nome_usuario(const User* u){
    if (!u) return NULL;
    return u->nome;
}

unsigned int obter_id_usuario(const User* u){
    if (!u) return 0;
    return u->id;
}

UserStatus obter_status_usuario(const User* u){
    if (!u) return USER_BLOCKED;
    return u->status;
}

int verifica_emprestimo(const User* u){
    if (!u) return 0;
    return (u->status==USER_OK);
}

int atualiza_status_usuario(User* u, UserStatus status){
    if (!u) return 0;
    u->status=status;
    return 1;
}