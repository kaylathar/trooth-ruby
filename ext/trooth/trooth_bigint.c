#include "trooth.h"
#include "trooth_bigint.h"
#include <trooth/bigint.h>
#include <ruby/ruby.h>

VALUE cTroothBigInt;

static void validate_is_BigInt(VALUE rb_object);
static int compare_BigInt(VALUE val1, VALUE val2);
static VALUE add(VALUE self, VALUE rb_object);
static VALUE subtract(VALUE self, VALUE rb_object);
static VALUE multiply(VALUE self, VALUE rb_object);
static VALUE exponentiate(VALUE self, VALUE rb_object);
static VALUE divide(VALUE self, VALUE rb_object);
static VALUE modulo(VALUE self, VALUE rb_object);
static VALUE not_equals(VALUE self, VALUE rb_object);
static VALUE equals(VALUE self, VALUE rb_object);
static VALUE greater_than(VALUE self, VALUE rb_object);
static VALUE less_than(VALUE self, VALUE rb_object);
static VALUE greater_than_equal(VALUE self, VALUE rb_object);
static VALUE less_than_equal(VALUE self, VALUE rb_object);
static VALUE compare(VALUE self, VALUE rb_object);
static VALUE absolute(VALUE self);
static VALUE initialize(VALUE self, VALUE rb_object);
static VALUE allocate(VALUE klass);
static VALUE deallocate(void* bigInt);
static TR_BigInt* ruby_obj_to_bigint(VALUE rb_object);
static VALUE to_string(VALUE self);
static VALUE to_integer(VALUE self);

void Init_trooth_BigInt()
{
	cTroothBigInt = rb_define_class_under(mTrooth,"BigInt", rb_cObject);
	rb_define_alloc_func(cTroothBigInt, allocate);
	rb_define_method(cTroothBigInt, "initialize", initialize, 1);
	rb_define_method(cTroothBigInt, "+", add, 1);
	rb_define_method(cTroothBigInt, "-", subtract, 1);
	rb_define_method(cTroothBigInt, "*", multiply, 1);
	rb_define_method(cTroothBigInt, "**", exponentiate, 1);
	rb_define_method(cTroothBigInt, "/", divide, 1);
	rb_define_method(cTroothBigInt, "%", modulo, 1);
	rb_define_method(cTroothBigInt, "==", equals, 1);
	rb_define_method(cTroothBigInt, "!=", not_equals, 1);
	rb_define_method(cTroothBigInt, "<", less_than, 1);
	rb_define_method(cTroothBigInt, ">", greater_than, 1);
	rb_define_method(cTroothBigInt, "<=", less_than_equal, 1);
	rb_define_method(cTroothBigInt, ">=", greater_than_equal, 1);
	rb_define_method(cTroothBigInt, "<=>", compare, 1);
	rb_define_method(cTroothBigInt, "abs", absolute, 0);
	rb_define_method(cTroothBigInt, "to_s", to_string, 0);
	rb_define_method(cTroothBigInt, "to_i", to_integer, 0);
}

static void validate_is_BigInt(VALUE rb_object)
{
	if (RBASIC(rb_object)->klass != cTroothBigInt)
	{
		rb_raise(rb_eTypeError, "not a valid value");
	}
}

static int compare_BigInt(VALUE val1, VALUE val2)
{
	Trooth_BigIntWrapper *wrapper1,*wrapper2;

	validate_is_BigInt(val1);
	validate_is_BigInt(val2);

	Data_Get_Struct(val1, Trooth_BigIntWrapper, wrapper1);
	Data_Get_Struct(val2, Trooth_BigIntWrapper, wrapper2);

	return TR_BigInt_compare(wrapper1->num,wrapper2->num);

}

