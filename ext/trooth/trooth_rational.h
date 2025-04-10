#ifndef TROOTH_RATIONAL_H
#define TROOTH_RATIONAL_H
#include <trooth/rational.h>

void Init_trooth_Rational();

typedef struct {
	TR_Rational* num;
} Trooth_RationalWrapper;

typedef enum {
  Rational_Numerator,
  Rational_Denominator
} TR_Rational_ElementType;

typedef TR_Rational* (*FracArithmeticFunction)(TR_Rational *op1, TR_Rational *op2);

#endif
