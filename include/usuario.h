#ifndef USUARIO
#define USUARIO
    #include <stdlib.h>
    //Status do usuario
    typedef enum {
        USER_BLOCKED,
        USER_OK
    } UserStatus;
    
    typedef struct User User;
    typedef struct UserConj UserConj;

    //Conjunto dos Usuarios
    UserConj* user_conj_create(size_t capacidade);
    void user_conj_destroy(UserConj* conjunto);

    unsigned int user_register(UserConj* conjunto, const char* nome);
    int user_delete(UserConj* conjunto, unsigned int user_id);
    User* user_find(UserConj* conjunto, unsigned int user_id);

    const char user_get_name(const User* user);
    unsigned int user_get_id(const User* user);
    UserStatus user_get_status(const User* user);

    //Retorna 1 se o user puder pegar um livro 
    //Retorna 0 se o user nao puder pegar um livro (ja tem um emprestimo ativo)
    int user_emprestimo(const User* user);
    //Atualiza o status o usuario
    int user_set_status(User* user, UserStatus status);



#endif