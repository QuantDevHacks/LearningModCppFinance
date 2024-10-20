#pragma once
#include "EnumsAndEnumClasses.h"

// NewFeatures.cpp
void new_features();			// Top level calling test function
void uniform_initialization_size_t_and_auto();
void range_based_for_loop();
void typedef_and_using();
void more_uniform_initialization();
void vector_unif_init_exception();
void std_format_example();
void ctad_example();

// EnumExamples.cpp
void enum_examples();			// Top level calling test function
void enum_const_examples(); 
void enum_class_examples();
void switch_statement_scoped_enum(Bond_Type bnd);

void lambda_expressions();		// Top level calling test function
void hello_world();
void more_lambda_examples();


// MathFunctions and Constants
void math_functions_and_constants();	// Top level calling test function
double trig_fcn(double theta, double phi);
double zero_coupon_bond(double face_value, double int_rate, double year_fraction);
double f(double x);
double f_pow(double x);			
double g(double x, double y);
double math_constant_fcn(double x, double y);