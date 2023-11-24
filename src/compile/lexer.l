%{
	#include "./parser.tab.h"
%}

%%

%%

int yywrap()
{
	return 1;
}
