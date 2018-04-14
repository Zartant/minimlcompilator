#include <stdbool.h>
#include <stdlib.h>
#include "mltype.h"
#include "mloper.h"
#ifndef _TYPE_TREE
#define _TYPE_TREE


typedef struct strtype strtype;

typedef struct trtype trtype;



// Ici tout ce qui gère les types complexes
strtype *strtype_create_primitiv(type_ml t);

strtype *strtype_create_fun(trtype *param, trtype *res);

strtype *strtype_upletfuse(trtype *t[], int nbofType);

bool strtype_cmp(const strtype *t1, const strtype *t2);

strtype *strtype_create_composite(mloper oper, trtype *a, trtype *b);
strtype *strtype_create_list(trtype *a);
//Des fonctions généralistes
void trtype_print(const trtype *ty);

trtype *trtype_create_pol(int i);

trtype *trtype_create_complex(strtype *t);

bool trtype_cmp (trtype *t1, trtype *t2);
bool trtype_isIn(trtype *t1, trtype *t2);
void trtype_replace(trtype *t, trtype *x, trtype **tr);
bool trtype_variable(trtype *t);
bool trtype_is_complex(trtype *t);
bool trtype_is_fun(trtype *t);
trtype *trtype_get_res (trtype *t);
trtype *trtype_get_param (trtype *t);
trtype *trtype_get_son (trtype *t);
trtype *trtype_get_next (trtype *t);

#endif //_TYPE_TREE
