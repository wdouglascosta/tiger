%{
#include <string.h>
#include <util.h>
#include <tokens.h>
#include <errormsg.h>

int charPos=1;

int yywrap(void)
{
 charPos=1;
 return 1;
}


void adjust(void)
{
 EM_tokPos=charPos;
 charPos+=yyleng;
}

%}

/* Lex Definitions: */
digits [0-9]+

/* palavras reservadas */
while {adjust(); return WHILE;}
for  {adjust(); return FOR;}
to  {adjust(); return TO;}
break   {adjust(); return BREAK;}
let  {adjust(); return LET;}
in   {adjust(); return IN;}
end   {adjust(); return END;}
function   {adjust(); return FUNCTION;}
var   {adjust(); return VAR;}
type   {adjust(); return TYPE;}
array {adjust(); return ARRAY;}
if   {adjust(); return IF;}
then {adjust(); return THEN;}
else {adjust(); return ELSE;}
do  {adjust(); return DO;}
of   {adjust(); return OF;}
nil   {adjust(); return NIL;}


%%
" "	 {adjust(); continue;}
\n	 {adjust(); EM_newline(); continue;}
","	 {adjust(); return COMMA;}
for  	 {adjust(); return FOR;}
[0-9]+	 {adjust(); yylval.ival=atoi(yytext); return INT;}
.	 {adjust(); EM_error(EM_tokPos,"illegal token");}
%%


void adjust(void){
	int i;
	yylloc.first_line = lexline;
	yylloc.first_collum = lexcol + 1;

	for (i = 0, yytext[i] != '\0'; i++) {
		if (yytext[i] == '\n){
			lexline++;
			lexcol = 0;
		} else if (yytext[i] == '\t'){
			lexcol += 8 - (lexcol % 8);
		} else {
			lexcol++;
		}
	}
}

