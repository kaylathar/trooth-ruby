#include "trooth.h"

VALUE mTrooth;
TR_Environment* globalEnvironment;

void Init_trooth()
{
	mTrooth = rb_define_module("Trooth");
	globalEnvironment = TR_Environment_alloc();

	Init_trooth_BigInt();
	Init_trooth_Fraction();
}
