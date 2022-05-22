#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nodo{
    struct nodo* izq;
    struct nodo* der;
    char data[45];
}t_nodo;

typedef t_nodo* t_arbol;

t_nodo* crearHoja( char* lexema);
t_nodo* crearNodo( char* lexema, t_nodo* hijoIzq, t_nodo* hijoDer);

t_nodo* crearHoja( char* lexema){
    t_nodo* nodo = (t_nodo*) malloc (sizeof(t_nodo));
    if(!nodo){
        printf("No se pudo reservar memoria para el nodo.\n");
        return NULL;
    }
    //Ponemos izq y der en NULL, y almacenamos el lexema dentro del t_nodo
    nodo->izq = NULL;
    nodo->der = NULL;
    strcpy(nodo->data, lexema);
    return nodo;
}

t_nodo* crearNodo(char* lexema, t_nodo* hijoIzq, t_nodo* hijoDer){
    t_nodo* padre = crearHoja(lexema);
    if(!padre){
        return NULL;
    }

    padre->izq = hijoIzq;
    padre->der = hijoDer;

    return padre;
}

#endif // ARBOL_H_INCLUDED