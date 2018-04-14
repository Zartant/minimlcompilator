#define SIZE 10
#include <stdlib.h>
#include "mltype.h"
#include "mloper.h"
#include "typetree.h"
#include <stdbool.h>
#include <stdio.h>
typedef struct strtype strtype;
typedef struct trtype trtype;


struct strtype {
	 // Les autres membres du n-uplet, le suivant ici
	
	//cas où l'on a une fonction 
	trtype *param;
	trtype *res; 
	
	//cas où on a une opération
	mloper oper;
	trtype *op1;
	trtype *op2;
	
	type_ml type; //VOIR mltype.h
	trtype *son;//n'existe qu'en cas de type TREE, pointe le premier paramètre du n-uplet
};


//Un trtype est complexe ou polymorphe
struct trtype {
	bool iscomplex; 
	union {
		strtype *type;
		int poltype;
	};
	trtype *next;	
};



// Ici tout ce qui gère les types complexes
strtype *strtype_create_primitiv(type_ml t) {
	strtype *ret = malloc(sizeof(*ret));
	ret -> type = t;
	ret -> son = NULL;
	ret -> op1 = NULL;
	ret -> op2 = NULL;
	ret -> param = NULL;
	ret -> res = NULL;
	return ret;
}

strtype *strtype_create_fun(trtype *param, trtype *res) {
	strtype *ret = malloc(sizeof(*ret));
	ret -> type = FUN;
	ret -> son = NULL;
	ret -> param = param;
	ret -> res = res;
	ret -> op1 = NULL;
	ret -> op2 = NULL;
	return ret;
}

strtype *strtype_upletfuse(trtype *t[], int nbofType)  {
	strtype *father = malloc(sizeof(*father));
	father -> type = TREE;
	father -> son = t[0];
	for (int i = 1; i < nbofType; ++i) {
		t[i - 1] -> next = t[i]; 
	}
	father -> op1 = NULL;
	father -> op2 = NULL;
	father -> param = NULL;
	father -> res = NULL;
	return father;
}

bool strtype_cmp(const strtype *t1, const strtype *t2) {
	if (t1 -> type == TREE && t2 -> type == TREE) {
		trtype *pt1 = t1 -> son;
		trtype *pt2 = t2 -> son;
		while (pt1 != NULL && pt2 != NULL) {
			if (!trtype_cmp(pt1, pt2)) return false;
			pt1 = pt1 -> next;
			pt2 = pt2 -> next;
		}
	}
	if (t1 -> type == t2 -> type) {
		if (t1 -> type == FUN) {
			return trtype_cmp(t1 -> param, t2 -> param) && trtype_cmp(t1 -> res, t2 -> res);
		}
		if (t1 -> type == LIST) {
			return trtype_cmp(t1 -> son, t2 -> son);
		}
		return true;
	}
	return false;
	
}

strtype *strtype_create_list(trtype *a) {
	strtype *ret = malloc(sizeof(*ret));
	ret -> type = LIST;
	ret -> son = a;
	ret -> op1 = NULL;
	ret -> op2 = NULL;
	ret -> param = NULL;
	ret -> res = NULL;
	
}

strtype *strtype_create_composite(mloper oper, trtype *a, trtype *b) {
	strtype *ret = malloc(sizeof(*ret));
	ret -> type = OPE;
	ret -> son = NULL;
	ret -> oper = oper;
	ret -> op1 = a;
	ret -> op2 = b;
	ret -> param = NULL;
	ret -> res = NULL;
	return ret;
}

