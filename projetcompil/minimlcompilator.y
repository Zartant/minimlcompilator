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

static int pile_poly[2048];
static int pilepolysize = 0;

static char* pile_expr[2048];
static int pilrexprsize = 0;

int depile_poly(){
	return pile_poly[pilepolysize--];
}

char* depile_expr(){
	return pile_expr[pilexprsize--];
}

void empile_poly(int n){
	pile_poly[++pilepolysize] = n;
}

void empile_expr(char* n){
	strcpy(pile_expr[++pileexprsize],n);
}

void prelude();
void epilogue();
static int polnb = 0
int get_next_pol_type(){
	return ++polnb;
}

void rand_str(char *dest, size_t length) {
    srand((unsigned int)time(NULL));
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    while (length-- > 0) {
        size_t index = (size_t) rand() / RAND_MAX
          * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}

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

expr : expr PLUS expr {
	//Quand on va devoir désallouer ces etiquettes il faudra s'en souvenir...
	char* e = rand_str(e,10);
	char* f = rand_str(f,10);
	empile_expr(e);
	empile_expr(f);
	dprintf("trexpr* %s trexpr_create(INTEGER, %d);\n",$1,rand_str(e,10));
    dprintf("trexpr* %s = trexpr_create_composite(INTPLUS, %s, %s, trtype_create_pol(%d) );\n",e,f, get_next_pol_type()); 
	dprintf("trexpr* %s trexpr_create(INTEGER, %d);\n",$3,rand_str(e,10));
}
|expr : expr MINUS expr {
	char* e = rand_str(e,10);
	char* f = rand_str(f,10);
	empile_expr(e);
	empile_expr(f);
	dprintf("trexpr* %s trexpr_create(INTEGER, %d);\n",$1,rand_str(e,10));
    dprintf("trexpr* %s = trexpr_create_composite(INTMINUS, %s, %s, trtype_create_pol(%d) );\n",e,f, get_next_pol_type()); 
	dprintf("trexpr* %s trexpr_create(INTEGER, %d);\n",$3,rand_str(e,10));
}
|expr : expr TIMES expr {
	char* e = rand_str(e,10);
	char* f = rand_str(f,10);
	empile_expr(e);
	empile_expr(f);
	dprintf("trexpr* %s trexpr_create(INTEGER, %d);\n",$1,rand_str(e,10));
    dprintf("trexpr* %s = trexpr_create_composite(INTTIMES, %s, %s, trtype_create_pol(%d) );\n",e,f, get_next_pol_type()); 
	dprintf("trexpr* %s trexpr_create(INTEGER, %d);\n",$3,rand_str(e,10));
}
|expr : expr DIVIDE {
	char* e = rand_str(e,10);
	char* f = rand_str(f,10);
	empile_expr(e);
	empile_expr(f);
	dprintf("trexpr* %s trexpr_create(INTEGER, %d);\n",$1,rand_str(e,10));
    dprintf("trexpr* %s = trexpr_create_composite(INTDIVIDE, %s, %s, trtype_create_pol(%d) );\n",e,f, get_next_pol_type()); 
	dprintf("trexpr* %s trexpr_create(INTEGER, %d);\n",$3,rand_str(e,10));
}
%%

int main(int argc, char** argv){
	//on déclare l'environnement
	prelude();
	yyparse();
	epilogue();
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
	close(fd);
}
