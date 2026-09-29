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

Node* remove_value(Node *root, int value){
    
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