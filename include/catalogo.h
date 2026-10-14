#ifndef CATALOGO_H
#define CATALOGO_H

/**
 * @brief Representa um livro com isbn, nome, autor, número de cópias e sua posição na árvore binária de busca.
 */
typedef struct Livro{
    struct Livro *left;
    struct Livro *right;
    char isbn[14];
    char nome[50];
    char autor[50];
    int ano;
    int copias_disponiveis;
    int copias_totais;
} Livro; 

/**
 * @brief Insere um livro na árvore binária de busca.
 * @param isbn é o ISBN do livro a ser inserido.
 * @param nome é o nome do livro a ser inserido.
 * @param autor é o autor do livro a ser inserido.
 * @param ano é o ano da publicação do livro a ser inserido.
 * @param root é a raiz da árvore que vai ter o livro inserido.
 * @return Retorna a raiz da árvore em que ocorre a inserção.
 * @pre root deve ser NULL ou apontar para uma árvore de busca binária
 * válida, ordenada por ISBN usando strcmp.
 * @pre Se não forem NULL, isbn, nome e autor devem apontar para strings
 * válidas e terminadas em '\0'.
 * @note Criação de um novo nó na árvore, caso o livro com o ISBN 
 * correspondente não exista na árvore.
 * @note Caso o livro já exista na árvore, incrementa a quantidade 
 * total e disponivel do mesmo.
 * @note Caso seja inserido um livro em uma raiz vazia, ocorre a criação de uma 
 * nova raiz.
 * @note Se isbn, nome ou autor for NULL, ou ultrapassar os limites de
 * 13, 49 e 49 bytes, respectivamente (sem contar o '\0'), imprime uma
 * mensagem e retorna a raiz sem alterar o catálogo.
 * @note Se ano for menor do que zero, imprime erro e retorna a raíz, sem
 * alterar o catálogo.
 * @note Em casos de ISBN vazio, causará erro indicando o problema.
 * @warning Em casos de falha de alocação, ele retorna a raiz. Asism, pode retornar
 * NULL, quando há falha de alocação na criação de uma raiz.
 */
Livro *insere_livro(Livro *root, const char *isbn, const char *nome, const char *autor, int ano);

/**
 * @brief Remove o livro da árvore, a partir do ISBN.
 * @param root é a raiz do catálogo.
 * @param isbn é o ISBN do livro que terá o nó deletado.
 * @return Retorna a raiz da árvore que passou pela remoção do nó. A raiz da árvore se mantém a mesma
 * em casos que o nó contendo ISBN procurado não existir.
 * @pre root deve ser NULL ou apontar para uma árvore binária de busca
 * válida, ordenada por ISBN usando strcmp.
 * @pre ISBN deve ser uma string válida, e terminada em '\0'.
 * @note Caso delete a raiz, o endereço retornado será diferente.
 * @note A árvore preserva a sua propriedade de árvore binária de busca, no entanto pode
 * ter seu equilibrio alterado.
 * @note Caso ISBN seja NULL, imprimirá erro e retornará a raiz sem alteração do catálogo.
 * @warning Caso delete o último nó da árvore, retornará NULL.
 * @warning O retorno deve ser atribuído à variável que contém o catálogo.
 * @warning Remove o registro inteiro do livro, independentemente da quantidade de cópias.
 * @warning Devido ao armazenamento em int da quantidade de livros de mesmo ISBN, é possível ultrapassar 
 * o INT_MAX e causar comportamentos indefinidos.
 */
Livro *remove_livro(Livro *root, const char *isbn);

/**
 * @brief Busca um livro pelo ISBN e imprime suas informações.
 * @param root é raiz do catálogo.
 * @param isbn é o ISBN do livro procurado.
 * @return Retorna o ponteiro para o livro encontrado, ou NULL em casos que não for encontrado.
 * Retorna NULL, se ISBN for uma string vazia.
 * @pre root deve ser NULL ou apontar para uma árvore binária de busca
 * válida, ordenada por ISBN usando strcmp.
 * @pre Se ISBN não for NULL, deve apontar para uma string válida terminada em '\0'.
 * @note Imprime ISBN, nome, autor, ano de publicação e quantidades
 * de cópias disponíveis e totais quando encontra o livro.
 * @note Imprime uma mensagem de aviso caso o livro não seja encontrado.
 * @warning O ponteiro retornado pertence ao catálogo e não deve ser
 * liberado diretamente pelo chamador.
 */
Livro *busca_livro(Livro *root, const char *isbn);

/**
 * @brief Imprime os livros salvos do programa.
 * @param root Ponteiro para o catalogo de livros (raíz da árvore);
 * @pre O ponteiro catalogo não é NULL.
 */
void imprime_livros(Livro *root);

/**
 * @brief Libera toda a memória da árvore de livros.
 * @param root Raiz da árvore de livros (pode ser NULL).
 * @pre Todos os nós foram alocados dinamicamente (por insere_livro).
 * @post Todos os nós da árvore são liberados. O ponteiro root fica inválido, 
 * então é recomendado atribuir NULL a ele depois da função ser chamada.
 */
void remove_catalogo(Livro *root);

#endif