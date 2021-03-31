/*
 * Error message module, useful for producing error messages with file names
 * and line numbers.
 */
#include <string.h>
#include <stdbool.h>
extern bool EM_anyErrors;
void EM_newline(void);

extern int EM_tokPos;
void yyerror();
void EM_error(int pos, char *message, ...);
void EM_impossible();
void EM_reset();
