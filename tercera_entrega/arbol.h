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

struct struct_tablaSimbolos
{
	char nombre[100];
	char tipo[100];
	char valor[50];
	char longitud[100];
};

t_nodo* crearHoja( char* lexema);
t_nodo* crearNodo( char* lexema, t_nodo* hijoIzq, t_nodo* hijoDer);
void inOrden(t_arbol *pa, FILE *pIntermedia);
char* replace_char(char* str, char find, char replace);
void generarAssembler(t_arbol *pa, FILE *f);
void  printTablaDeSimbolosAsm(struct struct_tablaSimbolos* ts, FILE* f);
t_arbol* recorrerArbol(t_arbol *pa, FILE *f, struct struct_tablaSimbolos* ts);

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
    fprintf(stream, "id%d [label = \"%s\"];\n", thisId, replace_char((*pn).data,'"','\''));

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
char* replace_char(char* str, char find, char replace){
    char *current_pos = strchr(str,find);
    while (current_pos) {
        *current_pos = replace;
        current_pos = strchr(current_pos,find);
    }
    return str;
}

void generarAssembler(t_arbol *pa, FILE *f){
	char Linea[300];

	FILE *f_temp = fopen("Temp.asm", "wt");

	while(recorrerArbol(pArbol,f_temp, &tablaSimbolos) != pArbol){}
 
	fclose(f_temp);

	f_temp = fopen("Temp.asm", "rt");

	fprintf(f_asm, "include macros2.asm\ninclude number.asm\n.MODEL LARGE	; Modelo de Memoria\n.386	        ; Tipo de Procesador\n.STACK 200h		; Bytes en el Stack\n\n.DATA \n\n");

	printTablaDeSimbolosAsm(&tablaSimbolos, f_asm);

	fprintf(f_asm, "\n\n.Temp\n\nmov AX,@DATA    ; Inicializa el segmento de datos\nmov DS,AX\nmov es,ax ;\n\n");

	while(fgets(Linea, sizeof(Linea), f_temp))
	{
		fprintf(f_asm, Linea);
	}

	fprintf(f_asm, "\n\n\nmov ax,4c00h	; Indica que debe finalizar la ejecución\nint 21h\n\nEnd");

	fclose(f_temp);

	remove("Temp.asm");

	fclose(f_asm);
}

void  printTablaDeSimbolosAsm(struct struct_tablaSimbolos* ts, FILE* f){
    for(int i = 0; i < sizeof(*ts) / sizeof(struct struct_tablaSimbolos); i++){
       if((!strncmp((*ts[i])->nombre, "_", 1)) && (strcmp((*ts[i])->tipo, "int") == 0)) //Es CTE Entera
        {
            strcat((*ts[i])->valor, ".00");
            fprintf(f, "%-30s%-30s%-30s%-s %-s\n", (*ts[i])->nombre, "dd", (*ts[i])->valor, ";Cte en formato ", (*ts[i])->tipo);
        }
        else if(!strncmp((*ts[i])->nombre, "_", 1)) { // Es CTE
            if(strcmp((*ts[i])->tipo,"string") == 0)
				fprintf(f, "%-30s%-30s \"%s\"%-26s%-30s %-s\n", str_replace((*ts[i])->nombre," ","_"), "dd", (*ts[i])->valor, "", ";Cte en formato ", (*ts[i])->tipo);
			else
				fprintf(f, "%-30s%-30s%-30s%-s %-s\n", str_replace((*ts[i])->nombre," ","_"), "dd", (*ts[i])->valor, ";Cte en formato ", (*ts[i])->tipo);
		}
        else if(strncmp((*ts[i])->nombre, "_", 1)) //Es variable
            fprintf(f, "%-30s%-30s%-30s%-s %-s\n", (*ts[i])->nombre, "dd", "?", ";Variable", (*ts[i])->tipo);
    }
}

t_arbol* recorrerArbol(t_arbol *pa, FILE *f, struct struct_tablaSimbolos* ts){
    return pa;
}

#endif // ARBOL_H_INCLUDED