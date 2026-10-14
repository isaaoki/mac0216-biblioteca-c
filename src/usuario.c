#include "../include/usuario.h"
#include <string.h>
#include <stdio.h>

// 1. Funcao Basica de Hash para tabela
static unsigned int hash_id(unsigned int id, size_t capacidade) {
    return id % capacidade;
}

// 2. Criar o conjunto de usuarios com capacidade c
UserConj* cria_conj_usuarios(size_t c){
    if (c<=0) c=16; //Caso tenham passado uma capacidade <=0, o padrão é 16

    UserConj* conjunto= (UserConj*) malloc(sizeof(UserConj));
    if (!conjunto) return NULL; //caso a alocacao de errado

    conjunto->users= (UserNode**) calloc(c,sizeof(UserNode*));
    if (!conjunto->users) {// caso alocacao de errado
        free(conjunto);
        return NULL;
    }
    //Setando os valores do conjunto
    conjunto->tam=0;
    conjunto->capacidade=c;
    conjunto -> next_id=1;

    return conjunto;
}

// 3. Excluir o conjunto de usuarios 
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
    return 1; //Sucesso
}

// 4. Redimensionar o conjunto de usuarios (Duplica a capacidade)
int redimensiona_conj_usuarios(UserConj* conj){
    if (!conj) return 0;

    size_t c=conj->capacidade*2;
    UserNode** novo=(UserNode**) calloc(c, sizeof(UserNode*));
    if (!novo) return 0; //caso de errado a alocacao

    for (size_t i=0;i<conj->capacidade;i++){  //reindexando os nós
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
    return 1; //Sucesso
}

// 5. Registrar um usuario no sistema(criar e adicionar no conjunto)
unsigned int cria_usuario(UserConj* conj, const char* s, UserStatus status){
    if (!conj || !s) return 0;

    if (conj->tam >= conj->capacidade * 0.75) {
        redimensiona_conj_usuarios(conj);
    }

    User* u = (User*) malloc(sizeof(User));
    if (!u) return 0; //caso a alocacao de errado

    //Definindo os valores do usuario
    u->id=conj->next_id; 
    conj->next_id++;
    strcpy(u->nome, s); 
    u->status = status; 
    unsigned int idx=hash_id(u->id, conj->capacidade);

    UserNode* novo=(UserNode*) malloc(sizeof(UserNode));
    if (!novo){ //caso a alocacao de errado
        free(u);
        return 0;
    }
    
    //Transformando em um nó e adicionando no conjunto
    novo->user=u;
    novo->prox=conj->users[idx];
    conj->users[idx]=novo;
    conj->tam++;

    return u->id; //Sucesso retorna o id do usuario
}

// 6. Excluir um usuario do conjunto de acordo com o id
int remove_usuario(UserConj* conj, unsigned int id){
   if (!conj || id==0) return 0;

   unsigned int idx=hash_id(id, conj->capacidade); //encontrar o idx correspondente da tabela hash
   UserNode* atual=conj->users[idx];
   UserNode* prev=NULL;

   while (atual){ //Percorre o bucket
        User* u=atual->user;
        if (u->id==id){ //Se for o usuario com o id que procuramos
            if (u->status == USER_BLOCKED) return 0; //nao deleta se estiver com emprestimo pendente
            if (prev){
                prev->prox=atual->prox;
            } else{
                conj->users[idx]=atual->prox;
            }
            free(u);
            free(atual);
            conj->tam--;
            return 1; //Sucesso
        }
        prev=atual;
        atual=atual->prox;
   }
   
   return 0; //Caso nao tenha encontrado algum usuario com esse id
}

// 7. Buscar um usuario do conjunto de acordo com o id
User* busca_usuario(UserConj* conj, unsigned int id){
    if (!conj || id==0) return NULL;

    unsigned int idx=hash_id(id, conj->capacidade); //Acha o idx da tabela correspondente ao id
    UserNode* atual=conj->users[idx];
    while(atual){ //Percorre o bucket
        User* u=atual->user;
        if (u->id == id){ //Se for o usuario com o id que procuramos
            return u;
        }
        atual=atual->prox;
    }
    return NULL; //Caso nao tenha encontrado algum usuario com esse id
}

//8. Funcoes de consulta
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

void imprime_usuarios(UserConj *usuarios) {
   for (size_t i = 0; i < usuarios->capacidade; i++) {
        UserNode* u = usuarios->users[i];
        while (u != NULL) {
            printf("ID: %d, Nome: %s, Status %d\n", u->user->id, u->user->nome, u->user->status);
            u = u->prox;
        }
    }    
}