#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include "usuario.h"

int main() {
    // 1. Testando criacao do conjunto de usuarios 
    printf("Teste 1. Criando conjunto de usuarios sem capacidade inicial\n");
    UserConj *conj = cria_conj_usuarios(0); // Usando a capacidade padrão
    assert(conj != NULL);
    assert(conj->capacidade == 16);
    assert(conj->tam == 0);
    assert(conj->next_id == 1);
    printf("Conjunto criado com sucesso. Capacidade inicial: %zu, Tamanho: %zu\n\n", conj->capacidade, conj->tam);
    assert(remove_conj_usuarios(conj) == 1);

    printf("Teste 2. Criando conjunto de usuarios com capacidade 2 (redimensionamento)\n");
    conj = cria_conj_usuarios(2); // Capacidade 2 para forçar redimensionamento rápido
    assert(conj != NULL);
    assert(conj->capacidade == 2);
    assert(conj->tam == 0);
    assert(conj->next_id == 1);
    printf("Conjunto criado com sucesso. Capacidade inicial: %zu, Tamanho: %zu\n\n", conj->capacidade, conj->tam);

    // 2. Testando cadastro de usuarios  e redimensionamento automático
    printf("Teste 3. Cadastrando usuarios: Ana, Bruno, Carlos\n");
    unsigned int id1 = cria_usuario(conj, "Ana ", USER_OK);
    unsigned int id2 = cria_usuario(conj, "Bruno ", USER_OK);
    unsigned int id3 = cria_usuario(conj, "Carlos", USER_BLOCKED); // Cadastrado já bloqueado (com empréstimo)
    printf(" - Sucesso: Ana criada com - ID: %u | Nome: Ana \n", id1);
    printf(" - Sucesso: Ana criada com - ID: %u | Nome: Bruno \n", id2);
    printf(" - Sucesso: Carlos criada com - ID: %u | Nome: Carlos \n", id3);
    assert(id1 == 1 && id2 == 2 && id3 == 3);
    assert(conj->tam == 3);

    printf("Teste 4. Criando usuarios com parametros invalidos\n");
    char nome100[101];
    memset(nome100, 'a', 100); nome100[100] = '\0';
    assert(cria_usuario(NULL, "Teste", USER_OK) == 0);
    assert(cria_usuario(conj, NULL, USER_OK) == 0);
    assert(cria_usuario(conj, "", USER_OK) == 0);
    assert(cria_usuario(conj, nome100, USER_OK) == 0);
    assert(cria_usuario(conj, "Ana;Silva", USER_OK) == 0);
    assert(cria_usuario(conj, "Ana\nSilva", USER_OK) == 0);
    assert(conj->tam == 3);
    printf("Capacidade atual apos insercoes: %zu, Tamanho: %zu\n\n", conj->capacidade, conj->tam);

    // 3. Testando funcoes de busca e de consulta (busca_usuario, obtem_nome_usuario, obtem_id_usuario, obtem_status_usuario)
    printf("Teste 5. Testando funcoes de busca e de consulta\n");
    User* u_carlos = busca_usuario(conj, id3);
    assert(u_carlos != NULL);
    assert(obtem_id_usuario(u_carlos) == id3);
    assert(strcmp(obtem_nome_usuario(u_carlos), "Carlos") == 0);
    assert(obtem_status_usuario(u_carlos) == (UserStatus) USER_BLOCKED);
    printf("Usuario encontrado! ID: %u, Nome: %s\n", obtem_id_usuario(u_carlos), obtem_nome_usuario(u_carlos));
    printf("Consulta direta -> Nome: %s, ID: %u, Status: %d\n\n", obtem_nome_usuario(u_carlos), obtem_id_usuario(u_carlos), obtem_status_usuario(u_carlos));

    printf("Teste 6. Testando funcoes de busca e de consulta com parametros invalidos\n");
    assert(busca_usuario(NULL, 1) == NULL);
    assert(busca_usuario(conj, 0) == NULL);
    assert(busca_usuario(conj, 999) == NULL); // usuario inexistente
    assert(obtem_nome_usuario(NULL) == NULL);
    assert(obtem_id_usuario(NULL) == 0);
    assert(obtem_status_usuario(NULL) == USER_BLOCKED);

    // 4. Testando verificação e alteração de status (usuario_pode_emprestar e atualiza_status_usuario)
    printf("Teste 7. Testando regras de emprestimo e status\n");
    User* u_ana = busca_usuario(conj, id1);
    assert(u_ana != NULL);
    assert(obtem_status_usuario(u_ana) == USER_OK);    
    assert(usuario_pode_emprestar(u_ana) == 1);    
    printf("Atualizando status da Ana para USER_BLOCKED \n");
    assert(atualiza_status_usuario(u_ana, USER_BLOCKED) == 1);
    assert(obtem_status_usuario(u_ana) == USER_BLOCKED);    
    assert(usuario_pode_emprestar(u_ana) == 0);

    printf("Teste 8. Testando regras de emprestimo e status com parametros invalidos\n");
    assert(atualiza_status_usuario(NULL, USER_OK) == 0);
    assert(usuario_pode_emprestar(NULL) == 0);

    // 6. Testando exclusão de usuários com restrição (remove_usuario)
    printf("Teste 9. Testando remocao de usuarios...\n");
    // Tentando remover a Ana (que agora está com status USER_BLOCKED - deve falhar retornando 0)
    printf("Tentando remover a Ana (ID %u) que esta com emprestimo pendente...\n", id1);
    assert(remove_usuario(conj, id1) == 0);

    // Liberando o livro da Ana (atualizando status para USER_OK) e removendo-o em seguida
    atualiza_status_usuario(u_ana, USER_OK);
    printf("Ana devolveu o livro (status USER_OK). Removendo Ana (ID %u)\n", id1);
    assert(remove_usuario(conj, id1) == 1);
    printf("Tamanho atual do conjunto apos remocao: %zu\n\n", conj->tam);

    // 7. Remocao de usuarios
    printf("Teste 10. Testando remocao de usuarios com parametros invalidos...\n");
    assert(remove_usuario(NULL, 1) == 0);
    assert(remove_usuario(conj, 0) == 0);
    assert(remove_usuario(conj, 999) == 0); // usuario nao existe

    // 8. Criacao de usuario com id personalizado
    printf("Teste 11. Criar usuario com id personalizado\n");
    assert(cria_usuario_com_id(conj, 50, "Maria", USER_OK) == 50);
    assert(busca_usuario(conj, 50) != NULL);
    assert(cria_usuario_com_id(conj, 50, "Outra", USER_OK) == 0);
    assert(cria_usuario_com_id(conj, 0, "Joao", USER_OK) == 0);
    assert(cria_usuario_com_id(conj, UINT_MAX, "Joao", USER_OK) == 0);
    assert(cria_usuario_com_id(NULL, 60, "Joao", USER_OK) == 0);
    assert(cria_usuario_com_id(conj, 60, NULL, USER_OK) == 0);

    // 9. Checar impressao
    printf("Teste 12. Impressao dos usuarios\n");
    imprime_usuarios(conj);
    imprime_usuarios(NULL); // Nao imprime nada

    // 10. Limpando e destruindo o conjunto (remove_conj_usuarios)
    printf("Teste 13. Destruindo o conjunto de usuarios e liberando memoria\n");
    assert(remove_conj_usuarios(conj) == 1);

    printf("\n=== Todos os testes foram executados com sucesso! ===\n");
    return 0;
}