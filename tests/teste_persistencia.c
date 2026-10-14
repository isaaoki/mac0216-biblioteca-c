#include <stdio.h>
#include <assert.h>
#include "persistencia.h"

void imprime_usuarios(UserConj *usuarios) {
   for (size_t i = 0; i < usuarios->capacidade; i++) {
        UserNode* u = usuarios->users[i];
        while (u != NULL) {
            printf("ID: %d, Nome: %s, Status %d\n", u->user->id, u->user->nome, u->user->status);
            u = u->prox;
        }
    }    
}

void imprime_livros(Livro *catalogo) {
    if (catalogo == NULL) return;

    imprime_livros(catalogo->left);
    printf("ISBN: %s, Nome: %s, Autor: %s, Ano: %d, Copias disponiveis: %d, Copias totais: %d\n", catalogo->isbn, catalogo->nome, catalogo->autor, catalogo->ano, catalogo->copias_disponiveis, catalogo->copias_totais);
    imprime_livros(catalogo->right);
}
void imprime_emprestimos(Emprestimo* emprestimos) {
    Emprestimo *atual = emprestimos;

    while (atual != NULL) {
        printf("ID do usuario: %u, ISBN: %s, Prazo: %d, Data do emprestimo: %ld, Data da devolucao: %ld\n", atual->user_id, atual->isbn, atual->prazo, (long) atual->data_emprestimo, (long)atual->data_devolucao);
        atual = atual->prox;
    }
}

int main() {
    printf("Testando persistencia dos usuarios...\n");
    printf("Teste 1: carrega usuarios de arquivo usuarios.txt\n");
    UserConj* usuarios = carrega_usuarios();
    assert(usuarios != NULL);
    
    if (usuarios != NULL) {
        imprime_usuarios(usuarios);
    }

    printf("Teste 2: salva usuarios em arquivo usuarios.txt\n");
    assert(salva_usuarios(usuarios) == 0);
    remove_conj_usuarios(usuarios);

    printf("Testando persistencia dos livros...\n");
    printf("Teste 3: carrega livros de arquivo livros.txt\n");
    Livro* catalogo = carrega_catalogo();
    assert(catalogo != NULL);
    
    if (catalogo != NULL) {
        imprime_livros(catalogo);
    }

    printf("Teste 4: salva livros em arquivo livros.txt\n");
    assert(salva_catalogo(catalogo) == 0);
    // TODO: Remover catologo

    printf("Testando persistencia dos emprestimos...\n");
    printf("Teste 5: carrega emprestimos de arquivo emprestimos.txt\n");
    Emprestimo* emprestimos = carrega_emprestimos();
    assert(emprestimos != NULL);

    imprime_emprestimos(emprestimos->prox); 

    printf("Teste 6: salva emprestimos em arquivo emprestimos.txt\n");
    assert(salva_emprestimos(emprestimos) == 0);
    emprestimo_destroy(emprestimos);

    return 0;
}