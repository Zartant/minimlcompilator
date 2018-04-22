#define SIZE 10
#include <stdlib.h>
#include "mltype.h"
#include "typetree.h"
#include "mloper.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "exprtree.h"

trexpr *trexpr_create_pol(int val, char *id) {
	trexpr *ret = malloc(sizeof(*ret));
	strcpy(ret -> ID, id);
	ret -> treetype = trtype_create_pol(val);
	ret -> type = POLY;
	ret -> res = NULL;
	ret -> param = NULL;
	ret -> son = NULL;
	ret -> next = NULL;
	ret -> op1 = NULL;
	ret -> op2 = NULL;
	return ret;
}

trexpr *trexpr_create(type_ml t, void *value, char *id){
	trexpr *ret = malloc(sizeof(*ret));
	switch(t) {
		case (STRING) :
			ret -> value = malloc(strlen((const char *) value));
			ret -> treetype = trtype_create_complex(strtype_create_primitiv(STRING));
			break;
		case (INTEGER) :
			ret -> value = malloc(sizeof(int));
			ret -> treetype = trtype_create_complex(strtype_create_primitiv(INTEGER));

			break;
		case (FLOAT) : 
			ret -> value = malloc(sizeof(float));
			ret -> treetype = trtype_create_complex(strtype_create_primitiv(FLOAT));
			break;
		case (BOOLEAN) :
			ret -> value = malloc(sizeof(int));
			ret -> treetype = trtype_create_complex(strtype_create_primitiv(INTEGER));
			break;
		default :
			break;
	}
	
	if (t != LIST || t != POLY) {
		ret -> value = value;
		if (t != LIST) {
			strcpy(ret -> ID, id);
		}
	}
	if (t == LIST) {
		ret -> treetype == trtype_create_complex(strtype_create_list(trtype_create_pol(404))); //TYPE POLYMORPHE OUI
	}
	ret -> res = NULL;
	ret -> param = NULL;
	ret -> type = t;
	ret -> son = NULL;
	ret -> next = NULL;
	ret -> op1 = NULL;
	ret -> op2 = NULL;
	return ret;
}

trexpr *trexpr_create_fun(trexpr *param, trexpr *res, trtype *t) {
	trexpr *ret = malloc(sizeof(*ret));
	ret -> type = FUN;
	ret -> treetype = t;
	ret -> next = NULL;
	ret -> son = NULL;
	ret -> param = param;
	ret -> res = res;
	ret -> op1 = NULL;
	ret -> op2 = NULL;
	ret -> value = NULL;
	return ret;
}

trexpr *trexpr_upletfuse(trexpr *t[], int nbofType, trtype *typ)  {
	trexpr *father = malloc(sizeof(*father));
	father -> type = TREE;
	father -> son = t[0];
	father -> treetype = typ;
	for (int i = 1; i < nbofType; ++i) {
		t[i - 1] -> next = t[i]; 
	}
	father -> next = NULL;
	father -> op1 = NULL;
	father -> op2 = NULL;
	father -> param = NULL;
	father -> res = NULL;
	father -> value = NULL;
	return father;
	
}

//Plus trop certain de l'utilité
bool trexpr_cmp(const trexpr *t1, const trexpr *t2) {
	if (t1 -> type == TREE && t2 -> type == TREE) {
		trexpr *pt1 = t1 -> son;
		trexpr *pt2 = t2 -> son;
		while (pt1 != NULL && pt2 != NULL) {
			if (!trexpr_cmp(pt1, pt2)) return false;
			pt1 = pt1 -> next;
			pt2 = pt2 -> next;
		}
	}
	if (t1 -> type == t2 -> type) {
		if (t1 -> type == FUN) {
			return trexpr_cmp(t1 -> param, t2 -> param) && trexpr_cmp(t1 -> res, t2 -> res);
		}
		if (t1 -> type == STRING) {
			return strcmp((char *) t1 -> value, (char *) t2 -> value) == 0;
		}
		return true;
		//Ici gérer la comparaison des différents types
	}
	return false;
	
}


trexpr *trexpr_create_composite(mloper oper, trexpr *a, trexpr *b, trtype *t) {
	trexpr *ret = malloc(sizeof(*ret));
	ret -> treetype = t;
	ret -> type = OPE;
	ret -> son = NULL;
	ret -> next = NULL;
	ret -> oper = oper;
	ret -> op1 = a;
	ret -> op2 = b;
	ret -> value = NULL;
	ret -> param = NULL;
	ret -> res = NULL;
	return ret;
}





