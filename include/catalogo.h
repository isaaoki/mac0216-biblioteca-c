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
 * @brief Insere um livro na árvore binária de busca ou, se o ISBN já existir, acrescenta uma cópia.
 * @param root é a raiz da árvore que vai ter o livro inserido (pode ser NULL).
 * @param isbn é o ISBN do livro a ser inserido (1 a 13 caracteres).
 * @param nome é o nome do livro a ser inserido (1 a 49 caracteres).
 * @param autor é o autor do livro a ser inserido (1 a 49 caracteres).
 * @param ano é o ano da publicação do livro a ser inserido (>= 0).
 * @return Retorna a raiz do catálogo após a inserção. Em caso de erro, retorna a raíz recebida, sem alterar o catálogo ou NULL se o catálogo estava vazio.
 * @pre root deve ser NULL ou apontar para uma árvore de busca binária
 * válida, ordenada por ISBN usando strcmp.
 * @pre Se não forem NULL, isbn, nome e autor devem apontar para strings
 * válidas e terminadas em '\0'.
 * @post Se ISBN é novo, um nó é criado com 1 cópia totoal e 1 disponível. Se ISBN existe, cópias totais e disponíveis são aumentadas em 1.
 * @note Caso root seja NULL, ocorre a criação de uma nova raiz.
 * @note Se isbn, nome ou autor forem NULL, imprime erro e retorna a raiz sem alterar o catálogo.
 * @note Se isbn, nome ou autor for NULL, ou ultrapassar os limites de
 * 13, 49 e 49 bytes, respectivamente (sem contar o '\0'), imprime uma
 * mensagem e retorna a raiz sem alterar o catálogo.
 * @note Se isbn, nome ou autor contiverem ';' ou quebra de linha, imprime erro e retorna a
 * raiz sem alterar o catálogo (esses caracteres corromperiam o arquivo de persistência).
 * @note Se ano for menor do que zero, imprime erro e retorna a raíz, sem
 * alterar o catálogo.
 * @note Em caso de inserir um livro com ISBN já existente, função ignora nome, autor e ano.
 * @warning Em casos de falha de alocação, ele retorna a raiz.
 * @warning Devido ao armazenamento em int da quantidade de livros de mesmo ISBN, é possível ultrapassar 
 * o INT_MAX e causar comportamentos indefinidos.
 */
Livro *insere_livro(Livro *root, const char *isbn, const char *nome, const char *autor, int ano);

/**
 * @brief Remove o livro da árvore, a partir do ISBN.
 * @param root é a raiz do catálogo.
 * @param isbn é o ISBN do livro que terá o nó deletado.
 * @return Retorna a raiz da árvore após remoção do nó. A raiz da árvore se mantém a mesma
 * em casos que o nó contendo ISBN procurado não existir.
 * @pre root deve ser NULL ou apontar para uma árvore binária de busca
 * válida, ordenada por ISBN usando strcmp.
 * @pre ISBN deve ser uma string válida, e terminada em '\0'.
 * @post O nó removido é liberado; o ponteiro para ele deixa de ser válido.
 * @note Caso remova a raíz, o endereço retornado será diferente do recebido.
 * @note A árvore preserva a sua propriedade de árvore binária de busca, no entanto pode
 * ter seu equilibrio alterado.
 * @note Caso ISBN seja NULL, retornará a raiz sem alteração do catálogo.
 * @warning Caso delete o último nó da árvore, retornará NULL.
 * @warning O retorno deve ser atribuído à variável que contém o catálogo.
 * @warning Remove o registro inteiro do livro, independentemente da quantidade de cópias.
 * @warning A função apaga o livro mesmo que haja empréstimos ativos com o ISBN. É necessário validar antes na camada de empréstimo.
 */
Livro *remove_livro(Livro *root, const char *isbn);

/**
 * @brief Busca um livro pelo ISBN e imprime suas informações.
 * @param root é raiz do catálogo.
 * @param isbn é o ISBN do livro procurado.
 * @return Retorna o ponteiro para o livro encontrado, ou NULL se não existir, se isbn for NULL
 * ou se isbn for uma string vazia.
 * @pre root é NULL ou aponta para uma árvore binária de busca válida, ordenada por ISBN usando strcmp.
 * @pre Se ISBN não for NULL, deve apontar para uma string válida terminada em '\0'.
 * @note Não imprime nada, inclusive quando o livro não é encontrado; quem chama decide como tratar a ausência.
 * @warning O ponteiro retornado pertence ao catálogo e não deve ser
 * liberado diretamente pelo chamador.
 */
Livro *busca_livro(Livro *root, const char *isbn);

/**
 * @brief Imprime os livros salvos do programa em ordem crescente de ISBN (in-order).
 * @param root Ponteiro para o catalogo de livros (raíz da árvore);
 * @note Se root for NULL, nada é impresso.
 * @warning A impressão é recursiva; em árvores muito desbalanceadas a profundidade pode ser grande.
 */
void imprime_livros(Livro *root);

/**
 * @brief Libera toda a memória da árvore de livros.
 * @param root Raiz da árvore de livros (pode ser NULL).
 * @pre Todos os nós foram alocados dinamicamente (por insere_livro).
 * @post Todos os nós da árvore são liberados. O ponteiro root fica inválido, 
 * então é recomendado atribuir NULL a ele depois da função ser chamada.
 * @warning A liberação é recursiva; em árvores muito desbalanceadas a profundidade pode ser grande.
 */
void remove_catalogo(Livro *root);

#endif