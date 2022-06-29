 %{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"
#include "arbol.h"

int yystopparser=0;
FILE  *yyin;

int yylex();
int yyerror();
extern struct struct_tablaSimbolos tablaSimbolos[1000]; 
extern int puntero_array;
int contadorTipos = 0;
int contadorVar = 0;
char* auxTipoDato;
char matrizTipoDato[100][10];
char matrizVariables[100][10];
int contadorId = 0;
int agregarTipoEnTablaSimbolos(char* nombre, int contadorId);
void mensaje_error(char*);
void escribirEnTablaSimbolos();

FILE *f_intermedia;
FILE *f_dot_arbol;
FILE *f_asm;

// Declaración punteros arbol
t_nodo* ptr_star; //star
t_nodo* ptr_prog; //programa
t_nodo* ptr_zona; //zona_declaracion
t_nodo* ptr_decls; //declaraciones
t_nodo* ptr_decl; //declaracion
t_nodo* ptr_list_dec; //lista_declaracion
t_nodo* ptr_list_var; //lista_var
t_nodo* ptr_list_tip; //lista_tipo
t_nodo* ptr_algo; //algoritmo
t_nodo* ptr_bloq; //bloque
t_nodo* ptr_sub_bloq;
t_nodo* ptr_sent; //sentencia
t_nodo* ptr_cicl; //ciclo
t_nodo* ptr_asig; //asignacion
t_nodo* ptr_sele; //seleccion
t_nodo* ptr_true; //Rama verdadera
t_nodo* ptr_false;//Rama falsa
t_nodo* ptr_cond; //condicion
t_nodo* ptr_cond_aux;
t_nodo* ptr_comp; //comparacion
t_nodo* ptr_comp_aux;
t_nodo* ptr_expr; //expresion
t_nodo* ptr_inli; //inlist
t_nodo* ptr_inli_id;
t_nodo* ptr_list_exp; //lista_expresiones
t_nodo* ptr_betw; //between
t_nodo* ptr_betw_from;
t_nodo* ptr_betw_to;
t_nodo* ptr_term; //termino
t_nodo* ptr_fact; //factor
t_nodo* ptr_entr; //entrada
t_nodo* ptr_sali; //salida

//flags
int and_flag, or_flag;

%}
%token PROGRAM
%token END
%token IF
%token THEN
%token ENDIF
%token ELSE
%token WHILE
%token WRITE
%token READ
%token BETWEEN
%token INLIST
%token DECVAR
%token ENDDEC
%token COMP_IGUAL
%token COMP_MAYOR
%token COMP_MENOR
%token COMP_MAYOR_IGUAL
%token COMP_MENOR_IGUAL
%token COMP_DISTINTO
%token OPAR_ASIG
%token TIPO_INT
%token TIPO_FLOAT
%token TIPO_STRING
%token <num>CTE_ENTERA
%token <real>CTE_REAL
%token <str>CTE_STRING
%token OP_MAS 
%token OP_MENOS
%token OP_MULT
%token OP_DIV
%token OP_LOG_AND
%token OP_LOG_OR
%token OP_LOG_NOT
%token DOS_PUNTOS
%token PUN_Y_COM
%token COMA
%token <strid>ID
%token PAR_A
%token PAR_C
%token LLAVE_A
%token LLAVE_C
%token COR_A
%token COR_C

%union{
char * strid;
char * num;
char * real; 
char * str;
}

%%
start: programa {ptr_star = ptr_prog; inOrder(&ptr_star, f_intermedia);};

programa: PROGRAM zona_declaracion algoritmo END {ptr_prog = crearNodo("programa", ptr_algo, NULL); printf("\n***** Compilacion exitosa: OK *****\n");};
				  
zona_declaracion:	declaraciones {ptr_zona = ptr_decls;};

declaraciones:	declaracion {ptr_decls = ptr_decl;}
				|declaraciones declaracion {
					ptr_decls = crearNodo("declaraciones", ptr_decls, ptr_decl);};

