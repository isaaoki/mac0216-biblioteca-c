#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <string.h>
#include "emprestimo.h"

int main() {
    Emprestimo *cab = cria_lista_emprestimo();
    UserConj *usuarios = cria_conj_usuarios(10);
    Livro *catalogo = NULL;
    Livro *book;
    User *usuario;
    Emprestimo *encontrado;
    int multa = -1;

    catalogo = insere_livro(
        catalogo, "12345678", "Livro x", "Autor x", 2026
    );

    book = busca_livro(catalogo, "12345678");
    unsigned int id = cria_usuario(usuarios, "Usuario x", USER_OK);
    usuario = busca_usuario(usuarios, id);

    
    printf("Testes para Módulo empréstimo.\n");
    printf("Verificando variáveis a serem usadas...\n");
    assert(catalogo != NULL);
    assert(cab != NULL);
    assert(usuarios != NULL);
    assert(book != NULL);
    assert(id != 0);
    assert(usuario != NULL);
    printf("Alocação das variáveis bem sucedida.\n");


    printf("Teste 1 Empréstimo com parâmetros inválidos.\n");
    assert(empresta_livro(NULL, usuarios, book, id, book->isbn) == 0);
    assert(empresta_livro(cab, usuarios, book, id, NULL) == 0);
    assert(empresta_livro(cab, usuarios, book, id, "000000") == 0);
    assert(empresta_livro(cab, usuarios, NULL, id, book->isbn) == 0);
    assert(book->copias_disponiveis == 1);
    printf("Tratamento de erros realizado com sucesso.\n");

    printf("Teste 2 Empréstimo válido.\n");
    assert(empresta_livro(cab, usuarios, book, id, book->isbn) == 1);
    assert(book->copias_disponiveis == 0);
    assert(busca_emprestimo(cab, id, book->isbn) != NULL);
    printf("Empréstimo realizado com sucesso.\n");

    printf("Teste 2b Empréstimo com usuário bloqueado.\n");
    catalogo = insere_livro(catalogo, "87654321", "Livro y", "Autor y", 2026);
    Livro *book2 = busca_livro(catalogo, "87654321");
    assert(book2 != NULL && book2->copias_disponiveis == 1);
    // Usuário tem emprestimo ativo (USER_BLOCKED)
    assert(empresta_livro(cab, usuarios, book2, id, book2->isbn) == 0);
    assert(book2->copias_disponiveis == 1);
    printf("Usuário bloqueado não pode emprestar.\n");

    printf("Teste 2c Empréstimo de livro sem cópias disponíveis.\n");
    unsigned int id2 = cria_usuario(usuarios, "Usuario y", USER_OK);
    assert(id2 != 0);
    User *usuario2 = busca_usuario(usuarios, id2);
    assert(usuario2 != NULL && usuario_pode_emprestar(usuario2) == 1);
    // Livro tem 0 copias disponiveis
    assert(book->copias_disponiveis == 0);
    assert(empresta_livro(cab, usuarios, book, id2, book->isbn) == 0);
    assert(book->copias_disponiveis == 0);
    printf("Livro sem cópias não pode ser emprestado.\n");

    printf("Teste 3 Busca de empréstimo.\n");
    encontrado = busca_emprestimo(cab, id, book->isbn);
    assert(encontrado != NULL);
    assert(encontrado->user_id == id);
    assert(strcmp(encontrado->isbn, book->isbn) == 0);
    printf("Busca de empréstimo realizada com sucesso.\n");

    printf("Teste 4 Busca de empréstimo Inválido.\n");
    assert(busca_emprestimo(NULL, id, book->isbn) == NULL);
    assert(busca_emprestimo(cab, 0, book->isbn) == NULL);
    assert(busca_emprestimo(cab, id, NULL) == NULL);
    assert(busca_emprestimo(cab, id, "0000000") == NULL);
    printf("Tratamento de erros realizado com sucesso.\n");

    printf("Teste 5 Aumento do prazo.\n");
    assert(aumenta_prazo(cab, id, 3) == 1);
    assert(encontrado->prazo == 10);
    printf("Prazo aumentado com sucesso.\n");

    printf("Teste 6 Aumento do prazo com parâmetros inválidos.\n");
    assert(aumenta_prazo(NULL, id, 1) == 0);
    assert(aumenta_prazo(cab, id, -1) == 0);
    assert(aumenta_prazo(cab, 0, 1) == 0);
    assert(encontrado->prazo == 10);
    printf("Tratamento de erros realizado com sucesso.\n");

    printf("Teste 7 Cálculo de multa sem atraso.\n");
    assert(calcula_multa(encontrado, 10) == 0);
    printf("Multa igual a zero.\n");

    printf("Teste 8 Cálculo de multa com atraso.\n");
    assert(calcula_multa(encontrado, 12) == 4);
    printf("Multa calculada corretamente.\n");

    printf("Teste 9 Calculo de multa com parâmetros inválidos.\n");
    assert(calcula_multa(NULL, 10) == 0);
    assert(calcula_multa(encontrado, -1) == 0);
    printf("Tratamento de erros realizado com sucesso.\n");

    printf("Teste 10 Imprimindo dados do empréstimo.\n");
    imprime_emprestimos(cab);
    printf("Impressão realizada com sucesso.\n");

    printf("Teste 11 Impressão com parâmetro inválido.\n");
    imprime_emprestimos(NULL);
    printf("Impressão não realizada.\n");


    printf("Teste 12 Devolução válida.\n");
    assert(devolve_livro(cab, usuarios, book, id, &multa) == 1);
    assert(book->copias_disponiveis == 1);
    assert(busca_emprestimo(cab, id, book->isbn) == NULL);
    assert(multa == 0);
    printf("Devolução realizada com sucesso.\n");


    printf("Teste 13 Devolução de empréstimo inexistente.\n");
    multa = -1;
    assert(devolve_livro(cab, usuarios, book, id, &multa) == 0);
    assert(multa == 0);
    assert(book->copias_disponiveis == 1);
    printf("Tratamento de erros realizado com sucesso.\n");

    printf("Teste 14 Devolução com parâmetros inválidos.\n");
    assert(devolve_livro(NULL, usuarios, book, id, &multa) == 0);
    assert(devolve_livro(cab, NULL, book, id, &multa) == 0);
    assert(devolve_livro(cab, usuarios, NULL, id, &multa) == 0);
    printf("Tratamento de erros realizado com sucesso.\n");

    printf("Teste 15 Liberação da memória.\n");
    libera_lista_emprestimos(cab);
    assert(remove_conj_usuarios(usuarios) == 1);
    remove_catalogo(catalogo);
    printf("Memória liberada com sucesso.\n");
    
    return 0;
}
