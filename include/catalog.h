#ifndef CATALOG_H
#define CATALOG_H
/**
    * @brief Dados de um livro, e sua posição na árvore binária de busca.
*/
typedef struct Book{
    struct Book *left;
    struct Book *right;
    char isbn[14];
    char nome[50];
    char autor[50];
    int ano;
    int copias_disponiveis;
    int copias_totais;
} Book; 

/**
    * @brief Inserção de um livro na árvore binária de busca.
    * @param isbn é ISBN do livro a ser inserido.
    * @param nome é o nome do livro a ser inserido.
    * @param autor é o autor do livro a ser inserido.
    * @param ano é o ano da publicação do livro a ser inserido.
    * @param root é a raiz da árvore que vai ter o livro inserido.
    * @return Retorna a raiz da árvore em que ocorre a inserção.
    * @note Criação de um novo nó na árvore, caso o livro com o ISBN 
    * correspondente não exista na árvore.
    * @note Caso o livro já exista na árvore, incrementa a quantidade 
    * total e disponivel do mesmo.
    * @note Caso seja inserido um livro em uma raiz vazia, ocorre a criação de uma 
    * nova raiz.
    * @pre root deve ser NULL ou apontar para uma árvore de busca binária
    * válida, ordenada por ISBN usando strcmp.
    * @pre Se não forem NULL, isbn, nome e autor devem apontar para strings
    * válidas e terminadas em '\0'.
    * @note Se isbn, nome ou autor for NULL, ou ultrapassar os limites de
    * 13, 49 e 49 bytes, respectivamente (sem contar o '\0'), imprime uma
    * mensagem e retorna a raiz sem alterar o catálogo.
    * @warning Em casos de falha de alocação, ele retorna a raiz, logo há caso em que
    * retorna NULL, quando há falha de alocação na criação de uma raiz.
*/
Book *insert_Book(
    Book *root,
    const char *isbn,
    const char *nome,
    const char *autor,
    int ano
);
/**
    * @brief Destrói o nó do livro presente na árvore a partir do ISBN.
    * @param root é a raiz do catálogo.
    * @param ISBN é o ISBN do livro que terá o nó deletado.
    * @return Retorna a raiz da árvore que passou pela remoção do nó. A raiz da árvore se mantém a mesma
    * em casos que o nó contendo ISBN procurado não existir.
    * @note Caso delete a raiz, o endereço retornado será diferente.
    * @note A árvore preserva a sua propriedade de árvore binária de busca, no entanto pode
    * ter seu equilibrio alterado.
    * @pre root deve ser NULL ou apontar para uma árvore binária de busca
    *      válida, ordenada por ISBN usando strcmp.
    * @pre ISBN deve ser uma string válida, não nula e terminada em '\0'.
    * @warning Caso delete o último nó da árvore, retornará NULL.
    * @warning O retorno deve ser atribuído à variável que contém o catálogo.
    * @warning Remove o registro inteiro do livro, independentemente
    *          da quantidade de cópias.
*/
Book *destroy_Book(Book *root, const char *ISBN);
/**
    * @brief Busca um livro pelo ISBN e imprime suas informações.
    * @param root é raiz do catálogo.
    * @param ISBN é o ISBN do livro procurado.
    * @return Retorna o ponteiro para o livro encontrado, ou NULL em casos que não for encontrado,
    *         , ISBN for NULL, se a string estiver vazia.
    * @pre root deve ser NULL ou apontar para uma árvore binária de busca
    *      válida, ordenada por ISBN usando strcmp.
    * @pre Se ISBN não for NULL, deve apontar para uma string válida terminada em '\0'.
    * @note Imprime ISBN, nome, autor, ano de publicação e quantidades
    *       de cópias disponíveis e totais quando encontra o livro.
    * @note Não altera o catálogo e não imprime nada se não encontrar o livro.
    * @warning O ponteiro retornado pertence ao catálogo e não deve ser
    *          liberado diretamente pelo chamador.
 */
Book *search_and_info_Book(Book *root, const char *ISBN);

#endif
