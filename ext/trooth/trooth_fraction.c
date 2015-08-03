#include "trooth.h"
#include "trooth_fraction.h"
#include <trooth/fraction.h>
#include <ruby/ruby.h>

static TR_Environment* globalEnvironment;
static VALUE cTroothFraction;

static void validate_is_Fraction(VALUE rb_object);
static VALUE numerator(VALUE self);
static VALUE denominator(VALUE self);
static VALUE add(VALUE self, VALUE rb_object);
static VALUE subtract(VALUE self, VALUE rb_object);
static VALUE multiply(VALUE self, VALUE rb_object);
static VALUE absolute(VALUE self);
static VALUE initialize(VALUE self, VALUE rb_object);
static VALUE allocate(VALUE klass);
static VALUE deallocate(void* bigInt);
static TR_Fraction* ruby_obj_to_fraction(VALUE rb_object);
static VALUE to_string(VALUE self);

void Init_trooth_Fraction()
{
	cTroothFraction = rb_define_class_under(mTrooth,"Fraction", rb_cObject);
	rb_define_alloc_func(cTroothFraction, allocate);
	rb_define_method(cTroothFraction, "initialize", initialize, 1);
	rb_define_method(cTroothFraction, "+", add, 1);
	rb_define_method(cTroothFraction, "-", subtract, 1);
	rb_define_method(cTroothFraction, "*", multiply, 1);
	rb_define_method(cTroothFraction, "abs", absolute, 0);
	rb_define_method(cTroothFraction, "to_s", to_string, 0);
  rb_define_method(cTroothFraction, "numerator", to_string, 0);
  rb_define_method(cTroothFraction, "denominator", to_string, 0);


	globalEnvironment = TR_Environment_alloc();
}

static void validate_is_Fraction(VALUE rb_object)
{
	if (RBASIC(rb_object)->klass != cTroothFraction)
	{
		rb_raise(rb_eTypeError, "not a valid value");
	}
}

static VALUE execute_arithmetic(VALUE val1, VALUE val2, FracArithmeticFunction func)
{
	Trooth_FractionWrapper *wrapper1,*wrapper2,*wrapper3;
	TR_Fraction* result;
	VALUE obj;

	validate_is_Fraction(val1);
	validate_is_Fraction(val2);

	Data_Get_Struct(val1, Trooth_FractionWrapper, wrapper1);
	Data_Get_Struct(val2, Trooth_FractionWrapper, wrapper2);

	if (!wrapper1->num || !wrapper2->num)
	{
		rb_raise(rb_eRuntimeError, "not fully initialized Fraction");
	}

	 result = func(wrapper1->num,wrapper2->num);
	 obj = allocate(cTroothFraction);

	 Data_Get_Struct(obj, Trooth_FractionWrapper, wrapper3);
	 wrapper3->num = result;
	 return obj;
}

static TR_Fraction* ruby_obj_to_fraction(VALUE rb_object)
{
	TR_Fraction *fraction = NULL;

	switch(TYPE(rb_object))
	{
		case T_STRING:
			fraction = TR_Fraction_fromString(globalEnvironment, StringValueCStr(rb_object));
			break;
		default:
			rb_raise(rb_eTypeError, "not a valid value");
			break;
	}

	return fraction;
}


static VALUE add(VALUE self, VALUE rb_object)
{
	return execute_arithmetic(self,rb_object,TR_Fraction_add);
}

static VALUE subtract(VALUE self, VALUE rb_object)
{
	return execute_arithmetic(self,rb_object,TR_Fraction_subtract);
}

static VALUE multiply(VALUE self, VALUE rb_object)
{
	return execute_arithmetic(self,rb_object,TR_Fraction_multiply);
}

static VALUE divide(VALUE self, VALUE rb_object)
{
	return execute_arithmetic(self,rb_object,TR_Fraction_divide);
}

static VALUE absolute(VALUE self)
{
	Trooth_FractionWrapper *wrapper1,*wrapper2;
	TR_Fraction* result;
	VALUE obj;

	validate_is_Fraction(self);

	Data_Get_Struct(self, Trooth_FractionWrapper, wrapper1);

	if (!wrapper1->num)
	{
		rb_raise(rb_eRuntimeError, "not fully initialized Fraction");
	}

	 result = TR_Fraction_absolute(wrapper1->num);

	 obj = allocate(cTroothFraction);

	 Data_Get_Struct(obj, Trooth_FractionWrapper, wrapper2);
	 wrapper2->num = result;
	 return obj;
}

static VALUE queryElement(VALUE self, TR_Fraction_ElementType type)
{
	Trooth_FractionWrapper *wrapper1;
  Trooth_BigIntWrapper *wrapper2;
	TR_BigInt* result;
	VALUE obj;

	validate_is_Fraction(self);

	Data_Get_Struct(self, Trooth_FractionWrapper, wrapper1);

	if (!wrapper1->num)
	{
		rb_raise(rb_eRuntimeError, "not fully initialized Fraction");
	}

  switch (type)
  {
    case Fraction_Numerator:
      result = TR_Fraction_numerator(wrapper1->num);
      break;
    case Fraction_Denominator:
      result = TR_Fraction_denominator(wrapper1->num);
      break;
  }

	obj = allocate(cTroothBigInt);

	Data_Get_Struct(obj, Trooth_BigIntWrapper, wrapper2);
	wrapper2->num = result;
	return obj;
}

static VALUE numerator(VALUE self)
{
	 return queryElement(self,Fraction_Numerator);
}

static VALUE denominator(VALUE self)
{
    return queryElement(self,Fraction_Denominator);
}

static VALUE initialize(VALUE self, VALUE rb_object)
{
	Trooth_FractionWrapper* wrapper;

	Data_Get_Struct(self, Trooth_FractionWrapper, wrapper);

	wrapper->num = ruby_obj_to_fraction(rb_object);

	return self;
}

static VALUE to_string(VALUE self)
{
	Trooth_FractionWrapper *wrapper;
	char* result;
	VALUE obj;

	validate_is_Fraction(self);

	Data_Get_Struct(self, Trooth_FractionWrapper, wrapper);

	if (!wrapper->num)
	{
		rb_raise(rb_eRuntimeError, "not fully initialized Fraction");
	}

	 result = TR_Fraction_toString(wrapper->num);
	 obj = rb_str_new2(result);
	 free(result);

	 return obj;
}

static VALUE allocate(VALUE klass)
{
	Trooth_FractionWrapper* wrapper = malloc(sizeof(Trooth_FractionWrapper));
	memset(wrapper,0,sizeof(Trooth_FractionWrapper));

	return Data_Wrap_Struct(klass, NULL, deallocate, wrapper);
}

static VALUE deallocate(void* bigInt)
{
	Trooth_FractionWrapper* wrapper = (Trooth_FractionWrapper*)bigInt;
	if (wrapper->num)
	{
			TR_Fraction_free(wrapper->num);
			wrapper->num = NULL;
	}
	free(wrapper);
	return Qnil;
}
