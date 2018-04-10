
#ifndef _EXPR_TREE
#define _EXPR_TREE

#include <stdbool.h>
#include <stdlib.h>
#include "mltype.h"
#include "mloper.h"
#include "typetree.h"

typedef struct trexpr trexpr;
struct trexpr {
	trexpr *next; // Les autres membres du n-uplet, le suivant ici
	
	//cas où l'on a une fonction 
	trexpr *param;
	trexpr *res; 
	
	//cas où on a une opération
	mloper oper;
	trexpr *op1;
	trexpr *op2;
	trexpr *op3;

	
	//ID de l'expression
	char ID[50];
	
	//Vrai type construit de l'expression
	trtype *treetype;

	type_ml type; //VOIR mltype.h
	void *value; //si type prim
	trexpr *son;//n'existe qu'en cas de type TREE, pointe le premier paramètre du n-uplet
};

trexpr *trexpr_create_pol(int val, char *id);

trexpr *trexpr_create(type_ml t, void *value, char *id);

trexpr *trexpr_create_fun(trexpr *param, trexpr *res, trtype *t);

trexpr *trexpr_upletfuse(trexpr *t[], int nbofType, trtype *typ);

bool trexpr_cmp(const trexpr *t1, const trexpr *t2);


trexpr *trexpr_create_composite(mloper oper, trexpr *a, trexpr *b, trtype *t);




#endif //_EXPR_TREE

