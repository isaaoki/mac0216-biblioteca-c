#include "../include/persistencia.h"

UserConj* carrega_usuarios() {
    FILE *f;

    f = fopen("users.txt", "r");
    if (f == NULL) {
        printf("An error occured while opening users.txt\n");
        return NULL; 
    }

    // Cria conjunto de usuários
    UserConj* usuarios = cria_conj_usuarios(16);
    char nome[100];
    int status;

    while(fscanf(f, " %99[^;];%d", nome, &status) == 2) {
        UserStatus s;

        if (status == 1) s = USER_OK;
        else s = USER_BLOCKED;
        
        cria_usuario(usuarios, nome, s);
    }

    fclose(f);
    return usuarios;
}

int salva_usuarios(UserConj* usuarios) {
    FILE *f;

    f = fopen("users.txt", "w");
    if (f == NULL) {
        printf("An error occured while opening users.txt\n");
        return -1; 
    }

    // Percorrendo conjunto de usuários
    for (int i = 0; i < usuarios->capacidade; i++) {
        UserNode *u = usuarios->users[i];

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

Livro* carrega_catalogo() {
    FILE *f;

    f = fopen("books.txt", "r");
    if (f == NULL) {
        printf("An error occured while opening book.txt\n");
        return NULL; 
    }

    // Cria raíz da árvore
    Livro *root = NULL;
    Livro book; 
    int disp, tot;

    while(fscanf(f, " %13[^;];%49[^;];%49[^;];%d;%d;%d", book.isbn, book.nome, book.autor, &book.ano, &disp, &tot) == 6) {
        root = insere_livro(root, book.isbn, book.nome, book.autor, book.ano);

        // Após inserir, muda copias disponiveis e totais
        Livro* b = busca_livro(root, book.isbn);
        if (b) {
            b->copias_disponiveis = disp;
            b->copias_totais = tot;
        }
    }

    fclose(f);
    return root;
}

void salva_catalogo_aux(Livro *root, FILE *f) {
    // Caso base: quando árvore está vazia
    if (root == NULL) return;

    salva_catalogo_aux(root->left, f); // Percorre nós à esquerda
    fprintf(f, "%s;%s;%s;%d;%d;%d\n",
            root->isbn,
            root->nome,
            root->autor,
            root->ano,
            root->copias_disponiveis,
            root->copias_totais);
    salva_catalogo_aux(root->right, f); // Percorre nós à direita
}

int salva_catalogo(Livro* catalogo) {
    FILE *f;

    f = fopen("books.txt", "w");
    if (f == NULL) {
        printf("An error occured while opening books.txt\n");
        return -1; 
    }

    // Chama função recursiva que percorre a árvore
    salva_catalogo_aux(catalogo, f);

    fclose(f);
    return 0;
}