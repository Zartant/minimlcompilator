%{
	#include <stdio.h>
%}
%option noyywrap
%%
#*\n 	{} // On vire d'abord les commentaires
let				{return LET;}
([0-9]*[.][0-9]*)|[0-9]* {return NUMBER;} //un nombre peut être flottant ou entier
[a-z][a-z0-9]*	{return ID;}
=				{return EQUALS}
in				{return IN;}
->				{return ARROW:}
(				{return LPAREN;}
)				{return RPAREN;}
@				{return CONCATLIST;}
+				{return PLUS;}
::				{return ADDTOLIST;}
-				{return MINUS;}
*				{return TIMES;}
/				{return DIVIDE;}
;;				{return EOSTATEMENT;}

[ \t\n]+		{}
%%