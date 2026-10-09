#ifndef SET_H
#define SET_H
    #include <stdlib.h>
    typedef struct Set Set;
    typedef enum {
        INT,
        FLOAT,
        DOUBLE,
        CHAR,
        STRING
    } Tipo;
    Set* set_create(Tipo t);
    int set_countains(const Set* set, int x);
    int set_insert(Set* set, int x);
    int set_remove(Set* set, int x);
    size_t set_size(Set* set);
    int set_empty(Set* set);
#endif