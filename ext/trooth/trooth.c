#include "trooth.h"

VALUE mTrooth;

void Init_trooth()
{
	mTrooth = rb_define_module("Trooth");

	Init_trooth_BigInt();
	Init_trooth_Fraction();
}
