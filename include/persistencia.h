#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "usuario.h"
#include "catalogo.h"
#include "emprestimo.h"

/**
 * @brief Lê usuários de um arquivo e monta conjunto de usuários.
 * @param caminho Caminho do arquivo a ser lido.
 * @returns Ponteiro para conjunto de usuários ou NULL se não teve sucesso.
 * @pre Arquivo indicado por caminho é .txt e armazena, em cada linha, 
 * nome;status, nessa ordem, sendo status 0 para USER_OK e 1 para USER_BLOCKED.
 * @post Conjunto de usuários armazena em memória os usuários do arquivo.
 * @note Caso o arquivo não exista, a função exibe mensagem na tela e retorna NULL.
 */
UserConj* carrega_usuarios(const char *caminho);

/**
 * @brief Salva conjunto de usuários em um arquivo.
 * @param usuarios Conjunto de usuários.
 * @param caminho Caminho do arquivo a ser escrito.
 * @returns 0 se teve sucesso, -1 se não teve sucesso.
 * @pre Conjunto de usuários válido na memória.
 * @post Arquivo indicado por caminho contém os usuários do conjunto, sendo que cada linha possui nome;status.
 * @note Caso não seja possível abrir ou escrever no arquivo, a função exibe mensagem na tela e retorna -1.
 * @note O conjunto de usuários deve ser liberados com a função remove_conj_usuarios.
 */
int salva_usuarios(UserConj *usuarios, const char *caminho);

/**
 * @brief Lê livros de arquivo e monta árvore com livros.
 * @param caminho Caminho do arquivo a ser lido.
 * @returns Ponteiro para raíz da árvore com livros ou NULL se não teve sucesso.
 * @pre Arquivo indicado armazena, em cada linha, isbn;nome;autor;ano;copias_disponiveis;copias_totais, nessa ordem.
 * @post Livros do arquivo são armazenados em memória em uma árvore.
 * @note Caso o arquivo indicado não exista, a função exibe mensagem na tela e retorna NULL.
 * @note O conjunto de livros deve ser liberados com a função remove_catalogo.
 */
Livro* carrega_catalogo(const char *caminho);

/**
 * @brief Salva livros no arquivo indicado.
 * @param catalogo Raíz da árvore que armazena os livros.
 * @param caminho Caminho do arquivo a ser lido.
 * @returns 0 se teve sucesso, -1 se não teve sucesso.
 * @pre catalogo aponta para a raiz da árvore de livros ou é NULL.
 * @post Arquivo indicado contém os livros da árvore, sendo que cada linha possui 
 * isbn;nome;autor;ano;copias_disponiveis;copias_totais.
 * @note Caso não seja possível abrir ou escrever no arquivo, a função exibe mensagem na tela e retorna -1.
 */
int salva_catalogo(Livro *catalogo, const char *caminho);

/**
 * @brief Lê emprestimos de arquivo e monta conjunto de emprestimos
 * @param caminho Caminho do arquivo a ser lido.
 * @returns Ponteiro para o sentinela de emprestimo, sendo que o próximo é o primeiro empréstimo, 
 * ou NULL se não teve sucesso.
 * @pre Arquivo indicado por caminho armazena, em cada linha, id_usuario;isbn;prazo;data_emprestimo;data_devolucao, nessa ordem,
 * sendo que as datas são armazenadas como long.
 * @post Emprestimos do arquivo são armazenados em memória de execução.
 * @note Caso o arquivo indicado não exista, a função exibe mensagem na tela e retorna NULL.
 * @note O conjunto de empréstimos deve ser liberados com a função emprestimo_destroy.
 */
Emprestimo* carrega_emprestimos(const char *caminho);

/**
 * @brief Salva emprestimos no arquivo indicado.
 * @param cab Ponteiro para sentinela de empréstimos, 
 * @param caminho Caminho do arquivo a ser lido.
 * cab->prox é o primeiro empréstimo.
 * @returns 0 se teve sucesso, -1 se não teve sucesso.
 * @pre cab não é NULL e cab->prox aponta para o primeiro emprestimo.
 * @post Arquivo indicado contém os emprestimos, sendo que cada linha possui id_usuario;isbn;prazo;data_emprestimo;data_devolucao, nessa ordem.
 * @note Caso não seja possível abrir ou escrever no arquivo, a função exibe mensagem na tela e retorna -1.
 */
int salva_emprestimos(Emprestimo *cab, const char *caminho);

#endif