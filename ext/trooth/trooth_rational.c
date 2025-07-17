#include "trooth.h"
#include "trooth_rational.h"
#include <trooth/rational.h>
#include <ruby/ruby.h>

static VALUE cTroothRational;
static void validate_is_Rational(VALUE rb_object);
static VALUE numerator(VALUE self);
static VALUE denominator(VALUE self);
static VALUE add(VALUE self, VALUE rb_object);
static VALUE subtract(VALUE self, VALUE rb_object);
static VALUE multiply(VALUE self, VALUE rb_object);
static VALUE divide(VALUE self, VALUE rb_object);
static VALUE not_equals(VALUE self, VALUE rb_object);
static VALUE equals(VALUE self, VALUE rb_object);
static VALUE greater_than(VALUE self, VALUE rb_object);
static VALUE less_than(VALUE self, VALUE rb_object);
static VALUE greater_than_equal(VALUE self, VALUE rb_object);
static VALUE less_than_equal(VALUE self, VALUE rb_object);
static VALUE compare(VALUE self, VALUE rb_object);
static VALUE absolute(VALUE self);
static VALUE initialize(int argc, VALUE* argv, VALUE self);
static VALUE allocate(VALUE klass);
static VALUE deallocate(void* bigInt);
static TR_Rational* ruby_obj_to_rational(VALUE rb_object);
static VALUE denominator_to_string(VALUE self);
static VALUE numerator_to_string(VALUE self);
static VALUE to_string(VALUE self);

void Init_trooth_Rational()
{
	cTroothRational = rb_define_class_under(mTrooth,"Rational", rb_cObject);
	rb_define_alloc_func(cTroothRational, allocate);
	rb_define_method(cTroothRational, "initialize", initialize, -1);
	rb_define_method(cTroothRational, "+", add, 1);
	rb_define_method(cTroothRational, "-", subtract, 1);
	rb_define_method(cTroothRational, "*", multiply, 1);
	rb_define_method(cTroothRational, "/", divide, 1);
	rb_define_method(cTroothRational, "==", equals, 1);
	rb_define_method(cTroothRational, "!=", not_equals, 1);
	rb_define_method(cTroothRational, "<", less_than, 1);
	rb_define_method(cTroothRational, ">", greater_than, 1);
	rb_define_method(cTroothRational, "<=", less_than_equal, 1);
	rb_define_method(cTroothRational, ">=", greater_than_equal, 1);
	rb_define_method(cTroothRational, "<=>", compare, 1);
	rb_define_method(cTroothRational, "abs", absolute, 0);
	rb_define_method(cTroothRational, "to_s", to_string, 0);
  	rb_define_method(cTroothRational, "numerator", numerator, 0);
  	rb_define_method(cTroothRational, "denominator", denominator, 0);
}

static void validate_is_Rational(VALUE rb_object)
{
	if (RBASIC(rb_object)->klass != cTroothRational)
	{
		rb_raise(rb_eTypeError, "not a valid value");
	}
}

static VALUE execute_arithmetic(VALUE val1, VALUE val2, FracArithmeticFunction func)
{
	Trooth_RationalWrapper *wrapper1,*wrapper2,*wrapper3;
	TR_Rational* result;
	VALUE obj;

	validate_is_Rational(val1);
	validate_is_Rational(val2);

	Data_Get_Struct(val1, Trooth_RationalWrapper, wrapper1);
	Data_Get_Struct(val2, Trooth_RationalWrapper, wrapper2);

	if (!wrapper1->num || !wrapper2->num)
	{
		rb_raise(rb_eRuntimeError, "not fully initialized Rational");
	}

	 result = func(wrapper1->num,wrapper2->num);
	 obj = allocate(cTroothRational);

	 Data_Get_Struct(obj, Trooth_RationalWrapper, wrapper3);
	 wrapper3->num = result;
	 return obj;
}

static TR_Rational* ruby_obj_to_rational(VALUE rb_object)
{
	TR_Rational *rational = NULL;

	switch(TYPE(rb_object))
	{
		case T_STRING:
			rational = TR_Rational_fromString(globalEnvironment, StringValueCStr(rb_object));
			break;
		default:
			rb_raise(rb_eTypeError, "not a valid value");
			break;
	}

	return rational;
}


static VALUE add(VALUE self, VALUE rb_object)
{
	return execute_arithmetic(self,rb_object,TR_Rational_add);
}

static VALUE subtract(VALUE self, VALUE rb_object)
{
	return execute_arithmetic(self,rb_object,TR_Rational_subtract);
}

static VALUE multiply(VALUE self, VALUE rb_object)
{
	return execute_arithmetic(self,rb_object,TR_Rational_multiply);
}

static VALUE divide(VALUE self, VALUE rb_object)
{
	return execute_arithmetic(self,rb_object,TR_Rational_divide);
}

static VALUE absolute(VALUE self)
{
	Trooth_RationalWrapper *wrapper1,*wrapper2;
	TR_Rational* result;
	VALUE obj;

	validate_is_Rational(self);

	Data_Get_Struct(self, Trooth_RationalWrapper, wrapper1);

	if (!wrapper1->num)
	{
		rb_raise(rb_eRuntimeError, "not fully initialized Rational");
	}

	 result = TR_Rational_absolute(wrapper1->num);

	 obj = allocate(cTroothRational);

	 Data_Get_Struct(obj, Trooth_RationalWrapper, wrapper2);
	 wrapper2->num = result;
	 return obj;
}

