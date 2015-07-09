#ifndef TROOTH_BIGINT_H
#define TROOTH_BIGINT_H
#include <trooth/bigint.h>

void Init_trooth_BigInt();

extern VALUE cTroothBigInt;

typedef struct {
	TR_BigInt* num;
} Trooth_BigIntWrapper;

#endif
