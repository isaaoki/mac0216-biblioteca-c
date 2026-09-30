#include "bst.h"
#include <stdlib.h>
#include <stdio.h>

Node *insert_value(Node *root, int value){
    if (root == 0){
        root = malloc(sizeof(Node));
        root->value = value;
        root->left = 0;
        root->right = 0;
        return root;
    }

    Node* root_original = root;

    while(root != 0){
        if (value < root->value){
            if (root->left == 0){
                root->left = malloc(sizeof(Node));
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
                root->right = malloc(sizeof(Node));
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

Node *remove_value(Node *root, int value){
    Node* ROOT = root;
    Node* father = 0;

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
                    Node *substitute = root->left;
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
                    Node* substitute = root->left;
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
                    Node* substitute = root->right;
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

Node* search_value(Node *root, int value){
    while (root != 0){
        if (root->value == value){
            return root;
        }
        else if (value < root->value){
            root = root->left;
        }
        else{
            root = root->right;
        }
    }
    return NULL;
}
