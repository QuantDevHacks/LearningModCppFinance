/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "Declarations.h"

#include <cmath>
#include <numbers>
#include <iostream>
#include <format>

void math_functions_and_constants()
{
	using std::cout, std::format;

	cout << "\n***** math_functions_and_constants() ***** (top calling function)\n";

	// Examples using functions that follow:
	cout << "\n\n*** trig_fcn(.) ***\n";
	cout << format("sin({}) + cos({}) = {}\n", 
		std::numbers::pi, std::numbers::pi, trig_fcn(std::numbers::pi, std::numbers::pi));

	cout << "\n*** zero_coupon_bond(.) ***\n";
	cout << format("zero_coupon_bond({}, {}, {}) = {}\n",
		1000.0, 0.06, 0.05, zero_coupon_bond(1000.0, 0.06, 0.05));

	cout << "\n*** polynomial using Horner's Method f(.) ***\n";
	cout << format("f({}) = {}\n",
		1.0, f(1.0));

	cout << "\n*** polynomial using brute force f_pow(.) (version of f(.) using std::pow(.)) ***\n";
	cout << format("f_pow({}) = {}\n",
		1.0, f_pow(1.0));

	cout << "\n*** real exponent g(.) ***\n";
	cout << format("g({}, {}) = {}\n",
		1.0, 1.0, g(1.0, 1.0));

	cout << "\n*** C++ mathematical constants math_constant_fcn(.) ***\n";
	cout << format("math_constant_fcn({}, {}) = {}\n",
		1.0, 1.0, math_constant_fcn(1.0, 1.0));

}

double trig_fcn(double theta, double phi)
{
	return std::sin(theta) + std::cos(phi);
}

double zero_coupon_bond(double face_value, double int_rate, double year_fraction)
{
	return face_value * std::exp(-int_rate * year_fraction);
}

// Polynomial using Horner's Method:
double f(double x)
{
	return x * (x * (x * (8.0 * x + 7.0) + 4.0 * x) - 10.0) - 6.0;
}

// Same polynomial, but using brute force method:
double f_pow(double x)				// f(.) in text
{
	return 8.0 * std::pow(x, 4) + 7.0 * std::pow(x, 3) +
		4.0 * std::pow(x, 2) + 10.0 * x - 6.0;
}

double g(double x, double y)
{
	return std::pow(x, -1.368 * x) + 4.19 * y;
}

// C++20 mathematical constants
double math_constant_fcn(double x, double y)
{
	using namespace std::numbers;

	double math_inv_sqrt_two_pi =
		inv_sqrtpi / sqrt2;

	return math_inv_sqrt_two_pi * (std::sin(pi * x) +
		std::cos(inv_pi * y));
}