static VALUE execute_arithmetic(VALUE val1, VALUE val2, ArithmeticFunction func)
{
	Trooth_BigIntWrapper *wrapper1,*wrapper2,*wrapper3;
	TR_BigInt* result;
	VALUE obj;

	validate_is_BigInt(val1);
	validate_is_BigInt(val2);

	Data_Get_Struct(val1, Trooth_BigIntWrapper, wrapper1);
	Data_Get_Struct(val2, Trooth_BigIntWrapper, wrapper2);

	if (!wrapper1->num || !wrapper2->num)
	{
		rb_raise(rb_eRuntimeError, "not fully initialized BigInt");
	}

	 result = func(wrapper1->num,wrapper2->num);
	 obj = allocate(cTroothBigInt);

	 Data_Get_Struct(obj, Trooth_BigIntWrapper, wrapper3);
	 wrapper3->num = result;
	 return obj;
}

static TR_BigInt* ruby_obj_to_bigint(VALUE rb_object)
{
	TR_BigInt *bigInt = NULL;
	VALUE tmpStr;
	ID toStringSymbol;
	ID toIntSymbol;

	switch(TYPE(rb_object))
	{
		/* When API is available, we can handle these faster */
		case T_FLOAT:
			toIntSymbol = rb_intern("to_int");
			rb_object = rb_funcall(rb_object,toIntSymbol,0);
			/* Intentional fall-through */
		case T_BIGNUM:
		case T_FIXNUM:
			toStringSymbol = rb_intern("to_s");
			tmpStr = rb_funcall(rb_object,toStringSymbol,0);
			bigInt = TR_BigInt_fromString(globalEnvironment, StringValueCStr(tmpStr));
			break;			
		case T_STRING:
			bigInt = TR_BigInt_fromString(globalEnvironment, StringValueCStr(rb_object));
			break;
		default:
			rb_raise(rb_eTypeError, "not a valid value");
			break;
	}

	return bigInt;
}

static VALUE divide(VALUE self, VALUE rb_object)
{
	Trooth_BigIntWrapper *wrapper1,*wrapper2,*wrapper3;
	TR_BigInt_DivisionResult* result;
	VALUE obj;

	validate_is_BigInt(self);
	validate_is_BigInt(rb_object);

	Data_Get_Struct(self, Trooth_BigIntWrapper, wrapper1);
	Data_Get_Struct(rb_object, Trooth_BigIntWrapper, wrapper2);

	if (!wrapper1->num || !wrapper2->num)
	{
		rb_raise(rb_eRuntimeError, "not fully initialized BigInt");
	}

	 result = TR_BigInt_divide(wrapper1->num,wrapper2->num);

	 obj = allocate(cTroothBigInt);
	 Data_Get_Struct(obj, Trooth_BigIntWrapper, wrapper3);
	 wrapper3->num = TR_BigInt_DivisionResult_quotient(result);

	 TR_BigInt_DivisionResult_free(result);
	 return obj;
}

static VALUE modulo(VALUE self, VALUE rb_object)
{
	Trooth_BigIntWrapper *wrapper1,*wrapper2,*wrapper3;
	TR_BigInt_DivisionResult* result;
	VALUE obj;

	validate_is_BigInt(self);
	validate_is_BigInt(rb_object);

	Data_Get_Struct(self, Trooth_BigIntWrapper, wrapper1);
	Data_Get_Struct(rb_object, Trooth_BigIntWrapper, wrapper2);

	if (!wrapper1->num || !wrapper2->num)
	{
		rb_raise(rb_eRuntimeError, "not fully initialized BigInt");
	}

	 result = TR_BigInt_divide(wrapper1->num,wrapper2->num);

	 obj = allocate(cTroothBigInt);
	 Data_Get_Struct(obj, Trooth_BigIntWrapper, wrapper3);
	 wrapper3->num = TR_BigInt_DivisionResult_remainder(result);

	 TR_BigInt_DivisionResult_free(result);
	 return obj;
}

static VALUE add(VALUE self, VALUE rb_object)
{
	return execute_arithmetic(self,rb_object,TR_BigInt_add);
}

static VALUE subtract(VALUE self, VALUE rb_object)
{
	return execute_arithmetic(self,rb_object,TR_BigInt_subtract);
}

static VALUE multiply(VALUE self, VALUE rb_object)
{
	return execute_arithmetic(self,rb_object,TR_BigInt_multiply);
}

