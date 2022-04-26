
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     PROGRAM = 258,
     END = 259,
     IF = 260,
     THEN = 261,
     ENDIF = 262,
     ELSE = 263,
     WHILE = 264,
     WRITE = 265,
     READ = 266,
     BETWEEN = 267,
     INLIST = 268,
     DECVAR = 269,
     ENDDEC = 270,
     COMP_IGUAL = 271,
     COMP_MAYOR = 272,
     COMP_MENOR = 273,
     COMP_MAYOR_IGUAL = 274,
     COMP_MENOR_IGUAL = 275,
     COMP_DISTINTO = 276,
     OPAR_ASIG = 277,
     TIPO_INT = 278,
     TIPO_FLOAT = 279,
     TIPO_STRING = 280,
     CTE_ENTERA = 281,
     CTE_REAL = 282,
     CTE_STRING = 283,
     OP_MAS = 284,
     OP_MENOS = 285,
     OP_MULT = 286,
     OP_DIV = 287,
     OP_LOG_AND = 288,
     OP_LOG_OR = 289,
     OP_LOG_NOT = 290,
     DOS_PUNTOS = 291,
     PUN_Y_COM = 292,
     COMA = 293,
     ID = 294,
     PAR_A = 295,
     PAR_C = 296,
     LLAVE_A = 297,
     LLAVE_C = 298,
     COR_A = 299,
     COR_C = 300
   };
#endif
/* Tokens.  */
#define PROGRAM 258
#define END 259
#define IF 260
#define THEN 261
#define ENDIF 262
#define ELSE 263
#define WHILE 264
#define WRITE 265
#define READ 266
#define BETWEEN 267
#define INLIST 268
#define DECVAR 269
#define ENDDEC 270
#define COMP_IGUAL 271
#define COMP_MAYOR 272
#define COMP_MENOR 273
#define COMP_MAYOR_IGUAL 274
#define COMP_MENOR_IGUAL 275
#define COMP_DISTINTO 276
#define OPAR_ASIG 277
#define TIPO_INT 278
#define TIPO_FLOAT 279
#define TIPO_STRING 280
#define CTE_ENTERA 281
#define CTE_REAL 282
#define CTE_STRING 283
#define OP_MAS 284
#define OP_MENOS 285
#define OP_MULT 286
#define OP_DIV 287
#define OP_LOG_AND 288
#define OP_LOG_OR 289
#define OP_LOG_NOT 290
#define DOS_PUNTOS 291
#define PUN_Y_COM 292
#define COMA 293
#define ID 294
#define PAR_A 295
#define PAR_C 296
#define LLAVE_A 297
#define LLAVE_C 298
#define COR_A 299
#define COR_C 300




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 77 "Sintactico.y"

char * strid;
char * num;
char * real; 
char * str;



/* Line 1676 of yacc.c  */
#line 151 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


