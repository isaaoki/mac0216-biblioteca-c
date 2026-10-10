#include <stdio.h>
#include "../include/persistence.h"

int main() {
    printf("Testando persistência dos usuários...\n");
    UserConj* users = user_load();

    if (users != NULL) {
        printf("Usuários carregados com sucesso\n");
        
        for (size_t i = 0; i < users->capacidade; i++) {
            UserNode* u = users->users[i];
            while (u != NULL) {
                printf("ID: %d, Nome: %s, Status %d\n", u->user->id, u->user->nome, u->user->status);
                u = u->prox;
            }
        }
        user_save(users);
        remove_conj_usuarios(users);
    }

    printf("Testando persistência dos livros...\n");
    Livro* catalog = catalog_load();
    if (catalog != NULL) {
        printf("Livros carregados com sucesso\n");
        catalog_save(catalog);
        printf("Salvando livros no arquivo\n");
    }

    return 0;
}
