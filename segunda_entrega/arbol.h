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
void inOrden(t_arbol *pa, FILE *pIntermedia);
char *str_replace(char *orig, char *rep, char *with);

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

void inOrder(t_arbol *pa, FILE *pIntermedia)
{
    if(!(*pa))
        return;
    inOrder(&(*pa)->izq, pIntermedia);
	printf(" %s  ", (*pa)->data);
    fprintf(pIntermedia, " %s  ", (*pa)->data);  
    inOrder(&(*pa)->der, pIntermedia);
}


void grabarNodoDOT(t_nodo *pn, FILE* stream, int* numero)
{
    int thisId = (*numero);
    fprintf(stream, "id%d [label = \"%s\"];\n", thisId, str_replace((*pn).data,'"','\"'));

    if ((*pn).izq)
    {
        int izqId = ++(*numero);
        grabarNodoDOT((*pn).izq, stream, numero);
        fprintf(stream, "id%d -> id%d ;\n", thisId , izqId);
    }

    if ((*pn).der)
    {
        int derId = ++(*numero);
        grabarNodoDOT((*pn).der, stream, numero);
        fprintf(stream, "id%d -> id%d ;\n", thisId , derId);
    }
}

void generarDOT(t_arbol* pa, FILE* stream)
{
    fprintf(stream, "digraph BST {\n");
    fprintf(stream, "    node [fontname=\"Arial\"];\n");

    if(!(*pa))
        fprintf(stream, "\n");
    else if (!(*pa)->der && !(*pa)->izq)
        fprintf(stream, "    \"%s\";\n", (*pa)->data);
    else{
        int numero = 1;
        grabarNodoDOT((*pa), stream, &numero);
    }

    fprintf(stream, "}\n");
}


// Función de ayuda, replace string
// You must free the result if result is non-NULL.
char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

#endif // ARBOL_H_INCLUDED