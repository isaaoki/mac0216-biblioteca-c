#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../include/emprestimo.h"

struct tm *data; 

Emprestimo *criar_emprestimo(void){
    Emprestimo *cab = malloc(sizeof(Emprestimo));
    if (cab == NULL){
        printf("ERRO! Não foi possível alocar memória.");
        return NULL;
    }
    else{
        cab->prox = NULL;
        return cab;
    }
}


int emprestar_livro(Emprestimo *cab, UserConj *usuarios, Livro *book, unsigned int id, const char *isbn){
    User *usuario = busca_usuario(usuarios, id);
    if (usuario == NULL){
        printf("Empréstimo negado!\n Usuário não encontrado.\n");
        return 0;
    }
    else{
        if(verifica_emprestimo(usuario)== 1){
            if(book->copias_disponiveis > 0){
                Emprestimo *empret = malloc(sizeof(Emprestimo));
                if (empret == NULL){
                    printf("ERRO! Não foi possível alocar memória para novo empréstimo.");
                    return 0;
                }
                else{
                    book->copias_disponiveis--;
                    atualiza_status_usuario(usuario, 0);

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

int devolver_livro(Emprestimo *cab, UserConj *usuarios, Livro *book, unsigned int id){
    Emprestimo *p, *q;
    p = cab;
    q = p->prox;
    while (q!= NULL && q->user_id != id){
        p= q;
        q= q->prox;
    }

    if(q!=NULL){
        User *usuario = busca_usuario(usuarios, q->user_id);
        if (usuario == NULL){
            printf("Devolução não realizada!\n Usuário não encontrado.\n");
            return 0;
        }
        else{
            book->copias_disponiveis++;
            atualiza_status_usuario(usuario, 1); //desbloqueia usuario

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

int aumentar_prazo(Emprestimo *cab, unsigned int id,int dias){
    Emprestimo *p, *q;
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

int aplicar_penalidade(Emprestimo *usuario, int dias_atraso){
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

void remove_emprestimos(Emprestimo *cab){
    if (cab == NULL) return;

    while(cab->prox != NULL) {
        Emprestimo *q = cab->prox;
        cab->prox = q->prox;
        free(q);
    }

    free(cab);
}

void imprime_emprestimos(Emprestimo* emprestimos) {
    Emprestimo *atual = emprestimos->prox; // Ignora sentinela

    while (atual != NULL) {
        printf("ID do usuario: %u, ISBN: %s, Prazo: %d, Data do emprestimo: %ld, Data da devolucao: %ld\n", atual->user_id, atual->isbn, atual->prazo, (long) atual->data_emprestimo, (long)atual->data_devolucao);
        atual = atual->prox;
    }
}

Emprestimo *busca_emprestimo(Emprestimo *emprestimos, unsigned int id, const char *isbn) {
    Emprestimo *temp = emprestimos->prox;
    while (temp != NULL) {
        if (temp->user_id == id && strcmp(temp->isbn, isbn) == 0) return temp;
        temp = temp->prox;
    }
    return NULL;
}