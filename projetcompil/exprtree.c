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
	ret -> next = NULL;
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
	
	ret -> type = t;
	ret -> son = NULL;
	ret -> next = NULL;
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
	return ret;
}

trexpr *trexpr_upletfuse(trexpr *t[], int nbofType, trtype *typ)  {
	trexpr *father = malloc(sizeof(*father));
	father -> type = TREE;
	father -> son = t[0];
	father -> treetype = typ;
	for (int i = 1; i <= nbofType; ++i) {
		t[i - 1] -> next = t[i]; 
	}
	return father;
}

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
	return ret;
}

trexpr *copy_expr (trexpr *expr) {
	if (expr == NULL) {
		return NULL;
	}
	trexpr *ret = malloc(sizeof(*ret));
	ret -> treetype = expr -> treetype;
	ret -> type = expr -> type;
	ret -> son = copy_expr(expr -> son);
	ret -> next = copy_expr(expr -> next);
	ret -> oper = expr -> oper;
	ret -> op1 = copy_expr(expr -> op1);
	ret -> op2 = copy_expr(expr -> op2);
	return ret;
}

int eval_operation(trexpr **expr, env *ctx) {
	int r1 = evaluate_expr(&(*expr -> op1), ctx);
	int r2 = evaluate_expr(&(*expr -> op2), ctx);
	//Test retour r1
	
	if (*expr -> type == OPE) {
		if (*expr -> oper == INTPLUS) {
			int *v1 = malloc(sizeof(int));
			v1 = *expr -> op1 -> value;
		    int *v2 = malloc(sizeof(int));
		    v2 = *expr -> op2 -> value;
		    int *ret = malloc(sizeof(int));
		    *ret = *v1 + *v2;
		    *expr = trexpr_create(INTEGER, ret, ""); 
		} else if (*expr -> oper == APPLY) {
			env *nctx = new_context();
			concat_context(nctx, ctx);
			add_value(nctx, *expr -> op1 -> param -> ID , *expr -> op2);
			*expr = *expr -> op1 -> res;
			evaluate_expr(expr, nctx); //Test retour
		}
	}
}

int evaluate_expr(trexpr **expr, env *ctx) {
	if (*expr == NULL) {
		return -1;
	}
	if (*expr -> type == POLY) {
		*expr = copy_expr(getvar(ctx, (*expr) -> ID));
	} else if (*expr -> type == OPE || *expr -> type == FUN) {
		return eval_operation(expr, ctx);
	} else if (*expr -> type == TREE) {
		trexpr **p = &(*expr -> son);
		while (*p != NULL) {
			evaluate_expr(p, ctx);
			p = &(*p -> next);
		}
	}
		
}




