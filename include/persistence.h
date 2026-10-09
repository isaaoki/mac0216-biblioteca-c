#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "usuario.h"
#include "catalogo.h"
#include <stdio.h>

/**
 * @brief Lê usuários de arquivo users.txt e monta conjunto de usuários.
 * @returns Ponteiro para conjunto de usuários ou NULL se não teve sucesso.
 * @pre Arquivo users.txt armazena, em cada linha, nome;status, nessa ordem, sendo status 0 para USER_OK e 1 para USER_BLOCKED.
 * @post Conjunto de usuários armazena em memória os usuários do arquivo.
 * @note Caso o arquivo users.txt não exista, a função exibe mensagem na tela e retorna NULL.
 */
UserConj* user_load();

/**
 * @brief Salva conjunto de usuários no arquivo users.txt.
 * @param users Conjunto de usuários.
 * @returns 0 se teve sucesso, -1 se não teve sucesso.
 * @pre Conjunto de usuários válido na memória.
 * @post Arquivo users.txt contém os usuários do conjunto, sendo que cada linha possui nome;status.
 * @note Caso não seja possível abrir ou escrever no arquivo, a função exibe mensagem na tela e retorna -1.
 */
int user_save(UserConj* users);

/**
 * @brief Lê livros de arquivo books.txt e monta árvore com livros.
 * @returns Ponteiro para raíz da árvore com livros ou NULL se não teve sucesso.
 * @pre Arquivo books.txt armazena, em cada linha, isbn;nome;autor;ano;copias_disponiveis;copias_totais, nessa ordem.
 * @post Livros do arquivo são armazenados em memória em uma árvore.
 * @note Caso o arquivo books.txt não exista, a função exibe mensagem na tela e retorna NULL.
 */
Livro* catalog_load();

/**
 * @brief Salva livros no arquivo books.txt.
 * @param catalog Raíz da árvore que armazena os livros.
 * @returns 0 se teve sucesso, -1 se não teve sucesso.
 * @pre catalog aponta para a raiz da árvore de livros ou é NULL.
 * @post Arquivo books.txt contém os livros da árvore, sendo que cada linha possui isbn;nome;autor;ano;copias_disponiveis;copias_totais.
 * @note Caso não seja possível abrir ou escrever no arquivo, a função exibe mensagem na tela e retorna -1.
 */
int catalog_save(Livro *catalog);

/**
 * @brief Percorre a árvore recursivamente e armazena os livros no arquivo.
 * @param root Nó atual da árvore.
 * @param f Arquivo onde os livros serão armazenados.
 * @returns void.
 * @pre f aponta para um arquivo aberto para escrita.
 * @post Os livros da subárvore de root são armazenados no arquivo apontado por f.
 */
void catalog_save_aux(Livro *root, FILE *f);

#endif