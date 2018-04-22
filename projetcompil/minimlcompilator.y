%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "envi.h"
#include "exprtree.h"
#include "mloper.h"
#include "typecontr.h"
#include "typetree.h"

#define OUTPUTFILE "output.c"
static int fd;

void prelude();
void epilogue();

%}
%union {
	int ival;
	float fval;
	char sval[64];
}

%token <ival> INT
%token <fval> BOOLEAN
%token <sval> EXPR
%token LPAREN RPAREN
%token PLUS MINUS TIMES DIVIDE FLPLUS FLMINUS FLTIMES FLDIVIDE
%token EOSTATEMENT

%left LET
%left PLUS MINUS
%left TIMES DIVIDE
%type<ival> iexpr
%type<fval> fexpr
%type<sval> strexpr

%%



%%

int main(int argc, char** argv){
	//on déclare l'environnement
	prelude();
	
}

void prelude(){
	fd = open(OUTPUTFILE, O_CREAT | O_RDWR);
	if(fd<0){
		exit(EXIT_FAILURE);
	}
	dprintf(fd,"#include \"envi.h\"\n");
	dprintf(fd,"#include \"exprtree.h\"\n");
	dprintf(fd,"#include \"mloper.h\"\n");
	dprintf(fd,"#include \"typecontr.h\"\n");
	dprintf(fd,"#include \"typetree.h\"\n\n");
	dprintf(fd,"int main(void) {\n");
}

void epilogue() {
	dprintf(fd,"\t}\n}\n");

}
