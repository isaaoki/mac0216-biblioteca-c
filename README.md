# mac0216-biblioteca-c

Biblioteca modular em C para gerenciamento de livros, usuários e empréstimos. O projeto reúne quatro módulos reutilizáveis, uma aplicação cliente de demonstração e testes automatizados.

A biblioteca oferece operações de cadastro, busca, empréstimo, devolução e persistência em arquivos de texto. A aplicação principal em `app/main.c` utiliza os cabeçalhos públicos para demonstrar o uso das operações implementadas. Pode ser compilado com ligação estática ou dinâmica.

## Pré-requisitos

- Ambiente Linux.
- GCC e os cabeçalhos da biblioteca padrão de C.
- GNU Make.
- GNU Binutils, que fornece o comando `ar` usado para gerar a biblioteca estática.
- Git para obter o repositório e acompanhar seu histórico.

Os comandos do Makefile devem ser executados na raiz do projeto.

## Estrutura e módulos

```
mac0216-biblioteca-c/
├── app/
│   └── main.c
├── include/
│   ├── catalogo.h
│   ├── emprestimo.h
│   ├── persistencia.h
│   └── usuario.h
├── src/
│   ├── catalogo.c
│   ├── emprestimo.c
│   ├── persistencia.c
│   └── usuario.c
├── tests/
│   ├── teste_catalogo.c
│   ├── teste_emprestimo.c
│   ├── teste_persistencia.c
│   └── teste_usuario.c
├── build/                     # Gerado durante a compilação
│   ├── obj/                   # Objetos (.o) e dependências (.d)
│   ├── lib/                   # Bibliotecas .a e .so
│   └── bin/                   # Executáveis de teste e suas dependências
├── livros.txt
├── usuarios.txt
├── emprestimos.txt
├── Makefile
├── README.md
├── programa_estatico          # Gerado por make
└── programa_dinamico          # Gerado por make
```

| Módulo | Responsabilidade | Interface pública |
| - | - | - |
| Catálogo | Cadastro, busca, remoção e listagem de livros em uma árvore binária de busca ordenada por ISBN. | [catalogo.h](include/catalogo.h) |
| Usuário | Cadastro, consulta, remoção e controle de status dos usuários em uma tabela hash com listas encadeadas. | [usuario.h](include/usuario.h) |
| Empréstimo | Registro e busca de empréstimos, devoluções, extensão de prazo e cálculo de multas, usando uma lista com sentinela. | [emprestimo.h](include/emprestimo.h) |
| Persistência | Leitura e gravação dos dados de livros, usuários e empréstimos em arquivos de texto. | [persistencia.h](include/persistencia.h) |

As 31 funções públicas estão declaradas em `include/`. Consulte esses cabeçalhos para as assinaturas, os tipos e os comentários sobre uso e retornos. As implementações estão em `src/`. O cliente inclui somente arquivos `.h` da biblioteca.

## Compilação

Para gerar as duas bibliotecas e os dois executáveis da aplicação:

```sh
make
```

Sem argumentos, `make` executa o alvo `all`. Os principais artefatos são:

| Artefato | Finalidade |
| - | - |
| `build/lib/lib_biblio_estat.a` | Biblioteca estática, criada com `ar`. |
| `build/lib/lib_biblio_din.so` | Biblioteca dinâmica, criada com `gcc -shared`. |
| `programa_estatico` | Cliente ligado à biblioteca estática do projeto. |
| `programa_dinamico` | Cliente ligado à biblioteca dinâmica do projeto. |

Para compilar apenas uma biblioteca ou uma versão do cliente:

```sh
make lib_biblio_estat
make lib_biblio_din
make programa_estatico
make programa_dinamico
```

Os objetos das duas bibliotecas são separados em arquivos com os sufixos `_estat.o` e `_din.o`. A versão dinâmica é compilada com `-fPIC`, que gera código independente de posição, possibilitando criar uma biblioteca compartilhada.

