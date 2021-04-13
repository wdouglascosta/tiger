#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "util.h"
#include "errormsg.h"
#include "parse.h"
#include "prabsyn.h"
#include "codigo_intermediario.h"

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
    printf("Iniciando analise semantica\n");
    SEM_transProg(prg, &IS_ERRORS);
    printf("Finalizada analise semantica\n");
  }else{
    printf("Erro! Parser não retornou uma estrutura válida!\n");
  }

  if(!IS_ERRORS){
    char *treeFileName = malloc(500);
    sprintf(treeFileName, "%s",filename);
    treeFileName = remove_ext(treeFileName, '.', '/');
    strcat(treeFileName, ".arv");
    FILE* treefile = fopen(treeFileName, "w+");
    printf("Iniciando geração da Arvore\n");
    pr_exp(treefile, prg, 0);
    printf("Fim!\n");

    char *emit = "-llvm-emit";
    if(argc == 3 && (strcmp(emit, argv[2]) == 0)){
      printf("Iniciando geração de codigo!\n");
      geraCodigoIntermediario(filename, prg);
      printf("Fim da geração de codigo\n");
    }
  }

  return 0;
}