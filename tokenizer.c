#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef enum{
 true =  1,
 false = 0
} bool;

typedef enum{
INT, // 123
FLOAT, //-123.0
STR, // "123"
ID, // a
ASSIGNMENT, // =
EQ, // ==
GT, // >
LT, // <
NOT, // not
OR, // or
AND, // and
LAMBDA, // lambda
QUOTE, // "
L_PATH, // (
R_PATH, // )
SEM_COL, // ;
R_ARROW, // ->
// binary operator
POW, // **
MUL, // *
INT_DIV, // //
DIV, // /
MOD, // %
ADD, // +
SUB, // -
} Token_type;


typedef struct{
  char* id_name;
  Token_type type;
  long long int int_val;
  double float_val;
  char* string;
} Token;



int main(void){
  
}
