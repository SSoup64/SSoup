%{
	#include <string.h>
	#include "./parser.tab.h"
%}

%%
"let"								{ return TOK_LET; }

[A-Za-z_][A-Za-z0-9_]				{ yylval.val = strdup(yytext); return TOK_IDENT; }
\"([^"\\]|\\.)*\"					{ yylval.val = strdup(yytext); return TOK_STR; }
[0-9]+(\.([0-9])+)?					{ yylval.val = strdup(yytext); return TOK_NUMBER; }

";"									{ return TOK_SEMICOLONS; }
%%

int yywrap()
{
	return 1;
}
