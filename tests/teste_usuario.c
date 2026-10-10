#include <stdio.h>
#include "../include/usuario.h"

int main() {

    // 1. Testando criacao do conjunto de usuarios (capacidade inicial pequena para testar redimensionamento)
    printf("1. Criando conjunto de usuarios com capacidade 2\n");
    UserConj* conj = cria_conj_usuarios(2); // Capacidade 2 para forçar redimensionamento rápido
    if (!conj) {
        printf("Erro ao criar o conjunto de usuarios!\n");
        return 1;
    }
    printf("Conjunto criado com sucesso. Capacidade inicial: %zu, Tamanho: %zu\n\n", conj->capacidade, conj->tam);

    // 2. Testando cadastro de usuarios  e redimensionamento automático
    printf("2. Cadastrando usuarios: Ana, Bruno, Carlos\n");
    unsigned int id1 = cria_usuario(conj, "Ana ", USER_OK);
    unsigned int id2 = cria_usuario(conj, "Bruno ", USER_OK);
    unsigned int id3 = cria_usuario(conj, "Carlos", USER_BLOCKED); // Cadastrado já bloqueado (com empréstimo)

    printf("Usuarios cadastrados:\n");
    printf(" - ID: %u | Nome: Ana \n", id1);
    printf(" - ID: %u | Nome: Bruno \n", id2);
    printf(" - ID: %u | Nome: Carlos (Bloqueado)\n", id3);
    printf("Capacidade atual apos insercoes: %zu, Tamanho: %zu\n\n", conj->capacidade, conj->tam);

    // 3. Testando busca de usuarios (busca_usuario)
    printf("3. Testando busca de usuarios por ID\n");
    User* u_encontrado = busca_usuario(conj, id2);
    if (u_encontrado) {
        printf("Usuario encontrado! ID: %u, Nome: %s\n", obter_id_usuario(u_encontrado), obter_nome_usuario(u_encontrado));
    } else {
        printf("Usuario com ID %u nao encontrado.\n", id2);
    }
    
    User* u_inexistente = busca_usuario(conj, 999);
    if (!u_inexistente) {
        printf("Busca por ID inexistente (999) retornou NULL corretamente.\n\n");
    }

    // 4. Testando funcoes de consulta (obter_nome_usuario, obter_id_usuario, obter_status_usuario)
    printf("4. Testando funcoes de consulta \n");
    User* u_carlos = busca_usuario(conj, id3);
    if (u_carlos) {
        printf("Consulta direta -> Nome: %s, ID: %u, Status: %d\n\n", 
               obter_nome_usuario(u_carlos), 
               obter_id_usuario(u_carlos), 
               obter_status_usuario(u_carlos));
    }
    User* u_bruno = busca_usuario(conj, id2);
    if (u_bruno) {
        printf("Consulta direta -> Nome: %s, ID: %u, Status: %d\n\n", 
               obter_nome_usuario(u_bruno), 
               obter_id_usuario(u_bruno), 
               obter_status_usuario(u_bruno));
    }

    // 5. Testando verificação e alteração de status (verifica_emprestimo e atualiza_status_usuario)
    printf("5. Testando regras de emprestimo e status\n");
    User* u_ana = busca_usuario(conj, id1);
    
    printf("Ana pode pegar emprestimo? %s\n", verifica_emprestimo(u_ana) ? "Sim (USER_OK)" : "Nao (USER_BLOCKED)");
    
    printf("Atualizando status da Ana para USER_BLOCKED \n");
    atualiza_status_usuario(u_ana, USER_BLOCKED);
    printf("Ana pode pegar emprestimo agora? %s\n\n", verifica_emprestimo(u_ana) ? "Sim" : "Nao (Bloqueado com sucesso)");

    // 6. Testando exclusão de usuários com restrição (remove_usuario)
    printf("6. Testando remocao de usuarios...\n");
    
    // Tentando remover a Ana (que agora está com status USER_BLOCKED - deve falhar retornando 0)
    printf("Tentando remover a Ana (ID %u) que esta com emprestimo pendente...\n", id1);
    int res_rem_bloqueado = remove_usuario(conj, id1);
    printf("Resultado da remocao (0 se falhar): %d\n", res_rem_bloqueado);

    // Liberando o livro do Bruno (atualizando status para USER_OK) e removendo-o em seguida
    atualiza_status_usuario(u_bruno, USER_OK);
    printf("Bruno devolveu o livro (status USER_OK). Removendo Bruno (ID %u)\n", id2);
    int res_rem_sucesso = remove_usuario(conj, id2);
    printf("Resultado da remocao (1 caso sucesso): %d\n", res_rem_sucesso);
    printf("Tamanho atual do conjunto apos remocao: %zu\n\n", conj->tam);

    // 7. Limpando e destruindo o conjunto (remove_conj_usuarios)
    printf("7. Destruindo o conjunto de usuarios e liberando memoria\n");
    int res_destruir = remove_conj_usuarios(conj);
    printf("Conjunto destruido com sucesso (retorno: %d).\n", res_destruir);

    printf("\n=== Todos os testes foram executados com sucesso! ===\n");
    return 0;
}