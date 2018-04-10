#ifndef MLOPER_H
#define MLOPER_H

enum mloper {
	INTPLUS, INTMINUS, INTMULT, INTDIV, FLOATPLUS, 
	FLOATMINUS, FLOATMULT, FLOATDIV, STRCNT, PUTLIST, CONCATLIST,
	GETHAN, GTHAN, EQ, LETHAN, LTHAN, // Opérateurs de comparaisons
	AND, OR, NOT, 
	AFFECT, LET, LETIN,
	APPLY
};

typedef enum mloper mloper;

#endif //MLOPER_H