declaracion:	DECVAR { printf("***** Inicio declaracion de variables *****\n"); } lista_declaracion ENDDEC {ptr_decl= ptr_list_dec; printf("*****\n Fin declaracion de variables *****\n");};

lista_declaracion:	lista_var DOS_PUNTOS lista_tipo {ptr_list_dec = crearNodo("dec", ptr_list_var, ptr_list_tip);}
					| lista_declaracion lista_var DOS_PUNTOS lista_tipo {ptr_list_dec = crearNodo("lista_dec_vars", ptr_list_dec, crearNodo("dec", ptr_list_var, ptr_list_tip));};


lista_var:		ID {strcpy(matrizVariables[contadorId],yylval.strid) ;  contadorId++;contadorVar++;
					ptr_list_var = crearHoja($1);
					}
				| lista_var COMA ID {strcpy(matrizVariables[contadorId],yylval.strid) ; contadorId++;contadorVar++;
									ptr_list_var = crearNodo("list_var", ptr_list_var, crearHoja($3));
									};

 
lista_tipo:		TIPO_INT { auxTipoDato="int"; ptr_list_tip = crearHoja(auxTipoDato); for(int i = 0; i < contadorVar; ++i){strcpy(matrizTipoDato[contadorTipos],auxTipoDato); agregarTipoEnTablaSimbolos(matrizVariables[contadorTipos],contadorTipos); contadorTipos++; printf(" INT");} contadorVar=0; }
				|TIPO_FLOAT {  auxTipoDato="float"; ptr_list_tip = crearHoja(auxTipoDato); for(int i = 0; i < contadorVar; ++i){strcpy(matrizTipoDato[contadorTipos],auxTipoDato); agregarTipoEnTablaSimbolos(matrizVariables[contadorTipos],contadorTipos); contadorTipos++; printf(" REAL"); }contadorVar = 0; }
				|TIPO_STRING { auxTipoDato="string"; ptr_list_tip = crearHoja(auxTipoDato); for(int i = 0; i < contadorVar; ++i){strcpy(matrizTipoDato[contadorTipos],auxTipoDato); agregarTipoEnTablaSimbolos(matrizVariables[contadorTipos],contadorTipos); contadorTipos++; printf(" STRING");}contadorVar = 0; };
              

algoritmo:		bloque {ptr_algo = ptr_bloq; printf("\n***** Fin de bloque *****\n");};

bloque:			sentencia {if(ptr_bloq == NULL){ptr_bloq = ptr_sent;}else{ptr_bloq = crearNodo("bloque", ptr_bloq, ptr_sent);}}
				|bloque sentencia {ptr_bloq = crearNodo("bloque", ptr_bloq, ptr_sent);};

sub_bloque:		sentencia {ptr_sub_bloq = ptr_sent;}
				|sub_bloque sentencia {ptr_sub_bloq = crearNodo("sub_bloque", ptr_sub_bloq, ptr_sent);};


sentencia:		asignacion { ptr_sent = ptr_asig; printf(" - asignacion - OK \n"); }
				|seleccion { ptr_sent = ptr_sele; printf(" - seleccion - OK \n"); }
				|ciclo { ptr_sent = ptr_cicl; printf(" - ciclo - OK \n"); }
				|entrada { ptr_sent = crearNodo("entrada", ptr_entr, NULL); printf(" - entrada - OK \n"); }
				|salida { ptr_sent = crearNodo("salida", ptr_sali, NULL); printf(" - salida - OK \n"); };

ciclo:			WHILE PAR_A condicion PAR_C LLAVE_A sub_bloque LLAVE_C {ptr_cicl = crearNodo("ciclo", ptr_cond, ptr_sub_bloq);};
       
