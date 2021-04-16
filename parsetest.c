#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "util.h"
#include "errormsg.h"
#include "parse.h"
#include "prabsyn.h"

int main(int argc, char **argv)
{
  extern int yydebug;
  yydebug = 1;

  if (argc < 2) {
    fprintf(stderr,"Utilize o comando da seguinte forma: compilador.tge filename -options\n");
    exit(1);
  }

  char *filename = argv[1];
  printf("\nIniciando a execução do compilador\n");
  printf("\nExecutando 'parser'\n");
  A_exp prg = parse(filename);
  int IS_ERRORS;
  if (prg != NULL)
  {
    printf("Arvore construída");
  }else{
    printf("Erro! Parser não retornou uma estrutura válida!\n");
  }

  if(!IS_ERRORS){
    char *treeFileName = malloc(500);
    sprintf(treeFileName, "%s",filename);
    treeFileName = remove_ext(treeFileName, '.', '/');
    strcat(treeFileName, ".arv");
    FILE* treefile = fopen(treeFileName, "w+");
    printf("\nImprimindo a Arvore\n");
    pr_exp(treefile, prg, 0);
    printf("Fim!\n");

  }

  return 0;
}