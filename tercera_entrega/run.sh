#!/bin/bash

flex Lexico.l 

bison -dyv Sintactico.y

gcc lex.yy.c  y.tab.c -o segunda 

./segunda prueba.txt

dot -Tpng arbol.dot -o arbol.png

rm lex.yy.c
rm y.tab.c
rm y.tab.h
rm segunda