//Des fonctions généralistes
void trtype_print(const trtype *ty) {
	if (!(ty -> iscomplex)) {
		//char c = (char) (ty -> poltype + (int) 'a');
		//printf("%c", c);
		printf("%d", ty-> poltype);
		return;
	}
	strtype *t = ty -> type;
	switch(t -> type) {
		case INTEGER :
			printf("int");
			break;
		case STRING :
			printf("string");
			break;
		case FLOAT :
			printf("float");
			break;
		case BOOLEAN :
			printf("bool");
			break;
		//TODO : MACRO POLYMORPHES
		case FUN :
			printf("(");
			trtype_print(t -> param);
			printf(" -> ");
			trtype_print(t -> res);
			printf(")");
			break;
		case TREE :
			printf("(");
			trtype *p = t -> son;
			while (p != NULL) {
				trtype_print(p);
				p = p -> next;
				if (p != NULL) printf(" * ");
			}
			printf(")");
			break;
		case LIST :
			printf("[");
			trtype_print(t -> son);
			printf("]");
		default:
		
			break;
	}
}

trtype *trtype_create_pol(int i) {
	trtype *ret = malloc(sizeof(*ret));
	ret -> iscomplex = false;
	ret -> poltype = i;
	ret -> next = NULL;
	return ret;
}

trtype *trtype_create_complex(strtype *t) {
	trtype *ret = malloc (sizeof(*t));
	ret -> iscomplex = true;
	ret -> type = t;
	ret -> next = NULL;
	return ret;
}

//Inférence de type need
bool trtype_cmp (trtype *t1, trtype *t2) {
	
	if(t1 == NULL 
	|| t2 == NULL 
	|| t1 -> iscomplex 
	!= t2 -> iscomplex) return false;
	if(t1 -> iscomplex) {
		return strtype_cmp(t1 -> type, t2 -> type);
	}
	return t1 -> poltype == t2 -> poltype;
}

bool trtype_isIn(trtype *t1, trtype *t2) {
	if (trtype_cmp(t1, t2)) return true;
	if (t2 == NULL) return false;
	if (trtype_isIn(t1, t2 -> next)) return true;
	if (t2 -> iscomplex) {
		return (trtype_isIn(t1, t2 -> type -> op1)) 
		|| (trtype_isIn(t1, t2 -> type -> op2))
		|| (trtype_isIn(t1,t2 -> type -> param)) 
		|| (trtype_isIn(t1,t2 -> type -> res))
		|| (trtype_isIn(t1,t2 -> type -> son));
	}
	
}


void trtype_replace(trtype *t, trtype *x, trtype **tr) {
	if (trtype_cmp(t, (*tr))) {
		*tr = x;
		return;
	}
	if (*tr == NULL) return;
	if ((*tr) -> iscomplex) {
		trtype_replace(t, x, &((*tr) -> type -> op1)); 
		trtype_replace(t, x, &((*tr) -> type -> op2));
		trtype_replace(t, x, &((*tr) -> type -> son));
		trtype_replace(t, x, &((*tr) -> next));
		trtype_replace(t, x, &((*tr) -> type -> param));
		trtype_replace(t, x, &((*tr) -> type -> res));
	}
	
}

bool trtype_variable(trtype *tr) {
	if (tr == NULL) return false;
	if (tr -> iscomplex) {
		strtype *t = tr -> type; 
		return trtype_variable(t -> op1) || trtype_variable(t -> op2)
		|| trtype_variable(t -> son) || trtype_variable(tr -> next) 
		|| trtype_variable(t -> param) || trtype_variable(t -> res);
	} else {
		return true;
	}
}

bool trtype_is_complex(trtype *t) {
	return (t -> iscomplex);
}

bool trtype_is_fun(trtype *t) {
	if (t -> iscomplex) {
		return t -> type -> type == FUN;
	}
	return false;
}

trtype *trtype_get_param (trtype *t) {
	if (t -> iscomplex) {
		return t -> type -> param;
	} else {
		return NULL;
	}
}


trtype *trtype_get_res (trtype *t) {
	if (t -> iscomplex) {
		return t -> type -> res;
	} else {
		return NULL;
	}
}

trtype *trtype_get_son (trtype *t) {
	if (t -> iscomplex) {
		return t -> type -> son;
	} else {
		return NULL;
	}
}
trtype *trtype_get_next (trtype *t) {
	if (t -> iscomplex) {
		return t -> next;
	} else {
		return NULL;
	}
}
