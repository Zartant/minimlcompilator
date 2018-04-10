#ifndef TYPE_CONTR_H
#define TYPE_CONTR_H

#include "typetree.h"

typedef struct G G;
typedef struct constrain constrain;

G *new_constraints_set();

void add_constraint(G *g, trtype *t1, trtype *t2);

void print_constraint_set(G *g);

void print_constraint(constrain *c);


int get_listepol(G *g);

void incr_listepol(G *g);

int infer_type(G *g);

#endif //TYPE_CONTR_H
