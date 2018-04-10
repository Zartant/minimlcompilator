#ifndef CONTEXT_H
#define CONTEXT_H

#define IDSIZEMAX 50
#include "exprtree.h"
#include "typecontr.h"

typedef struct env env;
typedef struct var var;
struct env {
	var *liste;
};

struct var {
	var *next;

	char id[IDSIZEMAX];
	trexpr *expr;
};




env *new_context();

void add_value(env *e, char *id, trexpr *ex);



void concat_context(env *e1, env *e2);

trexpr *getvar(env *e, char *id);

void add_to_a_context (trexpr **exp, env *ctx, G *ctrset);

void apply_context(trexpr **exp, env *ctx, G *ctrset);



#endif //CONTEXT_H
