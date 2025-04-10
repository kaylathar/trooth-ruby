#ifndef TROOTH_BIGINT_H
#define TROOTH_BIGINT_H
#include <trooth/bigint.h>

extern VALUE cTroothBigInt;
void Init_trooth_BigInt();

typedef struct {
	TR_BigInt* num;
} Trooth_BigIntWrapper;

typedef TR_BigInt* (*ArithmeticFunction)(TR_BigInt *op1, TR_BigInt *op2);

#endif
