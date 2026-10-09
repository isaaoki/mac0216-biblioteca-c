#include "../include/persistence.h"

UserConj* user_load() {
    FILE *f;

    f = fopen("users.txt", "r");
    if (f == NULL) {
        printf("An error occured while opening users.txt\n");
        return NULL; 
    }

    // Cria conjunto de usuários
    UserConj* usuarios = user_conj_create(16);
    char nome[100];
    int status;

    while(fscanf(f, " %99[^;];%d", nome, &status) == 2) {
        UserStatus s;

        if (status == 1) s = USER_OK;
        else s = USER_BLOCKED;
        
        user_register(usuarios, nome, s);
    }

    fclose(f);
    return usuarios;
}

int user_save(UserConj* users) {
    FILE *f;

    f = fopen("users.txt", "w");
    if (f == NULL) {
        printf("An error occured while opening users.txt\n");
        return -1; 
    }

    // Percorrendo conjunto de usuários
    for (int i = 0; i < users->capacidade; i++) {
        UserNode *u = users->users[i];

        while (u != NULL) {
            User* atual = u->user;
            
            // Salva usuário no arquivo
            if (atual->status == USER_OK) fprintf(f, "%s;%d\n", atual->nome, 0);
            else fprintf(f, "%s;%d\n", atual->nome, 1);

            u = u->prox;
        }
    }
    
    fclose(f);
    return 0;
}

Book* catalog_load() {
    FILE *f;

    f = fopen("books.txt", "r");
    if (f == NULL) {
        printf("An error occured while opening book.txt\n");
        return NULL; 
    }

    // Cria raíz da árvore
    Book *root = NULL;
    Book book; 
    int disp, tot;

    while(fscanf(f, " %13[^;];%49[^;];%49[^;];%d;%d;%d", book.isbn, book.nome, book.autor, &book.ano, &disp, &tot) == 6) {
        root = insert_Book(root, book.isbn, book.nome, book.autor, book.ano);

        // Após inserir, muda copias disponiveis e totais
        Book* b = search_and_info_Book(root, book.isbn);
        if (b) {
            b->copias_disponiveis = disp;
            b->copias_totais = tot;
        }
    }

    fclose(f);
    return root;
}

int catalog_save(Book* catalog) {
    FILE *f;

    f = fopen("books.txt", "w");
    if (f == NULL) {
        printf("An error occured while opening books.txt\n");
        return -1; 
    }

    // Chama função recursiva que percorre a árvore
    catalog_save_aux(catalog, f);

    fclose(f);
    return 0;
}

void catalog_save_aux(Book *root, FILE *f) {
    // Caso base: quando árvore está vazia
    if (root == NULL) return;

    catalog_save_aux(root->left, f); // Percorre nós à esquerda
    fprintf(f, "%s;%s;%s;%d;%d;%d\n",
            root->isbn,
            root->nome,
            root->autor,
            root->ano,
            root->copias_disponiveis,
            root->copias_totais);
    catalog_save_aux(root->right, f); // Percorre nós à direita
}