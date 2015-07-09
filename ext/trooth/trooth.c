#include "trooth.h"

void Init_trooth()
{
	mTrooth = rb_define_module("Trooth");

	Init_trooth_BigInt();
}
