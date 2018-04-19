#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "mltype.h"
#include "typetree.h"
#include "typecontr.h"
#include "envi.h"
#include "exprtree.h"

int main(void) {
    trtype *integer1 = trtype_create_complex(strtype_create_primitiv(INTEGER));
    trtype *integer2 = trtype_create_complex(strtype_create_primitiv(INTEGER));
    trtype *integer3 = trtype_create_complex(strtype_create_primitiv(INTEGER));
    trtype *string1 = trtype_create_complex(strtype_create_primitiv(STRING));
    trtype *string2 = trtype_create_complex(strtype_create_primitiv(STRING));
    trtype *string3 = trtype_create_complex(strtype_create_primitiv(STRING));
    trtype *fl1 = trtype_create_complex(strtype_create_primitiv(FLOAT));
    trtype *fl2 = trtype_create_complex(strtype_create_primitiv(INTEGER));
    trtype *t1[2];
    trtype *t2[2];
    trtype *t3[2];
    trtype *t4[2];
    trtype *t5[2];
    t1[0] = integer1;
    t1[1] = string1;
    t2[0] = string2;
    t2[1] = integer2;
    t3[0] = integer3;
    t3[1] = string3;
    trtype *type1 = trtype_create_complex(strtype_upletfuse(t1, 2));
    trtype *type2 = trtype_create_complex(strtype_upletfuse(t2, 2));
    trtype *type3 = trtype_create_complex(strtype_upletfuse(t3, 2));

    printf("type1 == type2 -> %d\n", trtype_cmp(type1, type2));
    printf("type1 == type3 -> %d\n", trtype_cmp(type1, type3));
    printf("type3 == type2 -> %d\n", trtype_cmp(type3, type2));

    t4[0] = type1;
    t4[1] = fl1;
    t5[0] = type3;
    t5[1] = fl2;

    trtype *type4 = trtype_create_complex(strtype_upletfuse(t4, 2));
    trtype *type5 = trtype_create_complex(strtype_upletfuse(t5, 2));

    printf("type 4 == type 5 -> %d\n",trtype_cmp(type4, type5));


    printf("type 4 : "); trtype_print(type4); printf("\n");
    printf("type 5 : "); trtype_print(type5); printf("\n");

    trtype *fun = trtype_create_complex(strtype_create_fun(type4, type5));
    printf("4 -> 5  : "); trtype_print(fun); printf("\n");

    //On cherche maintenant à créer et à évaluer let f = function x -> function c -> let (a, b) = c in x + (a b);;
			
    //D'abord on créer tout le matériel nécessaire (qui sera plus tard générer automatiquement via bison
    //Un contexte + un ensemble de contrainte

    G *g = new_constraints_set();

    env *ctx = new_context();

    // On créer l'expression. Attention ici, la valeur de poltype est importante et doit être différente pour chaque expression

    /* trexpr *c1 = trexpr_create_pol(1, "c"); trtype_print(c1 -> treetype);
    trexpr *a1 = trexpr_create_pol(2, "a"); trtype_print(a1 -> treetype);
    trexpr *b1 = trexpr_create_pol(3, "b"); trtype_print(b1 -> treetype);
    trexpr *x = trexpr_create_pol(11, "x");
    trexpr *t[2];
    t[0] = a1; t[1] = b1;
    trexpr *couple = trexpr_upletfuse(t, 2, trtype_create_pol(4)); trtype_print(couple -> treetype);
    trexpr *c2 = trexpr_create_pol(5, "c");
    trexpr *affect = trexpr_create_composite(AFFECT, couple, c2, trtype_create_pol(6)); 
    trexpr *a2 = trexpr_create_pol(7, "a");
    trexpr *b2 = trexpr_create_pol(8, "b");
    trexpr *apply= trexpr_create_composite(APPLY, a2, b2, trtype_create_pol(9));
    //trexpr *plus2 = trexpr_create_composite(INTPLUS, plus1, trexpr_create(INTEGER, &i, ""), trtype_create_pol(10) ); 
    trexpr *apply2= trexpr_create_composite(INTPLUS, x, apply, trtype_create_pol(18));
    trexpr *let = trexpr_create_composite(LETIN, affect, apply2, trtype_create_pol(10));
    trexpr *f =  trexpr_create_fun(c1, let, trtype_create_pol(0));
    trexpr *f2 = trexpr_create_fun(x, f, trtype_create_pol(-1)); */
    
    
    //Expression suivante : let f = function x -> function c -> 3 :: (c@x);;
    trexpr *c1 = trexpr_create_pol(1, "c");
    trexpr *x1 = trexpr_create_pol(2, "x");
    trexpr *c2 = trexpr_create_pol(3, "c");
    trexpr *x2 = trexpr_create_pol(4, "x");
    trexpr *conc = trexpr_create_composite(CONCATLIST, c2, x2, trtype_create_pol(5));
    
    char v[50] = "";
    int v2 = 3;
    trexpr *texpr[2];
    texpr[0] = trexpr_create(STRING, v, "");
    texpr[1] = trexpr_create(INTEGER, &v2, ""); 
    trexpr *couple = trexpr_upletfuse(texpr, 2, trtype_create_pol(23));
    trexpr *insert = trexpr_create_composite(PUTLIST, couple, conc, trtype_create_pol(18));
    trexpr *f1 = trexpr_create_fun(c1, insert, trtype_create_pol(6));
    trexpr *f = trexpr_create_fun(x1, f1, trtype_create_pol(7)); 
    printf("patate\n");
    //add_to_a_context(affect, ctx, g); 
    apply_context((&(f)), ctx, g);
    printf("\nG valeurs : \n");
    print_constraint_set(g);
    
    infer_type(g);
    printf("\nG valeurs : \n");
    print_constraint_set(g);
    /*
    int n1 = 6;
    int n2 = 2;
    int n3 = 3;
    trexpr *b1 = trexpr_create(INTEGER, &n1, "");
    trexpr *b2 = trexpr_create(INTEGER, &n2, "");
    trexpr *b3 = trexpr_create(INTEGER, &n3, "");
    trexpr *tb[2];
    tb[0] = b2;
    tb[1] = b3;
    trexpr *tbexp = trexpr_upletfuse(tb, 2, trtype_create_pol(5));
    trexpr *p2 = trexpr_create_composite(PUTLIST, tbexp, trexpr_create(LIST, NULL, ""), trtype_create_pol(2));
    trexpr *p1 = trexpr_create_composite(PUTLIST, b1, p2, trtype_create_pol(3));
    */
    
    if (evaluate_expr(&f, ctx) == -1) {
		printf("error");
	} else {
   
    printf("Res : ");print_expr(f);printf("\n");
}


    return 0; 
}
