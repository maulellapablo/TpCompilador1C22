#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ts.h"

typedef struct nodo{
    struct nodo* izq;
    struct nodo* der;
    char data[45];
}t_nodo;

typedef t_nodo* t_arbol;

t_nodo* crearHoja( char* lexema);
t_nodo* crearNodo( char* lexema, t_nodo* hijoIzq, t_nodo* hijoDer);
void inOrden(t_arbol *pa, FILE *pIntermedia);
char* replace_char(char* str, char find, char replace);
void generarAssembler(t_arbol *pa, FILE *f, struct struct_tablaSimbolos* ts);
void  printTablaDeSimbolosAsm(struct struct_tablaSimbolos *ts, FILE *f);
t_arbol* inOrderAssembler(t_arbol *pa, FILE *f);
int esHoja(t_arbol* pa);
void invertirOperador(t_nodo* n);

extern struct struct_tablaSimbolos tablaSimbolos[1000]; 
extern int puntero_array;
int contAux = 0;
int contSalto = 0;
char str_aux[20];

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

void generarAssembler(t_arbol *pa, FILE *f_asm, struct struct_tablaSimbolos* ts){
	char Linea[300];

	FILE *f_temp = fopen("Temp.asm", "wt");

	while(inOrderAssembler(pa, f_temp) != pa){}
 
	fclose(f_temp);

	f_temp = fopen("Temp.asm", "rt");

	fprintf(f_asm, "include macros2.asm\ninclude number.asm\n.MODEL LARGE	; Modelo de Memoria\n.386	        ; Tipo de Procesador\n.STACK 200h		; Bytes en el Stack\n\n.DATA \n\n");

	printTablaDeSimbolosAsm(ts, f_asm);

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
       if((!strncmp(ts[i].nombre, "_", 1)) && (strcmp(ts[i].tipo, "int") == 0)) //Es CTE Entera
        {
            strcat(ts[i].valor, ".00");
            fprintf(f, "%-30s%-30s%-30s%-s %-s\n", ts[i].nombre, "dd", ts[i].valor, ";Cte en formato ", ts[i].tipo);
        }
        else if(!strncmp(ts[i].nombre, "_", 1)) { // Es CTE
            if(strcmp(ts[i].tipo,"string") == 0)
				fprintf(f, "%-30s%-30s \"%s\"%-26s%-30s %-s\n", replace_char(ts[i].nombre,' ','_'), "dd", ts[i].valor, "", ";Cte en formato ", ts[i].tipo);
			else
				fprintf(f, "%-30s%-30s%-30s%-s %-s\n", replace_char(ts[i].nombre,' ','_'), "dd", ts[i].valor, ";Cte en formato ", ts[i].tipo);
		}
        else if(strncmp(ts[i].nombre, "_", 1)) //Es variable
            fprintf(f, "%-30s%-30s%-30s%-s %-s\n", ts[i].nombre, "dd", "?", ";Variable", ts[i].tipo);
    }
}

int esHoja(t_arbol* pa){
    if(!*pa)
        return 0;
 
    return (!(*pa)->izq) && (!(*pa)->der);
}

void invertirOperador(t_nodo* n){
    if(n->data == "=="){
        strcpy(n->data, "!=");
        return;
    }
    if(n->data == "!="){
        strcpy(n->data, "==");
        return;
    }
    if(n->data == "<"){
        strcpy(n->data, ">=");
        return;
    }
    if(n->data == ">="){
        strcpy(n->data, "<");
        return;
    }
    if(n->data == ">"){
        strcpy(n->data, "<=");
        return;
    }
    if(n->data == "<="){
        strcpy(n->data, ">");
        return;
    }
}

t_arbol* inOrderAssembler(t_arbol *pa, FILE *f){

    if(*pa==NULL )
        return NULL;
    
    if(strcmp((*pa)->data, "ciclo")==0 ){
        traduccionAssembler(pa,f);
    }
    inOrderAssembler(&(*pa)->izq, f);
 
     if(strcmp((*pa)->data, "bloque")==0 ){
        traduccionAssembler(pa,f);
    }
   
    inOrderAssembler(&(*pa)->der, f);

    if(esHoja(&(*pa)->izq) && ((esHoja(&(*pa)->der)) || (*pa)->der == NULL)){
        traduccionAssembler(pa,f);
        return pa;
    }
}

void traduccionAssembler(t_arbol* pa, FILE* f){
       if(!*pa)
        return;
        char cadena[50]="";
        if(strcmp((*pa)->data, ">")==0 ){
            strcat(cadena,"JNA\0");
             traduccionCond(pa,f,cadena);
            return;
        }else if (strcmp((*pa)->data, "<")==0 ){
            strcat(cadena,"JNB\0");
             traduccionCond(pa,f,cadena);
            return;
        }else if (strcmp((*pa)->data, ">=")==0 ){
            strcat(cadena,"JNAE\0");
             traduccionCond(pa,f,cadena);
            return;
        }else if (strcmp((*pa)->data, "<=")==0 ){
             strcat(cadena,"JNBE\0");
             traduccionCond(pa,f,cadena);
            return;
        }else if (strcmp((*pa)->data, "==")==0 ){
             strcat(cadena,"JNE\0");
             traduccionCond(pa,f,cadena); 
            return;
        }else if (strcmp((*pa)->data, "!=")==0 ){
             strcat(cadena,"JE\0");
             traduccionCond(pa,f,cadena);
            return;
        }

    int tam=strlen("bloque");
    strncpy( cadena, (*pa)->data, tam);
    cadena[tam]='\0';
    if(strcmp(cadena, "bloque")!=0 && strcmp(cadena, "sub_bloque")!=0 ){
        if(strcmp((*pa)->data,":=")!=0)
            fprintf(f,"FLD %s\n", ((*pa)->izq)->data);
        fprintf(f,"FLD %s\n",((*pa)->der)->data);
    
        if(strcmp((*pa)->data, "+")==0)
            fprintf(f,"FADD \n");
        else if(strcmp((*pa)->data, "-")==0)
            fprintf(f,"FSUB \n");
        else if(strcmp((*pa)->data, "/")==0)
            fprintf(f,"FDIV \n");
        else if(strcmp((*pa)->data, "*")==0)
            fprintf(f,"FMUL \n");
        
        if(strcmp((*pa)->data,":=")==0){
            fprintf(f,"FSTP %s\n",((*pa)->izq)->data);    
        }else{
            sprintf(cadena,"@Aux%d",++contAux);
            fprintf(f,"FSTP %s\n",cadena);
            strcpy((*pa)->data, cadena);
            guardarEnTablaSimbolos("float", cadena);
        }
        fprintf(f,"FFREE\n"); 
    }
    free((*pa)->izq);
    free((*pa)->der);

    (*pa)->izq = NULL;
    (*pa)->der = NULL;
}

void traduccionCond(t_arbol* pa, FILE* f, char* salto){
     if(!*pa)
        return;
	
	fprintf(f,"FLD %s%s\n", ((*pa)->izq)->data);
	fprintf(f,"FCOMP %s%s\n", ((*pa)->der)->data);
    fprintf(f,"FSTSW ax\n");
    fprintf(f,"SAHF\n");
    sprintf(str_aux, "saltoelse%d", contSalto);
    fprintf(f,"%s %s\n", salto, str_aux);

     free((*pa)->izq);
     free((*pa)->der);

    (*pa)->izq = NULL;
    (*pa)->der = NULL;
}


#endif // ARBOL_H_INCLUDED