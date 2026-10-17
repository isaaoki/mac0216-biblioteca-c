#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "catalogo.h"

int main() {
    Livro *catalogo = NULL;
    Livro *encontrado;

    // Insere um livro em um catalogo vazio.
    printf("Teste 1. Insere livro em catalogo vazio\n");
    catalogo = insere_livro(catalogo, "9780000000001", "Livro teste", "Autor teste", 2026);
    assert(catalogo != NULL);
    assert(strcmp(catalogo->isbn, "9780000000001") == 0);
    assert(strcmp(catalogo->nome, "Livro teste") == 0);
    assert(strcmp(catalogo->autor, "Autor teste") == 0);
    assert(catalogo->ano == 2026);
    assert(catalogo->copias_totais == 1);
    assert(catalogo->copias_disponiveis == 1);
    assert(catalogo->left == NULL && catalogo->right == NULL);
    printf("[APROVADO] Insercao do livro.\n");

    printf("Teste 2. Insere livro com parametros invalidos\n");
    assert(insere_livro(catalogo, NULL, "Livro teste", "Autor teste", 2026) == catalogo);
    assert(insere_livro(catalogo, "9780000000001", NULL, "Autor teste", 2026) == catalogo);
    assert(insere_livro(catalogo, "9780000000001", "Livro teste", NULL, 2026) == catalogo);
    assert(insere_livro(catalogo, "", "Livro teste", "Autor teste", 2026) == catalogo);
    assert(insere_livro(catalogo, "9780000000001", "", "Autor teste", 2026) == catalogo);
    assert(insere_livro(catalogo, "9780000000001", "Livro teste", "", 2026) == catalogo);
    assert(insere_livro(catalogo, "9780000000001", "Livro;teste", "Autor teste", 2026) == catalogo);
    assert(insere_livro(catalogo, "97800000000001", "Livro teste", "Autor teste", 2026) == catalogo);
    assert(insere_livro(catalogo, "9780000000001", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "Autor teste", 2026) == catalogo);
    assert(insere_livro(catalogo, "9780000000001", "Livro teste", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 2026) == catalogo);
    assert(insere_livro(catalogo, "9780000000001", "Livro teste", "Autor teste", -1) == catalogo);
    printf("[APROVADO] Insercao com parametros invalidos rejeitada.\n");

    //A busca deve retornar o mesmo livro que foi inserido.
    printf("Teste 3. Busca livro\n");
    encontrado = busca_livro(catalogo, "9780000000001");
    assert(encontrado == catalogo);
    printf("[APROVADO] Busca do livro.\n");

    printf("Teste 4. Busca livro com parametros invalidos\n");
    assert(busca_livro(catalogo, NULL) == NULL);
    assert(busca_livro(catalogo, "") == NULL);
    assert(busca_livro(catalogo, "9780000000002") == NULL); // nao encontrado
    printf("[APROVADO] Busca do livro com parametros invalidos rejeitada.\n");

    printf("Teste 5. Imprime livros\n");
    imprime_livros(catalogo);
    imprime_livros(NULL); // Nao imprime nada
    printf("[APROVADO] Impressao dos livros.\n");

    // Uma remocao invalida deve manter o livro no catalogo.
    printf("Teste 6. Remocao dos livros com parametros invalidos\n");
    encontrado = remove_livro(catalogo, NULL);
    assert(encontrado == catalogo);
    assert(catalogo->copias_totais == 1);
    printf("[APROVADO] Remocao com ISBN nulo.\n");

    // Ao remover o unico livro, o catalogo deve ficar vazio.
    printf("Teste 7. Remocao dos livros com parametros validos\n");
    catalogo = remove_livro(catalogo, "9780000000001");
    assert(catalogo == NULL);
    printf("[APROVADO] Remocao do livro.\n");

    printf("Teste 8. Insere ISBN repetido soma copias\n");
    catalogo = insere_livro(catalogo, "9780000000001", "Livro teste", "Autor teste", 2026);
    catalogo = insere_livro(catalogo, "9780000000001", "Livro teste", "Autor teste", 2026);
    assert(catalogo != NULL);
    assert(catalogo->left == NULL && catalogo->right == NULL);
    assert(catalogo->copias_totais == 2);
    assert(catalogo->copias_disponiveis == 2);
    printf("[APROVADO] ISBN repetido aumenta as copias.\n");

    printf("Teste 9. Remover catalogo inteiro\n");
    catalogo = insere_livro(catalogo, "9780000000001", "Livro teste", "Autor teste", 2026); // Insere novamente o livro
    remove_catalogo(catalogo);
    printf("[APROVADO] Remocao do catalogo (desalocacao de memoria).\n");

    return 0;
}
