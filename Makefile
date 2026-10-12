CC = gcc

HEADERS_ESTAT = -Iinclude -Wall -c

HEADERS_DIN = -Iinclude -fPIC -Wall -c

AR = ar

AR_FLAGS = -rcv

DIR_OBJ = ./build/obj

DIR_LIB = ./build/lib


.PHONY: lib_biblio_estat lib_biblio_din
#alias
lib_biblio_estat: $(DIR_LIB)/lib_biblio_estat.a
lib_biblio_din: $(DIR_LIB)/lib_biblio_din.so

#Linkagem
programa_estatico: ./src/main.c ./build/lib/lib_biblio_estat.a
	$(CC) -Iinclude ./src/main.c -L./build/lib -l_biblio_estat -o programa_estatico

programa_dinamico: ./src/main.c
	$(CC) -Iinclude ./src/main.c -L./build/lib -l_biblio_estat -o programa_dinamico

#Biblioteca Estátíca
$(DIR_LIB)/lib_biblio_estat.a: $(DIR_OBJ)/catalogo_estat.o $(DIR_OBJ)/emprestimo_estat.o $(DIR_OBJ)/persistencia_estat.o $(DIR_OBJ)/set_estat.o $(DIR_OBJ)/usuario_estat.o 
	$(AR) $(AR_FLAGS) $(DIR_LIB)/lib_biblio_estat.a $(DIR_OBJ)/catalogo_estat.o $(DIR_OBJ)/emprestimo_estat.o $(DIR_OBJ)/persistencia_estat.o $(DIR_OBJ)/set_estat.o $(DIR_OBJ)/usuario_estat.o

$(DIR_OBJ)/catalogo_estat.o: ./src/catalogo.c
	$(CC) $(HEADERS_ESTAT) ./src/catalogo.c -o $(DIR_OBJ)/catalogo_estat.o

$(DIR_OBJ)/emprestimo_estat.o: ./src/emprestimo.c
	$(CC) $(HEADERS_ESTAT) ./src/emprestimo.c -o $(DIR_OBJ)/emprestimo_estat.o

$(DIR_OBJ)/persistencia_estat.o: ./src/persistencia.c
	$(CC) $(HEADERS_ESTAT) ./src/persistencia.c -o $(DIR_OBJ)/persistencia_estat.o

$(DIR_OBJ)/set_estat.o: ./src/set.c
	$(CC) $(HEADERS_ESTAT) ./src/set.c -o $(DIR_OBJ)/set_estat.o

$(DIR_OBJ)/usuario_estat.o: ./src/usuario.c
	$(CC) $(HEADERS_ESTAT) ./src/usuario.c -o $(DIR_OBJ)/usuario_estat.o

#Biblioteca Dinâmica
$(DIR_LIB)/lib_biblio_din.so: $(DIR_OBJ)/catalogo_din.o $(DIR_OBJ)/emprestimo_din.o $(DIR_OBJ)/persistencia_din.o $(DIR_OBJ)/set_din.o $(DIR_OBJ)/usuario_din.o 
	$(CC) -shared -o $(DIR_LIB)/lib_biblio_din.so $(DIR_OBJ)/catalogo_din.o $(DIR_OBJ)/emprestimo_din.o $(DIR_OBJ)/persistencia_din.o $(DIR_OBJ)/set_din.o $(DIR_OBJ)/usuario_din.o

$(DIR_OBJ)/catalogo_din.o: ./src/catalogo.c
	$(CC) $(HEADERS_DIN) ./src/catalogo.c -o $(DIR_OBJ)/catalogo_din.o

$(DIR_OBJ)/emprestimo_din.o: ./src/emprestimo.c
	$(CC) $(HEADERS_DIN) ./src/emprestimo.c -o $(DIR_OBJ)/emprestimo_din.o

$(DIR_OBJ)/persistencia_din.o: ./src/persistencia.c
	$(CC) $(HEADERS_DIN) ./src/persistencia.c -o $(DIR_OBJ)/persistencia_din.o

$(DIR_OBJ)/set_din.o: ./src/set.c
	$(CC) $(HEADERS_DIN) ./src/set.c -o $(DIR_OBJ)/set_din.o

$(DIR_OBJ)/usuario_din.o: ./src/usuario.c
	$(CC) $(HEADERS_DIN) ./src/usuario.c -o $(DIR_OBJ)/usuario_din.o