/**
    * @brief Unidade da árvore, um nó, que contém um valor, e os endereços dos nós filhos da direita e esquerda.
*/
typedef struct Node{
    struct Node *left;
    struct Node *right;
    int value;
} Node; 

/**
    * @brief Insere um valor na árvore de busca binária.
    * @param *root recebe a raiz da árvore como parâmetro.
    * @param value recebe o valor a ser inserido como parâmetro.
    * @return retorna a raiz em casos sucesso, caso falha retorna NULL.
    * @pre deve inserir endereco de uma raiz válida, ou NULL.
    * @note Caso queira inserir o valor na mesma árvore, é necessário passar
    sua raiz como parâmetro. Caso insira NULL, gerará nova árvore desconexa.
    * @note A liberação da memória deve ser feita pelo usuário.
    * @note Valores que já estão presentes na árvore, serão colocados a direita.
    * @warning O uso indevido ocupará muito to heap.
*/
Node *insert_value(Node *root, int value);
/**
    * @brief Remove o valor na árvore de busca binária.
    * @param *root recevbe a raiz da árvore como parâmetro.
    * @param value pertence a nó que vai ser retirado da árvore.
    * @return retorna a raiz da árvore com o nó retirado.
    * @pre *root deve ser uma raiz válida.
    * @note Caso a árvore não tenha o valor, será retornado a árvore intacta.
    * @note Caso a árvore tenha mais de um valor do mesmo, o mais perto da raiz será removido.
    * @note Caso o removido seja a raiz, retornará nova raiz.
    * @warning em casos que retorne NULL, evite acessar o conteúdo da raiz, visto que causará segmentation fault.
*/
Node *remove_value(Node *root, int value);
/**
    * @brief Encontra o valor na árvore de busca binária e retorna o endereço do nó com o valor.
    * @param *root receve a raiz da árvore como parâmetro.
    * @param value recebe o valor desejado a encontrar como parâmetro
    * @return retorna o endereço do nó com o valor.
    * @pre deve inserir endereco de uma raiz válida, ou NULL.
    * @note Caso o valor não esteja presente, recebe NULL.
    * @warning em casos que retorne NULL, evite acessar o conteúdo da raiz, visto que causará segmentation fault.
*/
Node *search_value(Node *root, int value);