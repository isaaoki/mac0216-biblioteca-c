#include "persistencia.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

UserConj* carrega_usuarios(const char *caminho) {
    FILE *f;

    f = fopen(caminho, "r");
    if (f == NULL) {
        fprintf(stderr, "[ERRO] Erro ocorreu na abertura do arquivo %s\n", caminho);
        return NULL; 
    }

    // Cria conjunto de usuários
    UserConj* usuarios = cria_conj_usuarios(16);
    if (usuarios == NULL) {
        fclose(f);
        return NULL;
    }

    unsigned int id;
    char nome[100];
    int status;

    while(fscanf(f, " %u;%99[^;];%d", &id, nome, &status) == 3) {
        UserStatus s;

        if (status == 1) s = USER_OK;
        else if (status == 0) s = USER_BLOCKED;
        else {
            fprintf(stderr, "[AVISO] Usuario com id %d e nome %s tem status invalido (diferente de 0 e 1 no arquivo), colocando ele como bloqueado\n", id, nome);
            s = USER_BLOCKED;
        }

        int resultado = cria_usuario_com_id(usuarios, id, nome, s);
        if (resultado == 0) {
            fprintf(stderr, "[AVISO] Erro ao criar o usuario com id %d e nome %s, pulando esse registro\n", id, nome);
        }
    }

    fclose(f);
    return usuarios;
}

int salva_usuarios(UserConj* usuarios, const char *caminho) {
    if (usuarios == NULL || caminho == NULL) {
        fprintf(stderr, "[ERRO] Parametros invalidos ao salvar usuarios.\n");
        return 0;
    }

    FILE *f;

    f = fopen(caminho, "w");
    if (f == NULL) {
        fprintf(stderr, "[ERRO] Erro ocorreu na abertura do arquivo %s\n", caminho);
        return 0; 
    }

    // Percorrendo conjunto de usuários
    for (size_t i = 0; i < usuarios->capacidade; i++) {
        UserNode *u = usuarios->users[i];

        while (u != NULL) {
            User* atual = u->user;
            
            // Salva usuário no arquivo
            fprintf(f, "%u;%s;%d\n", atual->id, atual->nome, atual->status == USER_OK);
            u = u->prox;
        }
    }
    
    fclose(f);
    return 1;
}

Livro* carrega_catalogo(const char *caminho) {
    FILE *f;

    f = fopen(caminho, "r");
    if (f == NULL) {
        fprintf(stderr, "[ERRO] Erro ocorreu na abertura do arquivo %s\n", caminho);
        return NULL; 
    }

    // Cria raíz da árvore
    Livro *root = NULL;
    char isbn[14], nome[50], autor[50];
    int ano, disp, tot;

    while(fscanf(f, " %13[^;];%49[^;];%49[^;];%d;%d;%d", isbn, nome, autor, &ano, &disp, &tot) == 6) {
        root = insere_livro(root, isbn, nome, autor, ano);

        // Após inserir, muda copias disponiveis e totais
        Livro* b = busca_livro(root, isbn);
        if (b) {
            b->copias_disponiveis = disp;
            b->copias_totais = tot;
        }
    }

    fclose(f);
    return root;
}

static void salva_catalogo_aux(Livro *root, FILE *f) {
    // Caso base: quando árvore está vazia
    if (root == NULL) return;

    fprintf(f, "%s;%s;%s;%d;%d;%d\n",
        root->isbn,
        root->nome,
        root->autor,
        root->ano,
        root->copias_disponiveis,
        root->copias_totais);
    salva_catalogo_aux(root->left, f); // Percorre nós à esquerda
    salva_catalogo_aux(root->right, f); // Percorre nós à direita
}

int salva_catalogo(Livro* catalogo, const char *caminho) {
    FILE *f;

    f = fopen(caminho, "w");
    if (f == NULL) {
        fprintf(stderr, "[ERRO] Erro ocorreu na abertura do arquivo %s\n", caminho);
        return 0; 
    }

    // Chama função recursiva que percorre a árvore
    salva_catalogo_aux(catalogo, f);

    fclose(f);
    return 1;
}

Emprestimo* carrega_emprestimos(const char *caminho) {
    FILE *f;

    f = fopen(caminho, "r");
    if (f == NULL) {
        fprintf(stderr, "[ERRO] Erro ocorreu na abertura do arquivo %s\n", caminho);
        return NULL; 
    }

    Emprestimo *cab = cria_lista_emprestimo();
    if (cab == NULL) {
        fprintf(stderr, "[ERRO] Erro na alocação de memória dos empréstimos\n");
        fclose(f);
        libera_lista_emprestimos(cab);
        return NULL;
    }

    unsigned int user_id;
    char isbn[14];
    int prazo;
    long data_emprestimo, data_devolucao;

    while (fscanf(f, " %u;%13[^;];%d;%ld;%ld", &user_id, isbn, &prazo, &data_emprestimo, &data_devolucao) == 5) {
        Emprestimo *novo = malloc(sizeof(Emprestimo));

        if (novo == NULL) {
            fprintf(stderr, "[ERRO] Falha ao alocar memoria para emprestimos\n");
            fclose(f);
            libera_lista_emprestimos(cab);
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
    if (cab == NULL || caminho == NULL) {
        fprintf(stderr, "[ERRO] Parametros invalidos ao salvar emprestimos.\n");
        return 0;
    }
    
    FILE *f;

    f = fopen(caminho, "w");
    if (f == NULL) {
        fprintf(stderr, "[ERRO] Erro ocorreu na abertura do arquivo %s\n", caminho);
        return 0; 
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
    return 1;
}