asignacion:		ID OPAR_ASIG expresion {ptr_asig = crearNodo(":=", crearHoja($1), ptr_expr);};
                  
          
seleccion: 		IF  PAR_A condicion PAR_C THEN sub_bloque ENDIF {
																	if(and_flag){
																		ptr_sele = crearNodo("if", ptr_cond,crearNodo("if",ptr_cond_aux,ptr_sub_bloq));
																		and_flag = 0;
																	}else if(or_flag) {
																		ptr_sele = crearNodo("if", ptr_cond,crearNodo("else", ptr_sub_bloq, crearNodo("if",ptr_cond_aux,ptr_sub_bloq)));
																		or_flag = 0;
																	}else{
																		ptr_sele = crearNodo("if", ptr_cond, ptr_sub_bloq);
																	}
																}
				| IF  PAR_A condicion PAR_C THEN sub_bloque {ptr_true = ptr_sub_bloq;} ELSE sub_bloque {ptr_false = ptr_sub_bloq;} ENDIF {
																	if(and_flag){
																		ptr_sele = crearNodo("if", ptr_cond, crearNodo("else",crearNodo("if",ptr_cond_aux,crearNodo("else",ptr_true,ptr_false)),ptr_false));
																		and_flag = 0;
																	}else if(or_flag) {
																		ptr_sele = crearNodo("if", ptr_cond, crearNodo("else",ptr_true,crearNodo("if",ptr_cond_aux,crearNodo("else",ptr_true,ptr_false))));
																		or_flag = 0;
																	}else{
																		ptr_sele = crearNodo("if", ptr_cond, crearNodo("else", ptr_true, ptr_false));
																	}
																};

condicion:		comparacion {ptr_cond = ptr_comp;}
				|comparacion OP_LOG_AND {ptr_cond_aux = ptr_comp; } comparacion {and_flag = 1;ptr_cond = ptr_comp;}
				|comparacion OP_LOG_OR {ptr_cond_aux = ptr_comp; } comparacion	{or_flag = 1;ptr_cond = ptr_comp;}
				|OP_LOG_NOT comparacion {invertirOperador(ptr_comp;); ptr_cond = ptr_comp;}
				|inlist { ptr_cond = ptr_inli; printf(" - inlist - OK \n"); };
				|between { ptr_cond = ptr_betw; printf(" - between - OK \n"); };

comparacion:	expresion {ptr_comp_aux = ptr_expr;} COMP_IGUAL expresion {ptr_comp = crearNodo("==",ptr_comp_aux,ptr_expr);}  
				|expresion {ptr_comp_aux = ptr_expr;} COMP_MAYOR expresion	{ptr_comp = crearNodo(">",ptr_comp_aux,ptr_expr);}   
				|expresion {ptr_comp_aux = ptr_expr;} COMP_MENOR expresion {ptr_comp = crearNodo("<",ptr_comp_aux,ptr_expr);}  
				|expresion {ptr_comp_aux = ptr_expr;} COMP_MAYOR_IGUAL expresion  {ptr_comp = crearNodo(">=",ptr_comp_aux,ptr_expr);}  
				|expresion {ptr_comp_aux = ptr_expr;} COMP_MENOR_IGUAL expresion {ptr_comp = crearNodo("<=",ptr_comp_aux,ptr_expr);}  
				|expresion {ptr_comp_aux = ptr_expr;} COMP_DISTINTO expresion {ptr_comp = crearNodo("!=",ptr_comp_aux,ptr_expr);} ;


expresion:		expresion { printf(" expresion"); } OP_MAS termino { printf(" termino"); ptr_expr=crearNodo("+",ptr_expr,ptr_term); }
				|expresion { printf(" expresion"); }OP_MENOS termino { printf(" termino"); ptr_expr=crearNodo("-",ptr_expr,ptr_term);}
				|termino { printf(" termino"); ptr_expr=ptr_term; };
				
inlist:			INLIST PAR_A ID {ptr_inli_id = crearHoja($3);} PUN_Y_COM COR_A lista_expresiones COR_C PAR_C {
					ptr_inli = crearNodo("inlist", crearHoja($3), ptr_list_exp);
				};

