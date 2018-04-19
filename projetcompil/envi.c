
#include <stdlib.h>
#include "mltype.h"
#include <stdbool.h>
#include "mloper.h"
#include "exprtree.h"
#include "envi.h"
#include "typecontr.h"
#include <string.h>
#include <stdio.h>

env *new_context() {
	env *e = malloc(sizeof(*e));
	e -> liste = NULL;
	return e;
}

void add_value(env *e, char *id, trexpr *ex) {
	var *new = malloc(sizeof(*new));
	new -> next = e -> liste;
	strcpy(new -> id, id);
	new -> expr = ex;
	e -> liste = new;
}



void concat_context(env *e1, env *e2) {
	var *p = e1 -> liste;
	if (p == NULL) {
		e1 -> liste = e2 -> liste;
	} else {
		while (p != NULL) {
		    p = p -> next;
		}
		p -> next = e2 -> liste;
	}
}

trexpr *getvar(env *e, char *id) {
	var *p = e -> liste;
	while (p != NULL && strcmp(p -> id, id) != 0) {
		p = p -> next;
	}
	if (p == NULL) return NULL;
	return p -> expr;
}

void add_to_a_context (trexpr **exp, env *ctx, G *ctrset) {
	if ((*exp)  -> type == TREE) {
		trexpr **p = &((*exp) -> son);
		trtype *t[10];
		int i = 0;
		while ((*p) != NULL) {
			add_to_a_context(p, ctx, ctrset);
			t[i] = (*p) -> treetype;
			i++;
			(*p) = (*p) -> next;
		}
		add_constraint(ctrset, (*exp) -> treetype, trtype_create_complex(strtype_upletfuse(t, i)));
	} else if ((*exp) -> type == POLY) {
		printf("%s",(*exp) -> ID);
		add_value(ctx, (*exp)  -> ID, (*exp) );
	} else if ((*exp)  -> type == OPE) {
		if ((*exp)  -> oper == AFFECT) {
			add_to_a_context(&((*exp)  -> op1), ctx, ctrset);
			apply_context(&((*exp)  -> op2), ctx, ctrset);
			add_constraint(ctrset, (*exp) -> op1 -> treetype, (*exp) -> op2 -> treetype);
		} else if ((*exp)  -> oper == PUTLIST) {
			add_to_a_context(&((*exp) -> op1), ctx, ctrset);
			add_to_a_context(&((*exp) -> op2), ctx, ctrset);
			add_constraint(ctrset, (*exp) -> treetype, (*exp) -> op2 -> treetype);
			int listtype = get_listepol(ctrset);
			trtype *t = trtype_create_pol(listtype);
			incr_listepol(ctrset);
			add_constraint(ctrset, (*exp) -> op1 -> treetype, t);
			add_constraint(ctrset, (*exp) -> op2 -> treetype, trtype_create_complex(strtype_create_list(t)));
			
		} /*else if ((*exp) -> oper == CONCATLIST) {
			add_to_a_context(&((*exp) -> op1), ctx, ctrset);
			add_to_a_context(&((*exp) -> op2), ctx, ctrset);
			add_constraint(ctrset, (*exp) -> treetype, trtype_create_complex(strtype_create_list((*exp) -> treetype)));
			add_constraint(ctrset, (*exp) -> op2 -> treetype, (*exp) -> op1 -> treetype);
		}*/
	}
}

void apply_context(trexpr **exp, env *ctx, G *ctrset) {
	//trtype_print(exp -> treetype);
	env *newctx = new_context();
	concat_context(newctx, ctx);
	if ((*exp) -> type == FUN) {
	    add_to_a_context(&((*exp)  -> param), newctx, ctrset);
	    apply_context(&((*exp) -> res), newctx, ctrset);
	    add_constraint(ctrset, (*exp) -> treetype, trtype_create_complex(strtype_create_fun((*exp) -> param -> treetype, (*exp) -> res -> treetype)));


	} else if ((*exp) -> type == TREE) {
	   	trexpr **p = &((*exp) -> son);
	   	trtype *tab[20];
	   	int i = 0;
	   	if (*p == NULL) printf("ERROOR");
	   	do {
			apply_context(p, ctx, ctrset);
			tab[i] = (*p) -> treetype;
			i++;
			if ((*p) -> next == NULL) break;
			p = &((*p) -> next);
			printf("calculus n-uplet : %d\n", i);
		} while (*p != NULL);
		if (i == 0) {
			printf("Argh");
			//GESTION de l'erreur, probablement retour -1;
		} else {
			add_constraint(ctrset, (*exp) -> treetype, trtype_create_complex(strtype_upletfuse(tab, i)));
		}
		
    } else if ((*exp) -> type == OPE) {
		if ((*exp) -> oper == LETIN) {
			add_to_a_context(&((*exp)  -> op1), ctx, ctrset);
			apply_context(&((*exp)-> op2), ctx, ctrset);
			add_constraint(ctrset, (*exp) -> treetype, (*exp) -> op2 -> treetype);
		}
		if ((*exp) -> oper == INTPLUS) {
			(*exp) -> treetype = trtype_create_complex(strtype_create_primitiv(INTEGER));
			apply_context(&((*exp) -> op1), ctx, ctrset);
			apply_context(&((*exp) -> op2), ctx, ctrset);
			add_constraint(ctrset, (*exp) -> op1 -> treetype, trtype_create_complex(strtype_create_primitiv(INTEGER)));
			add_constraint(ctrset, (*exp) -> op2 -> treetype, trtype_create_complex(strtype_create_primitiv(INTEGER)));
		}
		if ((*exp) -> oper == APPLY) {
			apply_context(&((*exp) -> op1), ctx, ctrset);
			apply_context(&((*exp) -> op2), ctx, ctrset);
			add_constraint(ctrset, (*exp) -> op1 -> treetype, trtype_create_complex(strtype_create_fun((*exp) -> op2 -> treetype , (*exp) -> treetype)));
		}
		if ((*exp) -> oper == PUTLIST) {
			apply_context(&((*exp) -> op1), ctx, ctrset);
			apply_context(&((*exp) -> op2), ctx, ctrset);
			add_constraint(ctrset, (*exp) -> treetype, (*exp) -> op2 -> treetype);
			int listtype = get_listepol(ctrset);
			trtype *t = trtype_create_pol(listtype);
			incr_listepol(ctrset);
			add_constraint(ctrset,t,  (*exp) -> op1 -> treetype);
			add_constraint(ctrset, (*exp) -> op2 -> treetype, trtype_create_complex(strtype_create_list(t)));
		}
		if ((*exp) -> oper == CONCATLIST) {
			apply_context(&((*exp) -> op1), ctx, ctrset);
			apply_context(&((*exp) -> op2), ctx, ctrset);
			int listtype = get_listepol(ctrset);
			trtype *t = trtype_create_pol(listtype);
			incr_listepol(ctrset);
			add_constraint(ctrset, (*exp) -> op1 -> treetype, trtype_create_complex(strtype_create_list(t)));
			add_constraint(ctrset, (*exp) -> op2 -> treetype, trtype_create_complex(strtype_create_list(t)));
			add_constraint(ctrset, (*exp) -> treetype, (*exp) -> op1 -> treetype);
			
			
		}
	} else if ((*exp) -> type == POLY) {
		printf("%s",(*exp) -> ID);
		*exp = getvar(ctx, (*exp) -> ID);
		if (*exp == NULL) {
			printf("ARGH");
			//Traitement de l'erreur (c'est que le mec a rentrer nimp
			
		}
	} 

}


