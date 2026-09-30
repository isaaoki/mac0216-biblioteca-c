#include <stdio.h>
#include <stdlib.h>
#include "bst.h"


int main(){

    Node* root = insert_value(NULL, 10);
    insert_value(root, 30);
    insert_value(root, 30);
    insert_value(root, 1);
    insert_value(root, 12);
    insert_value(root, 5);
    printf("%d",search_value(root, 10)->value);
    printf("%d",search_value(root, 12)->value);
    remove_value(root, 12);
    if (search_value(root, 12) != NULL) {
        printf("%d", search_value(root, 12)->value);
    }
    else {
        printf("12 nao foi encontrado\n");
    }
    return 0;
}