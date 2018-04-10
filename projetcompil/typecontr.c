#define IDSIZEMAX
#include <stdlib.h>
#include "mltype.h"
#include <stdbool.h>
#include <stdio.h>
#include "mloper.h"
#include "typetree.h"

typedef struct G G;
typedef struct constrain constrain;
struct G{
	constrain *liste;
	int listepol;
};

struct constrain {
	constrain *next;

	trtype *t1;
	trtype *t2;
};

typedef struct G G;
typedef struct constrain constrain;

G *new_constraints_set() {
	G *g = malloc(sizeof(*g));
	g -> liste = NULL;
	g -> listepol = 50;
	return g;
}

void add_constraint(G *g, trtype *t1, trtype *t2) {
	constrain *new = malloc(sizeof(*new));
	new -> next = g -> liste;
	new -> t1 = t1;
	new -> t2 = t2;
	g -> liste = new;
}

int get_listepol(G *g){
	return g -> listepol;
}

void incr_listepol(G *g) {
	g -> listepol++;
}

void print_constraint(constrain *c) {
	trtype_print(c -> t1);
	printf(" = ");
	trtype_print(c -> t2);
}
void print_constraint_set(G *g) {
	constrain *c = g -> liste;
	while (c != NULL) {
		print_constraint(c);
		printf("\n");
		c = c -> next;
	}
}


//GESTION DE L INFERENCE DE TYPE A PARTIR DUN ENSEMBLE DE CONTRAINTE

bool type_in_constraint(G *g, trtype *t, int except) {
	constrain *c = g -> liste;
	int i = 0;
	while (c != NULL) {
		if (i == except) {
			i++;
			c = c -> next;
		}
		if ((trtype_isIn(t, c -> t1)) || (trtype_isIn(t, c -> t2))) return true; 
		i++;
		c = c -> next;
		
	}
}

void replace_in_constraint(G *g, trtype *t, trtype *x, int except) {
	constrain *c = g -> liste;
	int i = 0;
	while (c != NULL) {
		if (i == except) {
			i++;
			c = c -> next;
		}
		trtype_replace(t, x, &(c -> t1));
		trtype_replace(t, x, &(c -> t2));
		i++;
		c = c -> next;
		
	}
}

int infer_type(G *g) {
	constrain **c = &(g -> liste);
	int i = 0;
	while (*c != NULL) {
		if (trtype_cmp((*c) -> t1, (*c) -> t2)) {
			*c = (*c) -> next;
			c = &(g -> liste);
			i = 0;
		} else if (trtype_is_complex((*c) -> t1) && (trtype_is_complex((*c) -> t1)) && (trtype_is_fun((*c) -> t1))
					&& (trtype_is_fun((*c) -> t1))) {
			constrain *free_con = *c;
			*c = (*c) -> next;
			add_constraint(g, trtype_get_param((*c) -> t1), trtype_get_param((*c) -> t1));
			add_constraint(g, trtype_get_res((*c) -> t1),trtype_get_res((*c) -> t1));
			c = &(g -> liste);
			i = 0;
		} else if (trtype_variable((*c) -> t2)) {
			trtype *buf;
			buf = (*c) -> t1;
			(*c) -> t1 = (*c) -> t2;
			(*c) -> t2 = buf;
			c = &(g -> liste);
			i = 0;
		} else if (!trtype_isIn((*c) -> t1, (*c) -> t2) && type_in_constraint(g, (*c) -> t1, i)) {
			replace_in_constraint(g, (*c) -> t1, (*c) -> t2, i);
			c = &(g -> liste);
			i = 0;
		} else {
		
			i++;
			*c = (*c) -> next;
		}
	}
	return 0;
}





