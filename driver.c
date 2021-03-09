/*
 * A test scaffold to run your lexer on an input file.
 */

#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "errormsg.h"

int yyparse(void); /* prototype for the lexing function */

extern FILE * yyin;  

/*
 * Variável do flex.
 */
extern int yydebug; 

int main(int argc, char **argv) {

    // abre arquivo com o código fonte
    yyin = fopen(argv[1],"r");
    if (!yyin) {
      printf(" Cannot open this file."); exit(1);
    }
    yydebug = 0;
    // faz o parser do código fonte
    yyparse(); 
    

}
