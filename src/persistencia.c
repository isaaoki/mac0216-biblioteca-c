#include "../include/persistencia.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

UserConj* carrega_usuarios(const char *caminho) {
    FILE *f;

    f = fopen(caminho, "r");
    if (f == NULL) {
        printf("[ERRO] Erro ocorreu na abertura do arquivo %s\n", caminho);
        return NULL; 
    }

    // Cria conjunto de usuários
    UserConj* usuarios = cria_conj_usuarios(16);
    if (usuarios == NULL) {
        printf("[ERRO] Erro na alocação de memória dos usuários\n");
        fclose(f);
        remove_conj_usuarios(usuarios);
        return NULL;
    }

    char nome[100];
    int status;

    while(fscanf(f, " %99[^;];%d", nome, &status) == 2) {
        UserStatus s;

        if (status == 0) s = USER_OK;
        else s = USER_BLOCKED;
        
        cria_usuario(usuarios, nome, s);
    }

    fclose(f);
    return usuarios;
}

int salva_usuarios(UserConj* usuarios, const char *caminho) {
    FILE *f;

    f = fopen(caminho, "w");
    if (f == NULL) {
        printf("[ERRO] Erro ocorreu na abertura do arquivo %s\n", caminho);
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

Livro* carrega_catalogo(const char *caminho) {
    FILE *f;

    f = fopen(caminho, "r");
    if (f == NULL) {
        printf("[ERRO] Erro ocorreu na abertura do arquivo %s\n", caminho);
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

int salva_catalogo(Livro* catalogo, const char *caminho) {
    FILE *f;

    f = fopen(caminho, "w");
    if (f == NULL) {
        printf("[ERRO] Erro ocorreu na abertura do arquivo %s\n", caminho);
        return -1; 
    }

    // Chama função recursiva que percorre a árvore
    salva_catalogo_aux(catalogo, f);

    fclose(f);
    return 0;
}

Emprestimo* carrega_emprestimos(const char *caminho) {
    FILE *f;

    f = fopen(caminho, "r");
    if (f == NULL) {
        printf("[ERRO] Erro ocorreu na abertura do arquivo %s\n", caminho);
        return NULL; 
    }

    Emprestimo *cab = criar_emprestimo();
    if (cab == NULL) {
        printf("[ERRO] Erro na alocação de memória dos empréstimos\n");
        fclose(f);
        remove_emprestimos(cab);
        return NULL;
    }

    unsigned int user_id;
    char isbn[14];
    int prazo;
    long data_emprestimo, data_devolucao;

    while (fscanf(f, "%u;%13[^;];%d;%ld;%ld", &user_id, isbn, &prazo, &data_emprestimo, &data_devolucao) == 5) {
        Emprestimo *novo = malloc(sizeof(Emprestimo));

        if (novo == NULL) {
            fclose(f);
            return NULL;
        }

        novo->user_id = user_id;
        strcpy(novo->isbn, isbn);
        novo->prazo = prazo;
        novo->data_emprestimo = (time_t)data_emprestimo;
        novo->data_devolucao = (time_t)data_devolucao;


        novo->prox = cab->prox;
        cab->prox = novo;
    }

    fclose(f);
    return cab;
}

int salva_emprestimos(Emprestimo* cab, const char *caminho) {
    FILE *f;

    f = fopen(caminho, "w");
    if (f == NULL) {
        printf("[ERRO] Erro ocorreu na abertura do arquivo %s\n", caminho);
        return -1; 
    }

    Emprestimo *atual = cab->prox;

    while (atual != NULL) {
        fprintf(f, "%u;%s;%d;%ld;%ld\n",
            atual->user_id,
            atual->isbn,
            atual->prazo,
            (long)atual->data_emprestimo,
            (long)atual->data_devolucao);
        atual = atual->prox;
    }
    
    fclose(f);
    return 0;
}