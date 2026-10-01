#include "../include/catalog.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Book *insert_Book(
    Book *root, 
    char isbn[14],
    char nome[50],
    char autor[50],
    int ano
    ){
    if (root == NULL){
        root = malloc(sizeof(Book));
        if (root == NULL){
            printf("Falta de memória.\n");
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
                    printf("Falta de memória.\n");
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
                    printf("Falta de memória.\n");
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

Book *destroy_book(Book *root, char *ISBN){
    Book* ROOT = root;
    Book* father = 0;

    while (root != 0){
        if (root-> value == value){
            if (father == 0){
                if (root->left == 0 && root->right == 0){
                }
                else if (root->left != 0 && root->right == 0){
                    ROOT = root->left;
                }
                else if (root->left == 0 && root->right != 0){
                    ROOT = root->right;
                }
                else{
                    ROOT = root->left;
                    Book *substitute = root->left;
                    while (substitute->right != 0){
                        substitute = substitute->right;
                    }
                    substitute->right = root->right;
                }
                free(root);
                return ROOT;
            }
            if (root->value >= father->value){
                if (root->left == 0 && root->right == 0){
                    father->right = 0;
                    free(root);
                    return ROOT;
                }
                else if (root->left == 0 && root->right != 0){
                    father->right = root->right;
                }
                else if (root->left != 0 && root->right == 0){
                    father->right = root->left;
                }
                else{
                    father->right = root->left;
                    Book* substitute = root->left;
                    while (substitute->right != 0){
                        substitute = substitute->right;
                    }
                    substitute->right = root->right;
                }
                free(root);
                return ROOT;
            }
            else{
                if (root->left == 0 && root->right == 0){
                    father->left = 0;
                    free(root);
                    return ROOT;
                }
                else if (root->left != 0 && root->right == 0){
                    father->left = root->left;
                }
                else if (root->left == 0 && root->right != 0){
                    father->left = root->right;
                }
                else{
                    father->left = root->right;
                    Book* substitute = root->right;
                    while (substitute->left != 0){
                        substitute = substitute->left;
                    }
                    substitute->left = root->left;
                }
                free(root);
                return ROOT;
            
            }
        }
        else if (value < root->value){
            father = root;
            root = root->left;
        }
        else{
            father = root;
            root = root->right;
        }
    }
    return ROOT;
}


//Melhorado
Book* search_and_info_book(Book *root, char ISBN[14]){
    if (ISBN == NULL || ISBN[0] == "\0"){
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
