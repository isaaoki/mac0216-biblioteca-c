#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../include/emprestimo.h"


struct tm *data; 

emprestimo *emprestimo_create(void){
    emprestimo *cab = malloc(sizeof(emprestimo));
    if (cab == NULL){
        printf("ERRO! Não foi possível alocar memória.");
        return NULL;
    }
    else{
        cab->prox = NULL;
        return cab;
    }
}


int emprestar_livro(emprestimo *cab, UserConj *usuarios, Livro *book, unsigned int id, const char *isbn){
    User *usuario = user_find(usuarios, id);
    if (usuario == NULL){
        printf("Empréstimo negado!\n Usuário não encontrado.\n");
        return 0;
    }
    else{
        if(user_emprestimo(usuario)== 1){
            if(book->copias_disponiveis > 0){
                emprestimo *empret = malloc(sizeof(emprestimo));
                if (empret == NULL){
                    printf("ERRO! Não foi possível alocar memória para novo empréstimo.");
                    return 0;
                }
                else{
                    book->copias_disponiveis--;
                    user_set_status(usuario, 0);

                    empret->user_id = id;
                    strncpy(empret->isbn, isbn, sizeof(empret->isbn) - 1);
                    empret->isbn[sizeof(empret->isbn) - 1] = '\0';
                    empret->prazo = 7;
                    empret->data_emprestimo = time(NULL);
                    empret->data_devolucao = empret->data_emprestimo + (7 * 24 * 60 * 60);
                    empret->prox = cab->prox;
                    cab->prox = empret;

                    printf(
                        "Empréstimo de livro realizado com sucesso!\n"
                        "Usuário com id: %u\n"
                        "Título do livro emprestado: %s\n",
                        id,
                        book->nome
                    );

                    data = localtime(&empret->data_emprestimo);
                    char data_formatada[20];
                    strftime(data_formatada, sizeof(data_formatada), "%d/%m/%Y", data);
                    printf(
                        "Emprestado em:  %s\n"
                        "Emprestado por: %d dias\n",
                        data_formatada,
                        empret->prazo
                    );

                    data = localtime(&empret->data_devolucao);
                    char data_retorno[20];
                    strftime(data_retorno, sizeof(data_retorno), "%d/%m/%Y", data);
                    printf("Devolução em: %s\n", data_retorno);
                    return 1;
                }
            }
            else{
                printf("Empréstimo negado!\n Livro não tem cópias disponíveis.");
                return 0;
            }
        }
        else{
            printf("Empréstimo negado!\n Usuário está bloqueado.\n");
            return 0;
        } 
    }
}

int devolver_livro(emprestimo *cab, UserConj *usuarios, Livro *book, unsigned int id){
    emprestimo *p, *q;
    p = cab;
    q = p->prox;
    while (q!= NULL && q->user_id != id){
        p= q;
        q= q->prox;
    }

    if(q!=NULL){
        User *usuario = user_find(usuarios, q->user_id);
        if (usuario == NULL){
            printf("Devolução não realizada!\n Usuário não encontrado.\n");
            return 0;
        }
        else{
            book->copias_disponiveis++;
            user_set_status(usuario, 1); //desbloqueia usuario

            time_t devolvido = time(NULL);
            double diferenca = difftime(devolvido, q->data_emprestimo);
            int dias = diferenca/(60*60*24);
            if (dias > q->prazo){
                printf("Data máxima de devolução extrapolada!\n Aplicando penalidade...\n");
                int multa = aplicar_penalidade(q, dias);
                printf("Multa de: %d reais.", multa);
                printf("Devolução bem sucedida!\n");
            }
            else{
                printf("Devolução bem sucedida!\n");
            }
            p->prox = q->prox;
            free(q);
            return 1;
        }
    }
    else{
        printf("Devolução não realizada!\n Empréstimo não encontrado.\n");
        return 0;
    }
}

int aumentar_prazo(emprestimo *cab, unsigned int id,int dias){
    emprestimo *p, *q;
    p = cab;
    q = p->prox;
    while (q!= NULL && q->user_id != id){
        p= q;
        q= q->prox;
    }

    if (q!=NULL){
        q->prazo+=dias;
        printf("Seu novo prazo é: %d dias desde a data inicial do empréstimo.", q->prazo);
        q->data_devolucao += (dias * 24 * 60 * 60);
        data = localtime(&q->data_devolucao);
        char data_retorno[20];
        strftime(data_retorno, sizeof(data_retorno), "%d/%m/%Y", data);
        printf("Devolução em: %s\n", data_retorno);
        return 1;
    }
    else{
        printf("Empréstimo não encontrado!\n Extenção do prazo não realizado.\n");
        return 0;
    }
}

int aplicar_penalidade(emprestimo *usuario, int dias_atraso){
    int dias_atrasado = dias_atraso - usuario->prazo;
    if (dias_atrasado < 0){
        printf("Penalidade não aplicada!\n Dias de atraso negativo.\n");
        return 0;
    }
    else{
        int multa = dias_atrasado * 2;
        return multa;
    }
}

void emprestimo_destroy(emprestimo *cab){
    for( emprestimo *p = cab; p->prox!=NULL; p = p->prox){
        emprestimo *q= p->prox;
        p->prox = q->prox;
        free(q);
    }
    free(cab);
}