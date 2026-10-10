#include <stdio.h>
#include "../include/persistencia.h"

int main() {
    printf("Testando persistência dos usuários...\n");
    UserConj* users = carrega_usuarios();

    if (users != NULL) {
        printf("Usuários carregados com sucesso\n");
        
        for (size_t i = 0; i < users->capacidade; i++) {
            UserNode* u = users->users[i];
            while (u != NULL) {
                printf("ID: %d, Nome: %s, Status %d\n", u->user->id, u->user->nome, u->user->status);
                u = u->prox;
            }
        }
        salva_usuarios(users);
        remove_conj_usuarios(users);
    }

    printf("Testando persistência dos livros...\n");
    Livro* catalog = carrega_catalogo();
    if (catalog != NULL) {
        printf("Livros carregados com sucesso\n");
        salva_catalogo(catalog);
        printf("Salvando livros no arquivo\n");
    }

    return 0;
}
