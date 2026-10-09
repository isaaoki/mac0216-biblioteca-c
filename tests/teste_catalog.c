#include <stdio.h>
#include <assert.h>
#include "catalog.h"

int main(void) {
    Book *catalogo = NULL;
    Book *encontrado;

    // Insere um livro em um catalogo vazio.
    catalogo = insert_Book(catalogo, "9780000000001", "Livro teste", "Autor teste", 2026);

    assert(catalogo != NULL);
    printf("[APROVADO] Insercao do livro.\n");

    //A busca deve retornar o mesmo livro que foi inserido.
    encontrado = search_and_info_Book(catalogo, "9780000000001");

    assert(encontrado == catalogo);
    printf("[APROVADO] Busca do livro.\n");

    // Entrada invalida na busca.
    encontrado = search_and_info_Book(catalogo, NULL);
    assert(encontrado == NULL);
    printf("[APROVADO] Busca com ISBN nulo.\n");

    // Uma remocao invalida deve manter o livro no catalogo.
    encontrado = destroy_Book(catalogo, NULL);
    assert(encontrado == catalogo);
    assert(catalogo->copias_totais == 1);
    printf("[APROVADO] Remocao com ISBN nulo.\n");

    // Ao remover o unico livro, o catalogo deve ficar vazio.
    catalogo = destroy_Book(catalogo, "9780000000001");

    assert(catalogo == NULL);
    printf("[APROVADO] Remocao do livro.\n");

    //Antes da correcao do ISBN vazio, este assert falhava.
    catalogo = insert_Book(catalogo, "", "Livro teste", "Autor teste", 2026);
    assert(catalogo == NULL);
    printf("[APROVADO] Insercao com ISBN vazio rejeitada.\n");

    // O limite e 13 bytes: um ISBN com 14 deve ser rejeitado.
    catalogo = insert_Book(catalogo, "97800000000012", "Livro teste", "Autor teste", 2026);
    assert(catalogo == NULL);
    printf("[APROVADO] Insercao com ISBN grande demais rejeitada.\n");

    return 0;
}