lista_expresiones:	lista_expresiones PUN_Y_COM expresion {
						ptr_list_exp = crearNodo("list_exp",
											crearNodo(";",
												crearNodo(":=", crearHoja("@aux"), ptr_expr),
												crearNodo("if", 
													crearNodo("==", crearHoja("@aux"), ptr_inli_id),
													crearNodo("else", crearHoja("true"), ptr_list_exp)
													)
												),
												NULL
											);
										}
                    | expresion {ptr_list_exp = crearNodo(";", 
													crearNodo(":=",
														crearHoja("@aux"),
														ptr_expr),
													crearNodo("if",
														crearNodo("==", ptr_inli_id, crearHoja("@aux")),
														crearHoja("true")
														)
													);
								};
					
between:		BETWEEN PAR_A ID COMA COR_A expresion {ptr_betw_from = crearNodo(":=", crearHoja("@from_aux"),ptr_expr);} PUN_Y_COM expresion {ptr_betw_to = crearNodo(":=", crearHoja("@to_aux"),ptr_expr);} COR_C PAR_C {
					ptr_betw = crearNodo(";", 
									ptr_betw_from,
									crearNodo("if", 
										crearNodo(">=", 
											crearHoja($3), 
											crearHoja("@from_aux")),
										crearNodo(";", 
											ptr_betw_to,
											crearNodo("if",
												crearNodo("<=", 
													crearHoja($3),
													crearHoja("@to_aux")),
												crearNodo("else",
													crearHoja("true"),
													crearHoja("false")
												)
											)
										)
									)
								);
};

termino:		termino OP_MULT factor { printf(" factor"); ptr_term=crearNodo("*",ptr_term,ptr_fact);}
				|termino OP_DIV factor { printf(" factor"); ptr_term=crearNodo("/",ptr_term,ptr_fact);}
				|factor { printf(" factor"); ptr_term=ptr_fact; };
                         

factor:			ID {ptr_fact = crearHoja($1); }
				|CTE_ENTERA {auxTipoDato = "_"; ptr_fact = crearHoja(strcat(auxTipoDato, $1)); }
				|CTE_REAL {auxTipoDato = "_"; ptr_fact = crearHoja(strcat(auxTipoDato, $1)); }
				|CTE_STRING {auxTipoDato = "_"; ptr_fact = crearHoja(strcat(auxTipoDato, $1)); }
				|PAR_A expresion PAR_C {;}
				;
 
entrada: 		READ ID {ptr_entr = crearHoja($2);};

salida:			WRITE CTE_STRING {ptr_sali = crearHoja($2);}
				|WRITE ID {ptr_sali = crearHoja($2);}; 
          
          
%%
 
int main(int argc,char *argv[])
{
  if ((yyin = fopen(argv[1], "rt")) == NULL){
	printf("\nERROR! No se pudo abrir el archivo: %s\n", argv[1]);
	return 1;
  }

  if ((f_intermedia = fopen("intermedia.txt", "wt")) == NULL){
	printf("\nERROR! No se pudo abrir el archivo intermedia\n");
	return 1;
  }

  if ((f_dot_arbol = fopen("arbol.dot", "wt")) == NULL){
	printf("\nERROR! No se pudo abrir el archivo .dot para armar el arbol\n");
	return 1;
  }

  if ((f_asm = fopen("Final.asm", "wt")) == NULL){
	printf("\nERROR! No se pudo abrir el archivo Final.asm para armar el programa\n");
	return 1;
  }


  or_flag = and_flag = 0;
  
  yyparse();
  escribirEnTablaSimbolos();
  fclose(yyin);
  fclose(f_intermedia);
  generarDOT(&ptr_star, f_dot_arbol);
  system ("Pause");
  return 0;
}

int agregarTipoEnTablaSimbolos(char* nombre, int contadorTipos)
{     
		for(int i = 0; i < puntero_array; i++)
		{
			if(strcmp(tablaSimbolos[i].nombre, nombre) == 0)
			{
				if(tablaSimbolos[i].tipo[0] == '\0')
				strcpy(tablaSimbolos[i].tipo,matrizTipoDato[contadorTipos]);
		  
				return 1; 
			}
		}
	
	return 0;	
}
