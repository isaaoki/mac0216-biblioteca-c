#include <stdio.h>
#include <assert.h>
#include "../include/catalogo.h"

int main(void) {
    Livro *catalogo = NULL;
    Livro *encontrado;

    // Insere um livro em um catalogo vazio.
    catalogo = insere_livro(catalogo, "9780000000001", "Livro teste", "Autor teste", 2026);

    assert(catalogo != NULL);
    printf("[APROVADO] Insercao do livro.\n");

    //A busca deve retornar o mesmo livro que foi inserido.
    encontrado = busca_livro(catalogo, "9780000000001");

    assert(encontrado == catalogo);
    printf("[APROVADO] Busca do livro.\n");

    // Entrada invalida na busca.
    encontrado = insere_livro(catalogo, NULL, NULL, NULL, 0);
    assert(encontrado == NULL);
    printf("[APROVADO] Busca com ISBN nulo.\n");

    // Uma remocao invalida deve manter o livro no catalogo.
    encontrado = remove_livro(catalogo, NULL);
    assert(encontrado == catalogo);
    assert(catalogo->copias_totais == 1);
    printf("[APROVADO] Remocao com ISBN nulo.\n");

    // Ao remover o unico livro, o catalogo deve ficar vazio.
    catalogo = remove_livro(catalogo, "9780000000001");

    assert(catalogo == NULL);
    printf("[APROVADO] Remocao do livro.\n");

    //Antes da correcao do ISBN vazio, este assert falhava.
    catalogo = insere_livro(catalogo, "", "Livro teste", "Autor teste", 2026);
    assert(catalogo == NULL);
    printf("[APROVADO] Insercao com ISBN vazio rejeitada.\n");

    // O limite e 13 bytes: um ISBN com 14 deve ser rejeitado.
    catalogo = insere_livro(catalogo, "97800000000012", "Livro teste", "Autor teste", 2026);
    assert(catalogo == NULL);
    printf("[APROVADO] Insercao com ISBN grande demais rejeitada.\n");

    return 0;
}
