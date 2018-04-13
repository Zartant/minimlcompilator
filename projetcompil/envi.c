
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
	while (p != NULL) {
		p = p -> next;
	}
	if (p == NULL) {
		e1 -> liste = e2 -> liste;
	} else {
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
	   	while (*p != NULL) {
			apply_context(p, ctx, ctrset);
			tab[i] = (*p) -> treetype;
			i++;
			p = (*p) -> next;
		}
		if (i == 0) {
			printf("Argh");
			//GESTION de l'erreur, probablement retour -1;
		} else {
			add_constraint(ctrset, (*exp) -> treetype, trtype_create_complex(strtype_upletfuse(tab, i + 1)));
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