### Compilação incremental

O Make compara as datas dos arquivos com as de suas dependências. Se um `.c` for alterado, seus objetos são recompilados e os artefatos dependentes são atualizados. Os demais módulos não precisam ser recompilados.

As opções `-MMD` e `-MP` geram arquivos `.d` com as dependências dos cabeçalhos. O Makefile inclui esses arquivos para também detectar alterações nos `.h`. Depois de uma compilação, executar `make` novamente sem modificar fontes ou cabeçalhos não refaz o trabalho.


## Alvos do Makefile

| Comando | Comportamento |
| --- | --- |
| `make` ou `make all` | Compila as bibliotecas estática e dinâmica e as duas versões do cliente. |
| `make lib_biblio_estat` | Gera somente a biblioteca estática e suas dependências. |
| `make lib_biblio_din` | Gera somente a biblioteca dinâmica e suas dependências. |
| `make programa_estatico` | Compila o cliente com a biblioteca estática. |
| `make programa_dinamico` | Compila o cliente com a biblioteca dinâmica. |
| `make run_estatico` | Compila quando necessário e executa o cliente estático. |
| `make run_dinamico` | Compila quando necessário e executa o cliente dinâmico. |
| `make run` | Compila quando necessário e executa primeiro o cliente dinâmico e depois o estático. |
| `make test` | Compila e executa os quatro programas de teste com a biblioteca estática. |
| `make install` | Compila as bibliotecas quando necessário e instala ambas e os cabeçalhos no prefixo escolhido. |
| `make clean` | Remove os artefatos de compilação e os executáveis de teste. |

## Execução e dados

Para executar cada versão separadamente:

```sh
make run_estatico
make run_dinamico
```

Após a compilação, também é possível executar diretamente:

```sh
./programa_estatico
./programa_dinamico
```

Para executar ambas em sequência:

```sh
make run
```

Não esqueça de executar o comando
```sh
chmod +x ...
```
para que a execução do programa seja possível.

O cliente realiza uma demonstração automática, sem menu ou entrada pelo teclado. Ele carrega os dados, acrescenta um livro e dois usuários, realiza um empréstimo, aumenta o prazo, registra a devolução e deixa outro empréstimo ativo. Por fim, salva e recarrega os resultados. Caso queira adaptar a funcionalidade para seus propósitos, modifique o código cliente.

Os três arquivos de entrada são procurados no diretório de execução. Por isso, execute o cliente na raiz do repositório, mantendo os arquivos fornecidos. Cada registro ocupa uma linha, sem cabeçalho, com campos separados por `;`:

| Arquivo | Campos, na ordem |
| - | - |
| `livros.txt` | `isbn;nome;autor;ano;copias_disponiveis;copias_totais` |
| `usuarios.txt` | `id;nome;status` |
| `emprestimos.txt` | `id_usuario;isbn;prazo;data_emprestimo;data_devolucao` |

O status do usuário é `1` para disponível e `0` para bloqueado. O prazo é medido em dias; as datas são gravadas como números de segundos desde a época Unix no ambiente Linux. A leitura é interrompida quando não consegue interpretar o próximo registro. O cliente encerra com erro se não conseguir carregar alguma estrutura; um catálogo vazio também impede a demonstração.

Os resultados são gravados em `demo_livros.txt`, `demo_usuarios.txt` e `demo_emprestimos.txt`, no diretório de execução. Esses arquivos são sobrescritos a cada execução; os três arquivos de entrada são preservados pela aplicação. Uma mensagem do cliente lista os nomes sem o prefixo `demo_`, mas a gravação e a releitura usam os nomes com esse prefixo. Em `make run`, a execução sobrescreve os resultados da execução anterior.

### Ligação estática e dinâmica

Na ligação estática, o código necessário da biblioteca `.a` é incorporado ao executável durante a ligação. Assim, `programa_estatico` não precisa do arquivo `.a` para executar. Isso se refere à biblioteca deste projeto (Não o objetivo do projeto).

