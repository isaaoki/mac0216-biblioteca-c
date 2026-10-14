#include <stdio.h>
#include <assert.h>
#include "persistencia.h"

int main() {
    printf("Testando persistencia dos usuarios...\n");
    printf("Teste 1: carrega usuarios de arquivo usuarios.txt\n");
    UserConj* usuarios = carrega_usuarios("usuarios.txt");
    assert(usuarios != NULL);
    
    imprime_usuarios(usuarios);

    printf("Teste 2: salva usuarios em arquivo usuarios.txt\n");
    int resultado_usuarios = salva_usuarios(usuarios, "usuarios.txt");
    assert(resultado_usuarios == 0);

    printf("Testando persistencia dos livros...\n");
    printf("Teste 3: carrega livros de arquivo livros.txt\n");
    Livro* catalogo = carrega_catalogo("livros.txt");
    assert(catalogo != NULL);

    imprime_livros(catalogo);

    printf("Teste 4: salva livros em arquivo livros.txt\n");
    int resultado_catalogo = salva_catalogo(catalogo, "livros.txt");
    assert(resultado_catalogo == 0);

    printf("Testando persistencia dos emprestimos...\n");
    printf("Teste 5: carrega emprestimos de arquivo emprestimos.txt\n");
    Emprestimo* emprestimos = carrega_emprestimos("emprestimos.txt");
    assert(emprestimos != NULL);

    imprime_emprestimos(emprestimos); 

    printf("Teste 6: salva emprestimos em arquivo emprestimos.txt\n");
    int resultado_emprestimos = salva_emprestimos(emprestimos, "emprestimos.txt");
    assert(resultado_emprestimos == 0);

    // Testes com erro de arquivo inexistente
    printf("Testes 7, 8, 9: erro de leitura de arquivo inexistente");
    assert(carrega_usuarios("nao_existe.txt") == NULL);
    assert(carrega_catalogo("nao_existe.txt") == NULL);
    assert(carrega_emprestimos("nao_existe.txt") == NULL);

    printf("Testes 10, 11, 12: erro de salvar arquivo em diretório inexistente");
    assert(salva_usuarios(usuarios, "/dir_inexistente/usuarios.txt") == -1);
    assert(salva_catalogo(catalogo, "/dir_inexistente/catalogo.txt") == -1);
    assert(salva_emprestimos(emprestimos, "/dir_inexistente/emprestimos.txt") == -1);

    remove_conj_usuarios(usuarios);
    usuarios = NULL;
    remove_catalogo(catalogo);
    catalogo = NULL;
    remove_emprestimos(emprestimos);
    emprestimos = NULL;
    return 0;
}