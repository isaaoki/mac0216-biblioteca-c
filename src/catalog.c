#include "../include/catalog.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Book *insert_Book(
    Book *root, 
    const char *isbn,
    const char *nome,
    const char *autor,
    int ano
    ){
    if (isbn == NULL || nome == NULL || autor == NULL){
        printf("[ERRO]Dados invalidos.\n");
        return root;
    }

    if (isbn[0] == '\0') {
        printf("[ERRO]ISBN nao pode ser vazio.\n");
        return root;
    }

    if (strlen(isbn) > 13 || strlen(nome) > 49 || strlen(autor) > 49){
        printf("[ERRO]Texto maior que o permitido.\n");
        return root;
    }

    if (root == NULL){
        root = malloc(sizeof(Book));
        if (root == NULL){
            printf("[ERRO]Falta de memória.\n");
            return NULL;
        }
        strcpy(root->isbn, isbn);
        strcpy(root->nome, nome);
        strcpy(root->autor, autor);
        root->ano = ano;
        root->left = NULL;
        root->right = NULL;
        root->copias_totais = 1;
        root->copias_disponiveis = 1;
        return root;
    }

    Book* root_original = root;
    int pos;
    while(1){
        pos = strcmp(isbn, root->isbn);
        if (pos == 0){
            root->copias_totais++;
            root->copias_disponiveis++;
            return root_original;
        }
        else if (pos < 0){
            if (root->left == NULL){
                root->left = malloc(sizeof(Book));
                if (root->left == NULL){
                    printf("[ERRO]Falta de memória.\n");
                    return root_original;
                }
                root = root->left;
                root->left = NULL;
                root->right = NULL;
                root->ano = ano;
                strcpy(root->isbn, isbn);
                strcpy(root->nome, nome);
                strcpy(root->autor, autor);
                root->copias_totais = 1;
                root->copias_disponiveis = 1;
                return root_original;
            }
            root = root->left;
        }
        else if (pos > 0){
            if (root->right == NULL){
                root->right = malloc(sizeof(Book));
                if (root->right == NULL){
                    printf("[ERRO]Falta de memória.\n");
                    return root_original;
                }
                root = root->right;
                root->left = NULL;
                root->right = NULL;
                root->ano = ano;
                strcpy(root->isbn, isbn);
                strcpy(root->nome, nome);
                strcpy(root->autor, autor);
                root->copias_totais = 1;
                root->copias_disponiveis = 1;
                return root_original;
            }
            root = root->right;
        }
    }
}

Book *destroy_Book(Book *ROOT, const char *ISBN) {
    Book *root = ROOT;
    Book *father = NULL;

    if (ISBN == NULL){
        printf("[ERRO]O ISBN é inválido.\n");
        return ROOT;
    }
    
    while (root != NULL) {
        int pos = strcmp(ISBN, root->isbn);
        // caso encontre
        if (pos == 0) {
            // caso o livro a ser deletado seja a raiz:
            if (father == NULL) {
                if (root->left == NULL && root->right == NULL) {
                    ROOT = NULL;
                }
                else if (root->left != NULL && root->right == NULL) {
                    ROOT = root->left;
                }
                else if (root->left == NULL && root->right != NULL) {
                    ROOT = root->right;
                }
                else {
                    ROOT = root->left;
                    Book *substitute = root->left;
                    while (substitute->right != NULL) {
                        substitute = substitute->right;
                    }
                    substitute->right = root->right;
                }

                free(root);
                return ROOT;
            }
            if (root == father->right) {
                if (root->left == NULL && root->right == NULL) {
                    father->right = NULL;
                }
                else if (root->left == NULL && root->right != NULL) {
                    father->right = root->right;
                }
                else if (root->left != NULL && root->right == NULL) {
                    father->right = root->left;
                }
                else {
                    father->right = root->left;

                    Book *substitute = root->left;

                    while (substitute->right != NULL) {
                        substitute = substitute->right;
                    }

                    substitute->right = root->right;
                }
            }

            else {
                if (root->left == NULL && root->right == NULL) {
                    father->left = NULL;
                }
                else if (root->left != NULL && root->right == NULL) {
                    father->left = root->left;
                }
                else if (root->left == NULL && root->right != NULL) {
                    father->left = root->right;
                }
                else {
                    father->left = root->right;
                    Book *substitute = root->right;
                    while (substitute->left != NULL) {
                        substitute = substitute->left;
                    }
                    substitute->left = root->left;
                }
            }

            free(root);
            return ROOT;
        }
        // movimentação pela árvore...
        else if (pos < 0) {
            father = root;
            root = root->left;
        }

        else {
            father = root;
            root = root->right;
        }
    }

    return ROOT;
}


//Melhorado
Book* search_and_info_Book(Book *root, const char *ISBN){
    if (ISBN == NULL || ISBN[0] == '\0'){
        return NULL;
    }
    while (root != NULL){
        int pos = strcmp(ISBN,root->isbn);
        if (pos == 0){
            printf(
                "ISBN: %s\n"
                "Nome do livro: %s\n"
                "Autor do livro: %s\n"
                "Ano de publicação: %d\n"
                "Disponíveis: %d\n"
                "Totais: %d\n",
                ISBN,
                root->nome,
                root->autor,
                root->ano,
                root->copias_disponiveis,
                root->copias_totais
            );
            return root;
        }
        else if (pos < 0){
            root = root->left;
        }
        else{
            root = root->right;
        }
    }
    return NULL;
}
