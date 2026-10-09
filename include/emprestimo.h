#ifndef EMPRESTIMO_H
#define EMPRESTIMO_H

#include "usuario.h"
#include "catalog.h"


typedef struct Emprestimo {
    unsigned int user_id;
    char isbn[14];
    int prazo;
    time_t data_emprestimo;
    time_t data_devolucao;
    struct Emprestimo *prox;
} emprestimo;

emprestimo *emprestimo_create(void);

int emprestar_livro(emprestimo *cab, UserConj *usuarios, Book *book, unsigned int id, const char *isbn);

int devolver_livro(emprestimo *cab, UserConj *usuarios, Book *book, unsigned int id);

int aumentar_prazo(emprestimo *cab, unsigned int id,int dias);

int aplicar_penalidade(emprestimo *usuario, int dias_atraso);

void emprestimo_destroy(emprestimo *cab);

#endif