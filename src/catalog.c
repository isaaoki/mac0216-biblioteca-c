#include "../include/catalog.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Book *insert_Book(Book *root, char *ISBN){
    if (root == 0){
        root = malloc(sizeof(Book));
        root->value = value;
        root->left = 0;
        root->right = 0;
        return root;
    }

    Book* root_original = root;

    while(root != 0){
        if (value < root->value){
            if (root->left == 0){
                root->left = malloc(sizeof(Book));
                if (root->left == 0){
                    printf("Falta de memória.\n");
                    return NULL;
                }
                root = root->left;
                root->left = 0;
                root->right = 0;
                root->value = value;
                return root_original;
            }
            root = root->left;
        }
        else if (value >= root->value){
            if (root->right == 0){
                root->right = malloc(sizeof(Book));
                if (root->right == 0){
                    printf("Falta de memória.\n");
                    return NULL;
                }
                root = root->right;
                root->right = 0;
                root->left = 0;
                root->value = value;
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
Book* search_and_info_book(Book *root, char *ISBN){
    if (ISBN == NULL || ISBN[0] == "\0"){
        return NULL;
    }
    while (root != NULL){
        int pos = strcmp(root->isbn, ISBN);
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