void *cpyvaluexpr(trexpr *expr) {
	if (expr -> value == NULL) {
		return NULL;
	}
	
	if (expr -> type == INTEGER) {
		int *v = malloc(sizeof(*v));
		*v = *((int *)(expr -> value));
		return v;
	}
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
	ret -> value = cpyvaluexpr(expr);
	return ret;
}



int eval_operation(trexpr **expr, env *ctx) {
	int r1 = evaluate_expr(&((*expr) -> op1), ctx);
	int r2 = evaluate_expr(&((*expr) -> op2), ctx);
	//Test retour r1
	
	if ((*expr)-> type == OPE) {
		if ((*expr) -> oper == INTPLUS) {
			int *v1 = malloc(sizeof(int));
			v1 = (*expr) -> op1 -> value;
		    int *v2 = malloc(sizeof(int));
		    v2 = (*expr) -> op2 -> value;
		    int *ret = malloc(sizeof(int));
		    *ret = *v1 + *v2;
		    *expr = trexpr_create(INTEGER, ret, ""); 
		} else if ((*expr) -> oper == APPLY) {
			env *nctx = new_context();
			concat_context(nctx, ctx);
			add_value(nctx, (*expr) -> op1 -> param -> ID , (*expr) -> op2);
			*expr = (*expr) -> op1 -> res;
			evaluate_expr(expr, nctx); //Test retour
		} else if ((*expr) -> oper == CONCATLIST) {
			if ((*expr) -> op1 -> type = LIST) {
				*expr = (*expr) -> op2;
			} else if ((*expr)-> op1 -> type = PUTLIST){
				trexpr *p = (*expr) -> op1 -> op2;
				(*expr) -> op1 -> op2 = (*expr) -> op2;
			    (*expr) -> op2 = (*expr) -> op1;
			    (*expr) -> op1 = p;
			}
		}
	}
}

int evaluate_expr(trexpr **expr, env *ctx) {
	if ((*expr) == NULL) {
		return -1;
	}
	if ((*expr) -> type == POLY) {
		*expr = copy_expr(getvar(ctx, (*expr) -> ID));
	} else if ((*expr) -> type == OPE) {
		return eval_operation(expr, ctx);
	} else if ((*expr) -> type == FUN) {
		return evaluate_expr(&((*expr) -> param), ctx);
		return evaluate_expr(&((*expr) -> res), ctx);
	}else if ((*expr) -> type == TREE) {
		trexpr **p = &((*expr) -> son);
		while (*p != NULL) {
			evaluate_expr(p, ctx);
			p = &((*p) -> next);
		}
	}
		
}

void print_list(trexpr *expr) {
	print_expr(expr -> op1);
	if (expr -> op2 -> type == OPE && expr -> op2 -> oper == PUTLIST) {
		printf(";");
		print_list(expr -> op2);
	} else if (expr -> op2 -> type == LIST) {
		printf("]");
	}
}

void print_expr(trexpr *expr) {
	if (expr == NULL) printf("ah");
	if (expr -> type == FUN) {
		printf("< fun >");
	} else if (expr -> type == TREE) {
	   printf("(");
	   trexpr *p = expr -> son;
	   while (p != NULL) {
		  print_expr(p);
		  p = p -> next;
		  if (p != NULL) printf(",");
	   }
	   printf(")");
	} else if (expr -> type == OPE) { // c'est forcément un INPUTLIST
		printf("[");
		print_list(expr);
		
	} else if (expr -> type == LIST) { //liste vide
		printf("[]");
	} else if (expr -> type == INTEGER) {
		printf("%d", *((int *)(expr -> value)));
	}
	
}
