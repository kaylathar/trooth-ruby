#ifndef TROOTH_FRACTION_H
#define TROOTH_FRACTION_H
#include <trooth/fraction.h>

void Init_trooth_Fraction();

typedef struct {
	TR_Fraction* num;
} Trooth_FractionWrapper;

typedef enum {
  Fraction_Numerator,
  Fraction_Denominator
} TR_Fraction_ElementType;

typedef TR_Fraction* (*FracArithmeticFunction)(TR_Fraction *op1, TR_Fraction *op2);

#endif
