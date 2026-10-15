#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "persistencia.h"

static void confere_usuarios(UserConj *usuarios, UserConj *copia) {
    for (size_t i = 0; i < usuarios->capacidade; i++) {
        UserNode* u = usuarios->users[i];

        while (u != NULL) {
            User *usuario = busca_usuario(copia, u->user->id);
            assert(usuario != NULL);
            assert(strcmp(usuario->nome, u->user->nome) == 0);
            int status1 = usuario->status == USER_OK ? 1 : 0;
            int status2 = u->user->status == USER_OK ? 1 : 0;
            assert(status1 == status2);
            u = u->prox;
        }
    }
}

static void confere_livros(Livro *a, Livro *b) {
    if (a == NULL) return;

    confere_livros(a->left, b);

    Livro *livro = busca_livro(b, a->isbn);
    assert(livro != NULL);
    assert(strcmp(livro->nome, a->nome) == 0);
    assert(strcmp(livro->autor, a->autor) == 0);
    assert(livro->ano == a->ano);
    assert(livro->copias_disponiveis == a->copias_disponiveis);
    assert(livro->copias_totais == a->copias_totais);

    confere_livros(a->right, b);
}

static int conta_livros(Livro *a) {
    if (a == NULL) return 0;
    else return 1 + conta_livros(a->left) + conta_livros(a->right);
}

static void confere_emprestimos(Emprestimo *a, Emprestimo *b) {
    Emprestimo *temp = a->prox;

    while(temp != NULL) {
        Emprestimo *emprestimo = busca_emprestimo(b, temp->user_id, temp->isbn);
        assert(emprestimo != NULL);
        assert(emprestimo->prazo == temp->prazo);
        assert(emprestimo->data_emprestimo == temp->data_emprestimo);
        assert(emprestimo->data_devolucao == temp->data_devolucao);
        temp = temp->prox;
    }
}

static int conta_emprestimos(Emprestimo *a) {
    int n = 0;
    Emprestimo *temp = a->prox;
    while (temp != NULL) {
        n++;
        temp = temp->prox;
    }
    return n;
}

int main() {
    printf("Testando persistencia dos usuarios...\n");
    printf("Teste 1: carrega usuarios de arquivo usuarios.txt\n");
    UserConj* usuarios = carrega_usuarios("usuarios.txt");
    assert(usuarios != NULL);
    
    imprime_usuarios(usuarios);

    printf("Teste 2: salva usuarios em arquivo usuarios.txt\n");
    int resultado_usuarios = salva_usuarios(usuarios, "usuarios.txt");
    assert(resultado_usuarios == 1);

    printf("Testando persistencia dos livros...\n");
    printf("Teste 3: carrega livros de arquivo livros.txt\n");
    Livro* catalogo = carrega_catalogo("livros.txt");
    assert(catalogo != NULL);

    imprime_livros(catalogo);

    printf("Teste 4: salva livros em arquivo livros.txt\n");
    int resultado_catalogo = salva_catalogo(catalogo, "livros.txt");
    assert(resultado_catalogo == 1);

    printf("Testando persistencia dos emprestimos...\n");
    printf("Teste 5: carrega emprestimos de arquivo emprestimos.txt\n");
    Emprestimo* emprestimos = carrega_emprestimos("emprestimos.txt");
    assert(emprestimos != NULL);

    imprime_emprestimos(emprestimos); 

    printf("Teste 6: salva emprestimos em arquivo emprestimos.txt\n");
    int resultado_emprestimos = salva_emprestimos(emprestimos, "emprestimos.txt");
    assert(resultado_emprestimos == 1);

    // Carregar de novo os mesmos dados e comparar
    printf("Testando o carregamento dos dados novamente e comparando...\n");
    printf("Teste 7: carrega os usuarios novamente e compara\n");
    UserConj* usuarios_copia = carrega_usuarios("usuarios.txt");
    assert(usuarios_copia != NULL);
    assert(usuarios->tam == usuarios_copia->tam);
    confere_usuarios(usuarios, usuarios_copia);
    remove_conj_usuarios(usuarios_copia);
    usuarios_copia = NULL;

    printf("Teste 8: carrega os livros novamente e compara\n");
    Livro* catalogo_copia = carrega_catalogo("livros.txt");
    assert(catalogo_copia != NULL);
    assert(conta_livros(catalogo) == conta_livros(catalogo_copia));
    confere_livros(catalogo, catalogo_copia);
    remove_catalogo(catalogo_copia);
    catalogo_copia = NULL;

    printf("Teste 9: carrega os emprestimos novamente e compara\n");
    Emprestimo* emprestimos_copia = carrega_emprestimos("emprestimos.txt");
    assert(emprestimos_copia != NULL);
    assert(conta_emprestimos(emprestimos) == conta_emprestimos(emprestimos_copia));
    confere_emprestimos(emprestimos, emprestimos_copia);
    libera_lista_emprestimos(emprestimos_copia);
    emprestimos_copia = NULL;

    // Testes com erro de arquivo/diretorio inexistente
    printf("Testes 10, 11, 12: erro de leitura de arquivo inexistente\n");
    assert(carrega_usuarios("nao_existe.txt") == NULL);
    assert(carrega_catalogo("nao_existe.txt") == NULL);
    assert(carrega_emprestimos("nao_existe.txt") == NULL);

    printf("Testes 13, 14, 15: erro de salvar arquivo em diretorio inexistente\n");
    assert(salva_usuarios(usuarios, "/dir_inexistente/usuarios.txt") == 0);
    assert(salva_catalogo(catalogo, "/dir_inexistente/catalogo.txt") == 0);
    assert(salva_emprestimos(emprestimos, "/dir_inexistente/emprestimos.txt") == 0);

    remove_conj_usuarios(usuarios);
    usuarios = NULL;
    remove_catalogo(catalogo);
    catalogo = NULL;
    libera_lista_emprestimos(emprestimos);
    emprestimos = NULL;
    return 0;
}