CC = gcc

HEADERS_ESTAT = -Iinclude -Wall -MMD -MP -c

HEADERS_DIN = -Iinclude -fPIC -Wall -MMD -MP -c

AR = ar

AR_FLAGS = -rcv

DIR_OBJ = ./build/obj

DIR_LIB = ./build/lib

DIR_TESTS = ./tests

DIR_BIN = ./build/bin

.DEFAULT_GOAL := all

#Criação dos diretórios:
$(DIR_OBJ) $(DIR_LIB) $(DIR_BIN):
	mkdir -p $@

all: programa_estatico programa_dinamico

run_estatico: programa_estatico
	./programa_estatico

run_dinamico: programa_dinamico
	./programa_dinamico

run: programa_dinamico programa_estatico
	./programa_dinamico
	./programa_estatico

clean:
	rm -f $(DIR_LIB)/*.so
	rm -f $(DIR_LIB)/*.a
	rm -f $(DIR_OBJ)/*.o
	rm -f $(DIR_OBJ)/*.d
	rm -f programa_dinamico programa_estatico
	rm -f $(DIR_BIN)/teste*
	rm -f $(DIR_BIN)/*.d

#Testes:
test: $(DIR_BIN)/teste_catalogo $(DIR_BIN)/teste_persistencia $(DIR_BIN)/teste_emprestimo $(DIR_BIN)/teste_usuario
	$(DIR_BIN)/teste_catalogo
	$(DIR_BIN)/teste_persistencia
	$(DIR_BIN)/teste_emprestimo
	$(DIR_BIN)/teste_usuario

$(DIR_BIN)/teste_catalogo: $(DIR_TESTS)/teste_catalogo.c $(DIR_LIB)/lib_biblio_estat.a | $(DIR_BIN)
	$(CC) -Iinclude -MMD -MP -MF $(DIR_BIN)/teste_catalogo.d -MT $(DIR_BIN)/teste_catalogo ./tests/teste_catalogo.c $(DIR_LIB)/lib_biblio_estat.a -o $(DIR_BIN)/teste_catalogo

$(DIR_BIN)/teste_persistencia: $(DIR_TESTS)/teste_persistencia.c $(DIR_LIB)/lib_biblio_estat.a | $(DIR_BIN)
	$(CC) -Iinclude -MMD -MP -MF $(DIR_BIN)/teste_persistencia.d -MT $(DIR_BIN)/teste_persistencia ./tests/teste_persistencia.c $(DIR_LIB)/lib_biblio_estat.a -o $(DIR_BIN)/teste_persistencia

$(DIR_BIN)/teste_emprestimo: $(DIR_TESTS)/teste_emprestimo.c $(DIR_LIB)/lib_biblio_estat.a | $(DIR_BIN)
	$(CC) -Iinclude -MMD -MP -MF $(DIR_BIN)/teste_emprestimo.d -MT $(DIR_BIN)/teste_emprestimo ./tests/teste_emprestimo.c $(DIR_LIB)/lib_biblio_estat.a -o $(DIR_BIN)/teste_emprestimo

$(DIR_BIN)/teste_usuario: $(DIR_TESTS)/teste_usuario.c $(DIR_LIB)/lib_biblio_estat.a | $(DIR_BIN)
	$(CC) -Iinclude -MMD -MP -MF $(DIR_BIN)/teste_usuario.d -MT $(DIR_BIN)/teste_usuario ./tests/teste_usuario.c $(DIR_LIB)/lib_biblio_estat.a -o $(DIR_BIN)/teste_usuario

#Install:
#Atribui valor nessa variável, caso ainda não tenha sido atribuido nada.
#O local será usado para instalar as bibliotecas.
PREFIX ?= /usr/local

install: $(DIR_LIB)/lib_biblio_estat.a $(DIR_LIB)/lib_biblio_din.so
	mkdir -p $(PREFIX)/lib
	mkdir -p $(PREFIX)/include
	cp $(DIR_LIB)/lib_biblio_estat.a $(PREFIX)/lib/
	cp $(DIR_LIB)/lib_biblio_din.so $(PREFIX)/lib/
	cp ./include/*.h $(PREFIX)/include/

.PHONY: all clean run_estatico run_dinamico run test install

.PHONY: lib_biblio_estat lib_biblio_din
#alias
lib_biblio_estat: $(DIR_LIB)/lib_biblio_estat.a
lib_biblio_din: $(DIR_LIB)/lib_biblio_din.so

#Linkagem
programa_estatico: $(DIR_OBJ)/main.o ./build/lib/lib_biblio_estat.a
	$(CC) -Iinclude $(DIR_OBJ)/main.o -L./build/lib -l_biblio_estat -o programa_estatico

programa_dinamico: $(DIR_OBJ)/main.o ./build/lib/lib_biblio_din.so
	$(CC) -Iinclude $(DIR_OBJ)/main.o -L./build/lib -l_biblio_din -Wl,-rpath,'$$ORIGIN/build/lib' -o programa_dinamico
#Foi necessário adicionar mais flags para que funcione sem instalação global.

#Compilação do main
$(DIR_OBJ)/main.o: ./src/main.c | $(DIR_OBJ)
	$(CC) -Iinclude -Wall -MMD -MP -c ./src/main.c -o $(DIR_OBJ)/main.o

#Biblioteca Estátíca
$(DIR_LIB)/lib_biblio_estat.a: $(DIR_OBJ)/catalogo_estat.o $(DIR_OBJ)/emprestimo_estat.o $(DIR_OBJ)/persistencia_estat.o $(DIR_OBJ)/usuario_estat.o | $(DIR_LIB)
	$(AR) $(AR_FLAGS) $(DIR_LIB)/lib_biblio_estat.a $(DIR_OBJ)/catalogo_estat.o $(DIR_OBJ)/emprestimo_estat.o $(DIR_OBJ)/persistencia_estat.o $(DIR_OBJ)/usuario_estat.o

$(DIR_OBJ)/catalogo_estat.o: ./src/catalogo.c | $(DIR_OBJ)
	$(CC) $(HEADERS_ESTAT) ./src/catalogo.c -o $(DIR_OBJ)/catalogo_estat.o

$(DIR_OBJ)/emprestimo_estat.o: ./src/emprestimo.c  | $(DIR_OBJ)
	$(CC) $(HEADERS_ESTAT) ./src/emprestimo.c -o $(DIR_OBJ)/emprestimo_estat.o

$(DIR_OBJ)/persistencia_estat.o: ./src/persistencia.c | $(DIR_OBJ)
	$(CC) $(HEADERS_ESTAT) ./src/persistencia.c -o $(DIR_OBJ)/persistencia_estat.o

$(DIR_OBJ)/usuario_estat.o: ./src/usuario.c | $(DIR_OBJ)
	$(CC) $(HEADERS_ESTAT) ./src/usuario.c -o $(DIR_OBJ)/usuario_estat.o

#Biblioteca Dinâmica
$(DIR_LIB)/lib_biblio_din.so: $(DIR_OBJ)/catalogo_din.o $(DIR_OBJ)/emprestimo_din.o $(DIR_OBJ)/persistencia_din.o $(DIR_OBJ)/usuario_din.o | $(DIR_LIB)
	$(CC) -shared -o $(DIR_LIB)/lib_biblio_din.so $(DIR_OBJ)/catalogo_din.o $(DIR_OBJ)/emprestimo_din.o $(DIR_OBJ)/persistencia_din.o $(DIR_OBJ)/usuario_din.o

$(DIR_OBJ)/catalogo_din.o: ./src/catalogo.c | $(DIR_OBJ)
	$(CC) $(HEADERS_DIN) ./src/catalogo.c -o $(DIR_OBJ)/catalogo_din.o

$(DIR_OBJ)/emprestimo_din.o: ./src/emprestimo.c | $(DIR_OBJ)
	$(CC) $(HEADERS_DIN) ./src/emprestimo.c -o $(DIR_OBJ)/emprestimo_din.o

$(DIR_OBJ)/persistencia_din.o: ./src/persistencia.c | $(DIR_OBJ)
	$(CC) $(HEADERS_DIN) ./src/persistencia.c -o $(DIR_OBJ)/persistencia_din.o

$(DIR_OBJ)/usuario_din.o: ./src/usuario.c | $(DIR_OBJ)
	$(CC) $(HEADERS_DIN) ./src/usuario.c -o $(DIR_OBJ)/usuario_din.o

#Considerar os .d, eles vão verificar se of .h foram alterados.
#Inclui automaticamente as dependências dos headers
-include $(wildcard $(DIR_OBJ)/*.d)
-include $(wildcard $(DIR_BIN)/*.d)