Na ligação dinâmica, o executável depende dos `.so`, carregados em runtime. O Makefile configura o caminho `$ORIGIN/build/lib`, relativo ao diretório do executável, para localizar `lib_biblio_din.so`. Não é necessário configurar `LD_LIBRARY_PATH` nem instalar a biblioteca no sistema para executar o cliente, enquanto você manter essa estrutura.


## Testes

```sh
make test
```

O alvo compila e executa os testes de catálogo, persistência, empréstimo e usuário, nessa ordem. Os executáveis ficam em `build/bin/` e usam a biblioteca estática.

| Teste | Principais verificações existentes |
| --- | --- |
| Catálogo | Inserção, busca, remoção, impressão e liberação. Tratamento de erros de textos vazios ou longos, ponteiros nulos e ano negativo. |
| Usuário | Criação e consulta, cadastro com ID definido, status e remoção. Tratamento de erros de  IDs duplicados, zero e `UINT_MAX`, nomes inválidos e remoção de usuário bloqueado. |
| Empréstimo | Empréstimo, busca, extensão de prazo, devolução, impressão e liberação. Tratamento de erros de parâmetros inválidos, prazo negativo, empréstimo inexistente e multa com e sem atraso. |
| Persistência | Leitura, gravação e comparação dos dados recarregados; Tratamento de erros de leitura de arquivo inexistente e gravação em diretório inexistente. |

Os testes usam `assert` para verificar os resultados. Quando uma condição falha, o programa é interrompido e `make test` termina com erro. Quando todos passam, o comando termina com código zero e apresenta as mensagens de sucesso dos testes. Mensagens como `[ERRO]` também aparecem nos casos de entrada inválida esperados. Isoladamente, elas não indicam falha em todos os testes.

## Instalação

O prefixo padrão é `/usr/local`. O comando abaixo instala nesse prefixo e depende de permissão de escrita nos diretórios de destino, é esperado que o usuário tenha as permissões de escrita no diretório destino:

```sh
make install
```

Para instalar no diretório do próprio usuário, sem privilégios administrativos:

```sh
make install PREFIX="$HOME/.local"
```

O alvo cria os diretórios necessários e copia:

- `lib_biblio_estat.a` e `lib_biblio_din.so` para `PREFIX/lib/`;
- os quatro cabeçalhos de `include/` para `PREFIX/include/`.

Os executáveis e os arquivos de dados não são instalados.

Depois de instalar em `$HOME/.local`, outro cliente pode incluir os cabeçalhos necessários e ser compilado com uma das opções abaixo. Substitua `cliente.c` pelo arquivo da sua aplicação:

```sh
# Ligação estática com a biblioteca instalada
gcc -I"$HOME/.local/include" cliente.c \
    "$HOME/.local/lib/lib_biblio_estat.a" -o cliente_estatico

# Ligação dinâmica com caminho de busca em tempo de execução
gcc -I"$HOME/.local/include" cliente.c \
    -L"$HOME/.local/lib" -l_biblio_din \
    -Wl,-rpath,"$HOME/.local/lib" -o cliente_dinamico
```

A opção `-I` informa onde encontrar os cabeçalhos; `-L` indica onde procurar a biblioteca durante a ligação. O `rpath` do segundo comando permite localizar a `.so` instalada durante a execução. Se escolher outro prefixo, ajuste esses caminhos.

## Limpeza

```sh
make clean
```

Deleta as bibliotecas `.a` e `.so` de `build/lib/`, os objetos `.o` e dependências `.d` de `build/obj/`, os executáveis `programa_estatico` e `programa_dinamico`, os arquivos `teste*` e as dependências `.d` de `build/bin/`.

Note que os diretórios de compilação permanecem. Os arquivos de entrada(Nesse caso os emprestimos.txt, livros.txt e usuarios.txt) e os resultados `demo_*.txt` também são mantidos.
