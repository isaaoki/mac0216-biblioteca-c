#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "emprestimo.h"

Emprestimo *cria_lista_emprestimo(void){
    Emprestimo *cab = malloc(sizeof(Emprestimo));
    if (cab == NULL){
        fprintf(stderr,"[ERRO] Não foi possível alocar memória para a lista de emprestimos.\n");
        return NULL;
    }
    else{
        cab->prox = NULL;
        return cab;
    }
}


int empresta_livro(Emprestimo *cab, UserConj *usuarios, Livro *book, unsigned int id, const char *isbn){
    if (cab == NULL || usuarios == NULL || book == NULL || isbn == NULL) {
        fprintf(stderr, "[ERRO] Emprestimo negado! Parametros invalidos.\n");
        return 0;
    }

    if (strcmp(book->isbn, isbn) != 0) {
        fprintf(stderr, "[ERRO] O ISBN passado é diferente do livro passado\n");
        return 0;
    }

    User *usuario = busca_usuario(usuarios, id);
    if (usuario == NULL){
        fprintf(stderr,"[ERRO] Emprestimo negado! Usuario nao encontrado.\n");
        return 0;
    }

    if (!usuario_pode_emprestar(usuario)) {
        fprintf(stderr, "[ERRO] Emprestimo negado! Usuario esta bloqueado.\n");
        return 0;
    }

    if (book->copias_disponiveis <= 0) {
        fprintf(stderr,"[ERRO] Empréstimo negado! Livro não tem cópias disponíveis.\n");
        return 0;
    }

    Emprestimo *emprest = malloc(sizeof(Emprestimo));
    if (emprest == NULL) {
        fprintf(stderr,"[ERRO] Não foi possível alocar memória para novo empréstimo.\n");
        return 0;
    }

    emprest->user_id = id;
    strncpy(emprest->isbn, isbn, sizeof(emprest->isbn) - 1);
    emprest->isbn[sizeof(emprest->isbn) - 1] = '\0';
    emprest->prazo = 7;
    emprest->data_emprestimo = time(NULL);
    emprest->data_devolucao = emprest->data_emprestimo + (7 * 24 * 60 * 60);
    emprest->prox = cab->prox;
    cab->prox = emprest;

    book->copias_disponiveis--;
    atualiza_status_usuario(usuario, USER_BLOCKED);
    return 1;
}

int devolve_livro(Emprestimo *cab, UserConj *usuarios, Livro *book, unsigned int id, int *multa){
    if (cab == NULL || usuarios == NULL || book == NULL) {
        fprintf(stderr, "[ERRO] Devolucao nao realizada! Parametros invalidos.\n");
        return 0;
    }

    // Inicialmente, configura multa como zero.
    if (multa != NULL) *multa = 0;

    Emprestimo *p, *q;
    p = cab;
    q = p->prox;
    while (q != NULL && !(q->user_id == id && strcmp(q->isbn, book->isbn) == 0)){
        p = q;
        q = q->prox;
    }

    if (q == NULL) {
        fprintf(stderr,"[ERRO] Devolução não realizada! Empréstimo não encontrado.\n");
        return 0;
    }
    
    User *usuario = busca_usuario(usuarios, q->user_id);
    if (usuario == NULL){
        fprintf(stderr,"[ERRO] Devolução não realizada! Usuário não encontrado.\n");
        return 0;
    }
    
    book->copias_disponiveis++;
    atualiza_status_usuario(usuario, USER_OK); //desbloqueia usuario

    time_t devolvido = time(NULL);
    double diferenca = difftime(devolvido, q->data_emprestimo);
    int dias = diferenca/(60*60*24);
    if (dias > q->prazo){
        *multa = calcula_multa(q, dias);
    }

    p->prox = q->prox;
    free(q);
    return 1;
}

int aumenta_prazo(Emprestimo *cab, unsigned int id, int dias){
    if (cab == NULL || dias < 0) {
        fprintf(stderr, "[ERRO] Extensao de prazo negada! Parametros invalidos.\n");
        return 0;
    }

    Emprestimo *p, *q;
    p = cab;
    q = p->prox;
    while (q!= NULL && q->user_id != id){
        p= q;
        q= q->prox;
    }

    if (q!=NULL){
        q->prazo+=dias;
        q->data_devolucao += (dias * 24 * 60 * 60);
        return 1;
    }
    else{
        fprintf(stderr,"[ERRO] Empréstimo não encontrado! Extensão do prazo não realizado.\n");
        return 0;
    }
}

int calcula_multa(const Emprestimo *usuario, int dias_totais){
    int dias_atrasado = dias_totais - usuario->prazo;
    if (dias_atrasado < 0){
        fprintf(stderr,"[ERRO] Penalidade não aplicada! Dias de atraso negativo.\n");
        return 0;
    }
    else{
        int multa = dias_atrasado * 2;
        return multa;
    }
}

void libera_lista_emprestimos(Emprestimo *cab){
    if (cab == NULL) return;

    while(cab->prox != NULL) {
        Emprestimo *q = cab->prox;
        cab->prox = q->prox;
        free(q);
    }

    free(cab);
}

void imprime_emprestimos(Emprestimo* emprestimos) {
    if (emprestimos == NULL) return;

    Emprestimo *atual = emprestimos->prox; // Ignora sentinela
    while (atual != NULL) {
        printf("ID do usuario: %u, ISBN: %s, Prazo: %d dias, ", atual->user_id, atual->isbn, atual->prazo);
        struct tm *data; 

        data = localtime(&atual->data_emprestimo);
        char data_formatada[20];
        strftime(data_formatada, sizeof(data_formatada), "%d/%m/%Y", data);            

        data = localtime(&atual->data_devolucao);
        char data_retorno[20];
        strftime(data_retorno, sizeof(data_retorno), "%d/%m/%Y", data);

        printf("Data do emprestimo: %s, Data da devolucao: %s\n", data_formatada, data_retorno);

        atual = atual->prox;
    }
}

Emprestimo *busca_emprestimo(Emprestimo *emprestimos, unsigned int id, const char *isbn) {
    if (emprestimos == NULL || isbn == NULL) {
        fprintf(stderr, "[ERRO] Busca do emprestimo com parametros invalidos.\n");
        return NULL;
    }

    Emprestimo *temp = emprestimos->prox;
    while (temp != NULL) {
        if (temp->user_id == id && strcmp(temp->isbn, isbn) == 0) return temp;
        temp = temp->prox;
    }
    
    return NULL;
}