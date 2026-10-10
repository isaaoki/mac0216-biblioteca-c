#ifndef EMPRESTIMO_H
#define EMPRESTIMO_H

#include "usuario.h"
#include "catalogo.h"

typedef struct Emprestimo {
    unsigned int user_id;
    char isbn[14];
    int prazo;
    time_t data_emprestimo;
    time_t data_devolucao;
    struct Emprestimo *prox;
} Emprestimo;

Emprestimo *criar_emprestimo(void);

int emprestar_livro(Emprestimo *cab, UserConj *usuarios, Livro *book, unsigned int id, const char *isbn);

int devolver_livro(Emprestimo *cab, UserConj *usuarios, Livro *book, unsigned int id);

int aumentar_prazo(Emprestimo *cab, unsigned int id,int dias);

int aplicar_penalidade(Emprestimo *usuario, int dias_atraso);

void remover_emprestimo(Emprestimo *cab);

#endif