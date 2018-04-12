%{
	#include <stdio.h>
%}
%option noyywrap
%%
#*\n 	{} // On vire d'abord les commentaires
["]*["]			{return STRING;}
let				{return LET;}
[0-9]* 			{return INT;} 
([0-9]*[.][0-9]*){return FLOAT;}
[a-z][a-z0-9]*	{return ID;}
=				{return EQUALS}
in				{return IN;}
if				{return IF;}
then			{return THEN;}
true			{return TRUE;}
false			{return FALSE;}
[				{return LBRACK;}
]				{return RBRACK;}
+.				{return FLPLUS;}
-.				{return FLMINUS;}
*.				{return FLTIMES;}
/.				{return FLDIVIDE;}
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