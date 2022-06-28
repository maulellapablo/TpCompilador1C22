#ifndef TS_H_INCLUDED
#define TS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct struct_tablaSimbolos
{
	char nombre[100];
	char tipo[100];
	char valor[50];
	char longitud[100];
};

int puntero_array = 0;
struct struct_tablaSimbolos tablaSimbolos[1000];

int guardarEnTablaSimbolos(char*, char*);

int guardarEnTablaSimbolos(char* tipo, char* nombre){
	
	char longitudConstanteString[10];
	int i;
	int posicion;
	char lexema[50];
	char aux[50];

	if(strcmp(strncpy(aux, tipo, 3),"CTE")==0){
		lexema[0]='_';
		lexema[1]='\0';
		//Se anexa al lexema un guion bajo al inicio
		strcat(lexema,nombre);
	}else{
		lexema[0]='\0';
		strcat(lexema,nombre);
	}
	
	//Recorremos la tabla de simbolos y en caso que el lexema ya exista, no se agrega y se retorna su posicion
	for(i = 0; i < puntero_array; i++)
	{
		if(strcmp(tablaSimbolos[i].nombre, lexema) == 0)
		{
			return i;
		}
	}
	
	//En caso de ser una CTE, guardamos el Valor en la tabla de simbolos
	if(strcmp(strncpy(aux, tipo, 3),"CTE")==0)
		strcpy(tablaSimbolos[puntero_array].valor, nombre);
	else
		tablaSimbolos[puntero_array].valor[0]='\0';
		
	strcpy(tablaSimbolos[puntero_array].nombre, lexema );

	tablaSimbolos[puntero_array].tipo[0]='\0';
	if(strcmp(tipo,"CTE_ENTERA")==0)
		strcat(tablaSimbolos[puntero_array].tipo, "int");
	if(strcmp(tipo,"CTE_STRING")==0)
		strcat(tablaSimbolos[puntero_array].tipo, "string");
	if(strcmp(tipo,"CTE_REAL")==0)
		strcat(tablaSimbolos[puntero_array].tipo, "float");

	//En caso de ser una CTE string, se cuentan los caracteres y se guardan en la tabla de simbolos
	if(tablaSimbolos[i].valor[0] == '\"')
	{
		snprintf(longitudConstanteString, sizeof(longitudConstanteString), "%d", strlen(tablaSimbolos[i].valor)-2);
		strcpy(tablaSimbolos[i].longitud,longitudConstanteString);
	}
	else
		tablaSimbolos[puntero_array].longitud[0]='0';
	
	//Se agrega en la tabla de simbolos y se retorna su posicion
	posicion = puntero_array;
	puntero_array++;
	
	return posicion;
}

#endif