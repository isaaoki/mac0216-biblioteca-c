#include <stdio.h>
#include "catalogo.h"
#include "emprestimo.h"
#include "persistencia.h"
#include "usuario.h"

// Demonstra as operacoes usando somente as funcoes publicas da biblioteca.
int demonstra_biblioteca(Livro *catalogo, UserConj *usuarios,
                               Emprestimo *emprestimos) {
    // Os IDs são inteiros sem sinal (nao negativos), como definido em usuario.h.
    unsigned int id_ana = cria_usuario(usuarios, "Ana", USER_OK);
    unsigned int id_bruno = cria_usuario(usuarios, "Bruno", USER_OK);
    // Verifica se os usuarios conseguiram IDS.
    if (id_ana == 0 || id_bruno == 0) {
        printf("Nao foi possivel cadastrar os usuarios.\n");
        return 0;
    }

    // Mostra os livros e os usuários cadastrados.
    // A main havia inserido um livro no catálogo da seguinte forma:
    // insere_livro(catalogo, "9780000000007", "Linguagem C", "B. Kernighan", 1988)

    printf("=== Livros e usuarios cadastrados ===\n");
    imprime_livros(catalogo);
    imprime_usuarios(usuarios);
    // Busca o livro pelo ISBN na árvore.
    Livro *livro = busca_livro(catalogo, "9780000000007");
    if (livro == NULL) {
        printf("Livro nao encontrado.\n");
        return 0;
    }

    printf("\n=== Emprestimo para Ana ===\n");
    // Caso empréstimo falhe.
    if (empresta_livro(emprestimos, usuarios, livro, id_ana, "9780000000007") == 0) {
        return 0;
    }
    // Caso não falhe, imprime.
    imprime_emprestimos(emprestimos);
    imprime_usuarios(usuarios);
    imprime_livros(catalogo);
    // Aumenta o prazo dos empréstimos em 3 dias.
    printf("\n=== Aumento do prazo em 3 dias ===\n");
    if (aumenta_prazo(emprestimos, id_ana, 3) == 0) {
        return 0;
    }
    imprime_emprestimos(emprestimos);
    // Devolução do empréstimo de um livro.
    printf("\n=== Devolucao de Ana ===\n");
    int multa;
    // Caso de erro como parâmetros inválidos.
    if (devolve_livro(emprestimos, usuarios, livro, id_ana, &multa) == 0) {
        return 0;
    }
    //devolve_livro atribuiu ao multa o valor calculado via ponteiro.
    printf("Livro devolvido. Multa: R$ %d\n", multa);
    imprime_usuarios(usuarios);

    // Deixa um empréstimo ativo para demonstrar a persistencia.
    printf("\n=== Emprestimo para Bruno ===\n");
    if (empresta_livro(emprestimos, usuarios, livro, id_bruno, "9780000000007") == 0) {
        return 0;
    }
    imprime_emprestimos(emprestimos);
    // Salva as estruturas de dados em arquivos, os caminhos são os segundo parametros.
    // Fiz salvar em um arquivo diferente, pq toda vez q eu executava esse código, ele poluia o emprestimo.txt e os outros.
    printf("\n=== Salvando os dados da demonstracao ===\n");
    int salvou_livros = salva_catalogo(catalogo, "demo_livros.txt");
    int salvou_usuarios = salva_usuarios(usuarios, "demo_usuarios.txt");
    int salvou_emprestimos = salva_emprestimos(emprestimos, "demo_emprestimos.txt");
    // Verifica se algum deles deu erro.
    if (salvou_livros == 0 || salvou_usuarios == 0 || salvou_emprestimos == 0) {
        printf("Falha ao salvar os dados da demonstracao.\n");
        return 0;
    }
    printf("Arquivos: demo_livros.txt, demo_usuarios.txt e demo_emprestimos.txt\n");
    return 1;
    // Continue lendo o main, pois vou continuar demonstrando a persistência(recarregar os dados).
}

int main(void) {
    // Carrega os dados dos arquivos para as estruturas em memoria.
    Livro *catalogo = carrega_catalogo("livros.txt");
    UserConj *usuarios = carrega_usuarios("usuarios.txt");
    Emprestimo *emprestimos = carrega_emprestimos("emprestimos.txt");
    // Verifica se os dados foram carregados.
    if (catalogo == NULL || usuarios == NULL || emprestimos == NULL) {
        printf("Nao foi possivel carregar os dados da biblioteca.\n");
        remove_catalogo(catalogo);
        if (usuarios != NULL) {
            remove_conj_usuarios(usuarios);
        }
        libera_lista_emprestimos(emprestimos);
        return 1;
    }
    printf("=== Dados carregados dos arquivos ===\n");
    imprime_livros(catalogo);
    imprime_usuarios(usuarios);
    imprime_emprestimos(emprestimos);

    // Acrescenta o livro usado na demonstracao ao catalogo carregado.
    catalogo = insere_livro(catalogo, "9780000000007", "Linguagem C", "B. Kernighan", 1988);
    //Caso dê errado, libera a memória das outras estruturas.
    if (catalogo == NULL) {
        remove_conj_usuarios(usuarios);
        libera_lista_emprestimos(emprestimos);
        return 1;
    }

    printf("Demonstracao da biblioteca\n");
    printf("Os arquivos demo_*.txt serao sobrescritos a cada execucao.\n\n");
    //Chama a função de demonstração.
    int sucesso = demonstra_biblioteca(catalogo, usuarios, emprestimos);
    //Com o término dela, libera as estruturas de dado.
    libera_lista_emprestimos(emprestimos);
    remove_catalogo(catalogo);
    remove_conj_usuarios(usuarios);
    if (sucesso == 0) {
        return 1;
    }

    //Recarrega os arquivos criados na demonstração depois de liberar os dados originais.
    printf("\n=== Carregando os dados salvos ===\n");
    catalogo = carrega_catalogo("demo_livros.txt");
    usuarios = carrega_usuarios("demo_usuarios.txt");
    emprestimos = carrega_emprestimos("demo_emprestimos.txt");
    // Caso dê certo, mostra novamente os dados.
    if (catalogo != NULL && usuarios != NULL && emprestimos != NULL) {
        sucesso = 1;
        imprime_livros(catalogo);
        imprime_usuarios(usuarios);
        imprime_emprestimos(emprestimos);
    } else {
        sucesso = 0;
        printf("Falha ao carregar os dados da demonstracao.\n");
    }
    // Libera novamente as estruturas de dado.
    libera_lista_emprestimos(emprestimos);
    remove_catalogo(catalogo);
    if (usuarios != NULL) {
        remove_conj_usuarios(usuarios);
    }

    if (sucesso == 1) {
        return 0;
    } else {
        return 1;
    }
}
