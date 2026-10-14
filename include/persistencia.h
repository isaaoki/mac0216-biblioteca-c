#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "usuario.h"
#include "catalogo.h"
#include "emprestimo.h"

/**
 * @brief Lê usuários de arquivo usuarios.txt e monta conjunto de usuários.
 * @returns Ponteiro para conjunto de usuários ou NULL se não teve sucesso.
 * @pre Arquivo usuarios.txt armazena, em cada linha, nome;status, nessa ordem, sendo status 0 para USER_OK e 1 para USER_BLOCKED.
 * @post Conjunto de usuários armazena em memória os usuários do arquivo.
 * @note Caso o arquivo usuarios.txt não exista, a função exibe mensagem na tela e retorna NULL.
 */
UserConj* carrega_usuarios();

/**
 * @brief Salva conjunto de usuários no arquivo usuarios.txt.
 * @param usuarios Conjunto de usuários.
 * @returns 0 se teve sucesso, -1 se não teve sucesso.
 * @pre Conjunto de usuários válido na memória.
 * @post Arquivo usuarios.txt contém os usuários do conjunto, sendo que cada linha possui nome;status.
 * @note Caso não seja possível abrir ou escrever no arquivo, a função exibe mensagem na tela e retorna -1.
 */
int salva_usuarios(UserConj* usuarios);

/**
 * @brief Lê livros de arquivo livros.txt e monta árvore com livros.
 * @returns Ponteiro para raíz da árvore com livros ou NULL se não teve sucesso.
 * @pre Arquivo livros.txt armazena, em cada linha, isbn;nome;autor;ano;copias_disponiveis;copias_totais, nessa ordem.
 * @post Livros do arquivo são armazenados em memória em uma árvore.
 * @note Caso o arquivo livros.txt não exista, a função exibe mensagem na tela e retorna NULL.
 */
Livro* carrega_catalogo();

/**
 * @brief Salva livros no arquivo livros.txt.
 * @param catalogo Raíz da árvore que armazena os livros.
 * @returns 0 se teve sucesso, -1 se não teve sucesso.
 * @pre catalog aponta para a raiz da árvore de livros ou é NULL.
 * @post Arquivo livros.txt contém os livros da árvore, sendo que cada linha possui isbn;nome;autor;ano;copias_disponiveis;copias_totais.
 * @note Caso não seja possível abrir ou escrever no arquivo, a função exibe mensagem na tela e retorna -1.
 */
int salva_catalogo(Livro *catalogo);

/**
 * @brief Lê emprestimos de arquivo emprestimos.txt e monta conjunto de emprestimos
 * @returns Ponteiro para o primeiro emprestimo ou NULL se não teve sucesso.
 * @pre Arquivo emprestimos.txt armazena, em cada linha, id_usuario;isbn;prazo;data_emprestimo;data_devolucao, nessa ordem,
 * sendo que as datas são armazenadas como long.
 * @post Emprestimos do arquivo são armazenados em memória de execução.
 * @note Caso o arquivo emprestimos.txt não exista, a função exibe mensagem na tela e retorna NULL.
 */
Emprestimo* carrega_emprestimos();

/**
 * @brief Salva emprestimos no arquivo emprestimos.txt.
 * @param cab Ponteiro para sentinela de empréstimos, 
 * cab->prox é o primeiro empréstimo.
 * @returns 0 se teve sucesso, -1 se não teve sucesso.
 * @pre cab não é NULL e cab->prox aponta para o primeiro emprestimo.
 * @post Arquivo emprestimos.txt contém os emprestimos, sendo que cada linha possui id_usuario;isbn;prazo;data_emprestimo;data_devolucao, nessa ordem.
 * @note Caso não seja possível abrir ou escrever no arquivo, a função exibe mensagem na tela e retorna -1.
 */
int salva_emprestimos(Emprestimo* cab);

#endif