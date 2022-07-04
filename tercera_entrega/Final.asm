include macros2.asm
include number.asm
.MODEL LARGE	; Modelo de Memoria
.386	        ; Tipo de Procesador
.STACK 200h		; Bytes en el Stack

.DATA 

int1                          dd                            ?                             ;Variable int
int2                          dd                            ?                             ;Variable int
int3                          dd                            ?                             ;Variable int
float1                        dd                            ?                             ;Variable float
float2                        dd                            ?                             ;Variable float
cadena                        dd                            ?                             ;Variable string
valorTeclado                  dd                            ?                             ;Variable string
_1                            dd                            1.00                          ;Cte en formato  int
_4                            dd                            4.00                          ;Cte en formato  int
_5                            dd                            5.00                          ;Cte en formato  int
_2_1                          dd                            2.1                           ;Cte en formato  float
_Inicio_programa               db                             "Inicio programa"             ,'$', 15 dup (?) ;Cte en formato 
_Condicion_verdadera           db                             "Condicion verdadera"         ,'$', 19 dup (?) ;Cte en formato 
_Condicion_verdadera_not       db                             "Condicion verdadera not"     ,'$', 23 dup (?) ;Cte en formato 
_Condicion_falsa_not           db                             "Condicion falsa not"         ,'$', 19 dup (?) ;Cte en formato 
_Condicion_falsa               db                             "Condicion falsa"             ,'$', 15 dup (?) ;Cte en formato 
_3                            dd                            3.00                          ;Cte en formato  int
_Funciona_perfectamente        db                             "Funciona perfectamente"      ,'$', 22 dup (?) ;Cte en formato 
@Aux1                         dd                            ?                             ;Variable float
@Aux2                         dd                            ?                             ;Variable float
@Aux3                         dd                            ?                             ;Variable float


.CODE

mov AX,@DATA    ; Inicializa el segmento de datos
mov DS,AX
mov es,ax ;

FLD _1
FSTP int1
FFREE
FLD _4
FLD int1
FADD 
FSTP @Aux1
FFREE
FLD @Aux1
FSTP int2
FFREE
FLD int2
FLD _5
FDIV 
FSTP @Aux2
FFREE
FLD @Aux2
FSTP int3
FFREE
FLD _2_1
FSTP float1
FFREE
displayString _Inicio_programa
displayString int1
FLD int1
FCOMP int2
FSTSW ax
SAHF
JNB saltoelse1
displayString _Condicion_verdadera
saltoelse1:
FFREE
FLD int1
FCOMP int2
FSTSW ax
SAHF
JNAE saltoelse2
displayString _Condicion_verdadera_not
JMP fin_if1
saltoelse2:
displayString _Condicion_falsa_not
fin_if1:
saltoelse3:
FFREE
FLD int2
FCOMP int1
FSTSW ax
SAHF
JNA saltoelse4
FLD int1
FCOMP int2
FSTSW ax
SAHF
JNB saltoelse4
displayString _Condicion_verdadera
saltoelse4:
FFREE
saltoelse5:
FFREE
FLD int1
FCOMP int3
FSTSW ax
SAHF
JNB saltoelse6
displayString _Condicion_verdadera
JMP fin_if2
saltoelse6:
displayString _Condicion_falsa
fin_if2:
saltoelse7:
FFREE
principiowhile:
FLD int1
FCOMP _3
FSTSW ax
SAHF
JE saltoelse8
displayString _Funciona_perfectamente
FLD int1
FLD _1
FADD 
FSTP @Aux3
FFREE
FLD @Aux3
FSTP int1
FFREE
JMP principiowhile
saltoelse8:



mov ax,4c00h	; Indica que debe finalizar la ejecución
int 21h

End