static VALUE exponentiate(VALUE self, VALUE rb_object)
{
	return execute_arithmetic(self,rb_object,TR_BigInt_exponentiate);
}

static VALUE not_equals(VALUE self, VALUE rb_object)
{
	return compare_BigInt(self,rb_object)!=0?Qtrue:Qfalse;
}

static VALUE equals(VALUE self, VALUE rb_object)
{
	return compare_BigInt(self,rb_object)==0?Qtrue:Qfalse;
}

static VALUE greater_than(VALUE self, VALUE rb_object)
{
	return compare_BigInt(self,rb_object)==1?Qtrue:Qfalse;
}

static VALUE less_than(VALUE self, VALUE rb_object)
{
	return compare_BigInt(self,rb_object)==-1?Qtrue:Qfalse;
}

static VALUE compare(VALUE self, VALUE rb_object)
{
	return INT2NUM(compare_BigInt(self,rb_object));
}

static VALUE greater_than_equal(VALUE self, VALUE rb_object)
{
	return compare_BigInt(self,rb_object)>-1?Qtrue:Qfalse;
}

static VALUE less_than_equal(VALUE self, VALUE rb_object)
{
	return compare_BigInt(self,rb_object)<1?Qtrue:Qfalse;
}

static VALUE absolute(VALUE self)
{
	Trooth_BigIntWrapper *wrapper1,*wrapper2;
	TR_BigInt* result;
	VALUE obj;

	validate_is_BigInt(self);

	Data_Get_Struct(self, Trooth_BigIntWrapper, wrapper1);

	if (!wrapper1->num)
	{
		rb_raise(rb_eRuntimeError, "not fully initialized BigInt");
	}

	 result = TR_BigInt_absolute(wrapper1->num);

	 obj = allocate(cTroothBigInt);

	 Data_Get_Struct(obj, Trooth_BigIntWrapper, wrapper2);
	 wrapper2->num = result;
	 return obj;
}

static VALUE initialize(VALUE self, VALUE rb_object)
{
	Trooth_BigIntWrapper* wrapper;

	Data_Get_Struct(self, Trooth_BigIntWrapper, wrapper);

	wrapper->num = ruby_obj_to_bigint(rb_object);

	return self;
}

static VALUE to_string(VALUE self)
{
	Trooth_BigIntWrapper *wrapper;
	char* result;
	VALUE obj;

	validate_is_BigInt(self);

	Data_Get_Struct(self, Trooth_BigIntWrapper, wrapper);

	if (!wrapper->num)
	{
		rb_raise(rb_eRuntimeError, "not fully initialized BigInt");
	}

	 result = TR_BigInt_toString(wrapper->num);
	 obj = rb_str_new2(result);
	 free(result);

	 return obj;
}


static VALUE to_integer(VALUE self)
{
	Trooth_BigIntWrapper *wrapper;
	char* result;
	VALUE obj;
	ID toIntSymbol;

	validate_is_BigInt(self);

	Data_Get_Struct(self, Trooth_BigIntWrapper, wrapper);

	if (!wrapper->num)
	{
		rb_raise(rb_eRuntimeError, "not fully initialized BigInt");
	}

	 result = TR_BigInt_toString(wrapper->num);
	 obj = rb_str_new2(result);
	 toIntSymbol = rb_intern("to_i");
	 obj = rb_funcall(obj,toIntSymbol,0);

	 free(result);

	 return obj;
}

static VALUE allocate(VALUE klass)
{
	Trooth_BigIntWrapper* wrapper = malloc(sizeof(Trooth_BigIntWrapper));
	memset(wrapper,0,sizeof(Trooth_BigIntWrapper));

	return Data_Wrap_Struct(klass, NULL, deallocate, wrapper);
}

static VALUE deallocate(void* bigInt)
{
	Trooth_BigIntWrapper* wrapper = (Trooth_BigIntWrapper*)bigInt;
	if (wrapper->num)
	{
			TR_BigInt_free(wrapper->num);
			wrapper->num = NULL;
	}
	free(wrapper);
	return Qnil;
}
