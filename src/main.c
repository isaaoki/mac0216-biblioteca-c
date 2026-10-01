#include <stdio.h>
#include <stdlib.h>
#include "catalog.h"

static void imprimir_resultado_busca(Book *catalogo, const char *isbn) {
    printf("\nBuscando ISBN %s:\n", isbn);
    if (search_and_info_Book(catalogo, isbn) == NULL) {
        printf("Livro nao encontrado.\n");
    }
}

int main(void) {
    Book *catalogo = NULL;

    catalogo = insert_Book(catalogo, "9780000000005", "Estruturas de Dados", "N. Wirth", 1976);
    catalogo = insert_Book(catalogo, "9780000000003", "Algoritmos", "R. Sedgewick", 2011);
    catalogo = insert_Book(catalogo, "9780000000007", "Linguagem C", "B. Kernighan", 1988);
    catalogo = insert_Book(catalogo, "9780000000002", "Calculo I", "J. Stewart", 2016);
    catalogo = insert_Book(catalogo, "9780000000004", "Calculo II", "J. Stewart", 2017);
    //testando livro repetido
    catalogo = insert_Book(catalogo, "9780000000005", "Estruturas de Dados", "N. Wirth", 1976);

    printf("=== Catalogo apos as insercoes ===\n");
    imprimir_resultado_busca(catalogo, "9780000000005");
    imprimir_resultado_busca(catalogo, "9780000000002");
    imprimir_resultado_busca(catalogo, "9780000000099");

    printf("\n=== Remocoes ===\n");
    catalogo = destroy_Book(catalogo, "9780000000002"); //folha
    catalogo = destroy_Book(catalogo, "9780000000003"); //um filho
    catalogo = destroy_Book(catalogo, "9780000000005"); //raiz com dois filhos

    imprimir_resultado_busca(catalogo, "9780000000002");
    imprimir_resultado_busca(catalogo, "9780000000003");
    imprimir_resultado_busca(catalogo, "9780000000004");
    imprimir_resultado_busca(catalogo, "9780000000007");

    //Libera os nos restantes usando a propria funcao de remocao
    catalogo = destroy_Book(catalogo, "9780000000004");
    catalogo = destroy_Book(catalogo, "9780000000007");

    if (catalogo == NULL){
        printf("\nTeste finalizado. Catalogo vazio.\n");
    }
    else{
        printf("\nTeste finalizado. Catalogo não vazio.\n");
    }
    return 0;
}
