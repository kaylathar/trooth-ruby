#include "trooth.h"
#include "trooth_bigint.h"
#include <trooth/bigint.h>
#include <ruby/ruby.h>

static TR_Environment* globalEnvironment;

static VALUE add(VALUE self, VALUE rb_object);
static VALUE subtract(VALUE self, VALUE rb_object);
static VALUE multiply(VALUE self, VALUE rb_object);
static VALUE not_equals(VALUE self, VALUE rb_object);
static VALUE equals(VALUE self, VALUE rb_object);
static VALUE greater_than(VALUE self, VALUE rb_object);
static VALUE less_than(VALUE self, VALUE rb_object);
static VALUE greater_than_equal(VALUE self, VALUE rb_object);
static VALUE less_than_equal(VALUE self, VALUE rb_object);
static VALUE absolute(VALUE self);
static VALUE initialize(VALUE self, VALUE rb_object);
static VALUE allocate(VALUE klass);
static VALUE deallocate(void* bigInt);

void Init_trooth_BigInt()
{
	cTroothBigInt = rb_define_class_under(mTrooth,"BigInt", rb_cObject);
	rb_define_alloc_func(cTroothBigInt, allocate);
	rb_define_method(cTroothBigInt, "initialize", initialize, 1);
	rb_define_method(cTroothBigInt, "+", add, 1);
	rb_define_method(cTroothBigInt, "-", subtract, 1);
	rb_define_method(cTroothBigInt, "*", multiply, 1);
	rb_define_method(cTroothBigInt, "==", equals, 1);
	rb_define_method(cTroothBigInt, "!=", not_equals, 1);
	rb_define_method(cTroothBigInt, "<", less_than, 1);
	rb_define_method(cTroothBigInt, ">", greater_than, 1);
	rb_define_method(cTroothBigInt, "<=", less_than_equal, 1);
	rb_define_method(cTroothBigInt, ">=", greater_than_equal, 1);
	rb_define_method(cTroothBigInt, "abs", absolute, 0);

	globalEnvironment = TR_Environment_alloc();
}

static VALUE add(VALUE self, VALUE rb_object)
{

}

static VALUE subtract(VALUE self, VALUE rb_object)
{

}

static VALUE multiply(VALUE self, VALUE rb_object)
{

}

static VALUE not_equals(VALUE self, VALUE rb_object)
{

}

static VALUE equals(VALUE self, VALUE rb_object)
{

}

static VALUE greater_than(VALUE self, VALUE rb_object)
{

}

static VALUE less_than(VALUE self, VALUE rb_object)
{

}

static VALUE greater_than_equal(VALUE self, VALUE rb_object)
{

}

static VALUE less_than_equal(VALUE self, VALUE rb_object)
{
	Trooth_BigIntWrapper* wrapper1,*wrapper2;

	/* Update this when we support comparing a string or FixNum... */
        Check_Type(rb_object,T_OBJECT);

	if (RBASIC(rb_object)->klass == cTroothBigInt)
	{
        	rb_raise(rb_eTypeError, "not a valid value");
	}

  Data_Get_Struct(self, Trooth_BigIntWrapper, wrapper1);
	Data_Get_Struct(rb_object, Trooth_BigIntWrapper, wrapper2);

	return TR_BigInt_compare(wrapper1->num,wrapper2->num)<1;

}

static VALUE absolute(VALUE self)
{


}



static VALUE initialize(VALUE self, VALUE rb_object)
{
	Trooth_BigIntWrapper* wrapper;

	Data_Get_Struct(self, Trooth_BigIntWrapper, wrapper);

	switch(TYPE(rb_object))
	{
		case T_FIXNUM:
			break;
		case T_BIGNUM:
			break;
		case T_STRING:
			wrapper->num = TR_BigInt_fromString(globalEnvironment, StringValueCStr(rb_object));
			break;
		default:
			rb_raise(rb_eTypeError, "not a valid value");
			break;
	}

	return self;
}

static VALUE allocate(VALUE klass)
{
	Trooth_BigIntWrapper* wrapper = malloc(sizeof(Trooth_BigIntWrapper));

	return Data_Wrap_Struct(klass, NULL, deallocate, wrapper);
}

static VALUE deallocate(void* bigInt)
{
	TR_BigInt_free((TR_BigInt*) bigInt);
}