static int compare_Rational(VALUE self, VALUE rb_object)
{
	Trooth_RationalWrapper *wrapper1,*wrapper2;

	validate_is_Rational(self);
	validate_is_Rational(rb_object);

	Data_Get_Struct(self, Trooth_RationalWrapper, wrapper1);
	Data_Get_Struct(rb_object, Trooth_RationalWrapper, wrapper2);

	return TR_Rational_compare(wrapper1->num, wrapper2->num);
}

static VALUE not_equals(VALUE self, VALUE rb_object)
{
	return compare_Rational(self, rb_object) != 0?Qtrue:Qfalse;
}

static VALUE equals(VALUE self, VALUE rb_object)
{
	return compare_Rational(self, rb_object) == 0?Qtrue:Qfalse;
}

static VALUE greater_than(VALUE self, VALUE rb_object)
{
	return compare_Rational(self, rb_object) == 1?Qtrue:Qfalse;
}

static VALUE less_than(VALUE self, VALUE rb_object)
{
	return compare_Rational(self, rb_object) == -1?Qtrue:Qfalse;
}

static VALUE compare(VALUE self, VALUE rb_object)
{
	return INT2NUM(compare_Rational(self, rb_object));
}

static VALUE greater_than_equal(VALUE self, VALUE rb_object)
{
	return compare_Rational(self, rb_object) > -1?Qtrue:Qfalse;
}

static VALUE less_than_equal(VALUE self, VALUE rb_object)
{
	return compare_Rational(self, rb_object) < 1?Qtrue:Qfalse;
}

static VALUE queryElement(VALUE self, TR_Rational_ElementType type)
{
	Trooth_RationalWrapper *wrapper1;
  	Trooth_BigIntWrapper *wrapper2;
	TR_BigInt* result;
	VALUE obj;

	validate_is_Rational(self);

	Data_Get_Struct(self, Trooth_RationalWrapper, wrapper1);

	if (!wrapper1->num)
	{
		rb_raise(rb_eRuntimeError, "not fully initialized Rational");
	}

  switch (type)
  {
    case Rational_Numerator:
      result = TR_Rational_numerator(wrapper1->num);
      break;
    case Rational_Denominator:
      result = TR_Rational_denominator(wrapper1->num);
      break;
  }

	obj = allocate(cTroothBigInt);

	Data_Get_Struct(obj, Trooth_BigIntWrapper, wrapper2);
	wrapper2->num = result;
	return obj;
}

static VALUE numerator(VALUE self)
{
	 return queryElement(self,Rational_Numerator);
}

static VALUE denominator(VALUE self)
{
    return queryElement(self,Rational_Denominator);
}

static VALUE convertToBigInt(VALUE arg)
{
	VALUE args[1];

	switch (TYPE(arg))
	{
		case T_FLOAT:
		case T_BIGNUM:
		case T_FIXNUM:
		case T_STRING:
			args[0] = arg;
			arg = rb_class_new_instance(1, args, cTroothBigInt);
		default:
			if (RBASIC(arg)->klass != cTroothBigInt) {
				rb_raise(rb_eTypeError, "parameters must be of type BigInt or convertible to BigInt");
			}
	}

	return arg;
}

static VALUE initialize(int argc, VALUE* argv, VALUE self)
{
	Trooth_RationalWrapper* wrapper;
	Trooth_BigIntWrapper *num, *den;
	VALUE arg1;
	VALUE arg2;

	Data_Get_Struct(self, Trooth_RationalWrapper, wrapper);

	if (argc == 2)
	{
		arg1 = convertToBigInt(argv[0]);
		arg2 = convertToBigInt(argv[1]);

		Data_Get_Struct(arg1, Trooth_BigIntWrapper, num);
		Data_Get_Struct(arg2, Trooth_BigIntWrapper, den);

		if (num->num == NULL || den->num == NULL)
		{
			rb_raise(rb_eRuntimeError, "not fully initialized BigInt");
		}

		wrapper->num = TR_Rational_fromIntegers(num->num,den->num);
	} else if (argc == 1) {
		wrapper->num = ruby_obj_to_rational(argv[0]);
	} else {
		rb_raise(rb_eRuntimeError, "not enough or too many parameters");
	}

	return self;
}

static VALUE to_string(VALUE self)
{
	Trooth_RationalWrapper *wrapper;
	char* result;
	VALUE obj;

	validate_is_Rational(self);

	Data_Get_Struct(self, Trooth_RationalWrapper, wrapper);

	if (!wrapper->num)
	{
		rb_raise(rb_eRuntimeError, "not fully initialized Rational");
	}

	 result = TR_Rational_toString(wrapper->num);
	 obj = rb_str_new2(result);
	 free(result);

	 return obj;
}

static VALUE allocate(VALUE klass)
{
	Trooth_RationalWrapper* wrapper = malloc(sizeof(Trooth_RationalWrapper));
	memset(wrapper,0,sizeof(Trooth_RationalWrapper));

	return Data_Wrap_Struct(klass, NULL, deallocate, wrapper);
}

static VALUE deallocate(void* bigInt)
{
	Trooth_RationalWrapper* wrapper = (Trooth_RationalWrapper*)bigInt;
	if (wrapper->num)
	{
			TR_Rational_free(wrapper->num);
			wrapper->num = NULL;
	}
	free(wrapper);
	return Qnil;
}
