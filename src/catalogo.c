#include "catalogo.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Livro *insere_livro(Livro *root, const char *isbn, const char *nome, const char *autor, int ano) {
    // Verifica condições do livro
    if (isbn == NULL || nome == NULL || autor == NULL) {
        fprintf(stderr, "[ERRO] Dados invalidos.\n");
        return root;
    }

    if (isbn[0] == '\0' || nome[0] == '\0' || autor[0] == '\0') {
        fprintf(stderr, "[ERRO] ISBN, nome e autor nao pode ser vazio.\n");
        return root;
    }

    if (strpbrk(isbn, ";\r\n") != NULL || strpbrk(nome, ";\r\n") != NULL || strpbrk(autor, ";\r\n") != NULL) {
        fprintf(stderr, "[ERRO] Texto nao pode conter ';' nem quebra de linha.\n");
        return root;
    }


    if (strlen(isbn) > 13 || strlen(nome) > 49 || strlen(autor) > 49) {
        fprintf(stderr, "[ERRO] Texto maior que o permitido.\n");
        return root;
    }

    if (ano < 0) {
        fprintf(stderr,"[ERRO] Ano invalido (menor do que zero).\n");
        return root;
    }

    // Caso em que não existe raíz
    if (root == NULL) {
        root = malloc(sizeof(Livro));
        if (root == NULL) {
            fprintf(stderr,"[ERRO] Falta de memoria.\n");
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

    Livro* root_original = root;
    int pos;

    while (1) {
        pos = strcmp(isbn, root->isbn);

        // 1. Encontrou o mesmo livro, aumenta as cópias
        if (pos == 0) {
            root->copias_totais++;
            root->copias_disponiveis++;
            return root_original;
        } else if (pos < 0) { // 2: isbn está a esquerda do nó atual
            // 2.1. Chegou nas folhas
            if (root->left == NULL) {
                root->left = malloc(sizeof(Livro));
                if (root->left == NULL){
                    fprintf(stderr,"[ERRO] Falta de memoria.\n");
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
            // 2.2. Vai para a esquerda da árvore
            root = root->left; 
        } else if (pos > 0) { // 3. isbn está a direita do nó atual
            // 3.1. Chegou nas folhas
            if (root->right == NULL) {
                root->right = malloc(sizeof(Livro));
                if (root->right == NULL) {
                    fprintf(stderr, "[ERRO] Falta de memoria.\n");
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
            // 3.2. Vai para a direita da árvore
            root = root->right;
        }
    }
}

Livro *remove_livro(Livro *root, const char *isbn) {
    Livro *atual = root; // Ponteiro que navega pela árvore
    Livro *father = NULL; // Ponteiro para o pai do nó atual

    // Validação do ISBN
    if (isbn == NULL){
        fprintf(stderr, "[ERRO] ISBN invalido.\n");
        return root;
    }
    
    while (atual != NULL) {
        int pos = strcmp(isbn, atual->isbn);
        // 1. Encontra o livro na árvore
        if (pos == 0) {
            // Caso o livro a ser deletado seja a raiz
            if (father == NULL) {
                // A raíz não tem filhos
                if (atual->left == NULL && atual->right == NULL) {
                    root = NULL;
                } else if (atual->left != NULL && atual->right == NULL) { // A raíz tem apenas filho à esquerda
                    root = atual->left;
                } else if (atual->left == NULL && atual->right != NULL) { // A raíz tem apenas filho à direita
                    root = atual->right;
                } else { // A raíz tem dois filhos
                    root = atual->left;
                    Livro *substitute = atual->left;
                    while (substitute->right != NULL) {
                        substitute = substitute->right;
                    }
                    substitute->right = atual->right;
                }

                free(atual);
                return root;
            }

            // Caso o livro a ser deletado é o filho à direita
            if (atual == father->right) {
                if (atual->left == NULL && atual->right == NULL) {
                    father->right = NULL;
                } else if (atual->left == NULL && atual->right != NULL) {
                    father->right = atual->right;
                } else if (atual->left != NULL && atual->right == NULL) {
                    father->right = atual->left;
                } else {
                    father->right = atual->left;
                    Livro *substitute = atual->left;
                    while (substitute->right != NULL) {
                        substitute = substitute->right;
                    }
                    substitute->right = atual->right;
                }
            } else { // Caso o livro a ser deletado é o filho à esquerda
                if (atual->left == NULL && atual->right == NULL) {
                    father->left = NULL;
                } else if (atual->left != NULL && atual->right == NULL) {
                    father->left = atual->left;
                } else if (atual->left == NULL && atual->right != NULL) {
                    father->left = atual->right;
                } else {
                    father->left = atual->right;
                    Livro *substitute = atual->right;
                    while (substitute->left != NULL) {
                        substitute = substitute->left;
                    }
                    substitute->left = atual->left;
                }
            }

            free(atual);
            return root;
        } else if (pos < 0) { // Vai para a esquerda se isbn é menor
            father = atual;
            atual = atual->left;
        } else {
            father = atual;
            atual = atual->right;
        }
    }

    return root;
}

Livro *busca_livro(Livro *root, const char *isbn) {
    // Valida casos em que isbn é nulo ou vazio
    if (isbn == NULL || isbn[0] == '\0'){
        fprintf(stderr, "[ERRO] ISBN invalido\n");
        return NULL;
    }

    // Movimenta-se pela árvore, buscando isbn
    while (root != NULL){
        int pos = strcmp(isbn, root->isbn);
        if (pos == 0) { // Encontrou o livro
            return root;
        } else if (pos < 0) { // Caso em que isbn é menor
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return NULL;
}

void imprime_livros(Livro *root) {
    if (root == NULL) return;

    imprime_livros(root->left);
    printf("ISBN: %s, Nome: %s, Autor: %s, Ano: %d, Copias disponiveis: %d, Copias totais: %d\n", root->isbn, root->nome, root->autor, root->ano, root->copias_disponiveis, root->copias_totais);
    imprime_livros(root->right);
}

void remove_catalogo(Livro *root) {
    if (root == NULL) return;

    remove_catalogo(root->left);   
    remove_catalogo(root->right);  
    free(root);         
}
