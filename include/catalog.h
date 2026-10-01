/**
    * @brief Dados de um livro, e sua posição na árvore de busca binária.
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


Book *insert_Book(
    Book *root,
    const char *isbn,
    const char *nome,
    const char *autor,
    int ano
);

Book *remove_Book(Book *root, const char *ISBN);

Book *search_and_info_book(Book *root, const char *ISBN);
