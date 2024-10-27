// This file is licensed under the Mozilla Public License, v. 2.0.
// You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.

// Examples using user-defined templates in book
#include "Ch04_example_functions.h"
#include "UserDefinedTemplates.h"
#include "Fraction.h"
#include "Shapes.h"
#include "BlackScholes.h"
#include <vector>
#include <memory>
#include <utility>
#include <iostream>
#include <format>

void user_defined_template_examples()
{
	fcn_template_examples();
	class_template_examples();
	default_tmpl_param_example();
}

void fcn_template_examples()
{
	using std::vector, std::cout, std::format;
	cout << "\n*** fcn_template_examples() ***\n";

	cout << "Test Frac *:\n";
	auto prod = Fraction{2, 3} * Fraction{1, 2};
	cout << format("Test prod = {}/{}\n\n", prod.numer(), prod.denom());

	int sq_int = tmpl_square(4);	 			// square(const int&)
	double sq_real = tmpl_square(4.2); 			// square(const double&)

	cout << format("Square of ints = {}, square of reals = {}\n\n",	sq_int, sq_real);

	Fraction frac{2, 3};
	Fraction sq_frac = tmpl_square(frac); 		// square(const Fraction&)
	cout << format("Square of fraction {}/{} is {}/{}\n\n",
		frac.numer(), frac.denom(), sq_frac.numer(), sq_frac.denom());

	auto sq_frac_unif = tmpl_square<Fraction>({2, 3});
	cout << format("Square of fraction using unif initialization = {}/{}\n\n",
		sq_frac_unif.numer(), sq_frac_unif.denom());

	// operator* not defined for Circle.
	// Will not compile.
	Circle circ{1.0};
//	double area = tmpl_square(circ);		// <- Compiler error

	cout << "\n\n";
}

void class_template_examples()
{
	using std::vector, std::cout, std::format;
	cout << "\n*** class_template_examples() ***\n";

	MyPair<int> mp_int{10, 26};
	int min_int = mp_int.get_min(); 			// OK, returns 10

	MyPair<Fraction> mp_frac{{3, 2}, {5, 11}};
	Fraction min_frac = mp_frac.get_min(); 		//  Returns 3/2

	cout << format("Min(10, 26) = {}, Min(3/2, 5/11) = {}/{}\n", min_int, min_frac.numer(), min_frac.denom());

	// Will not compile:
	MyPair<Circle> fail{{1.5}, {3.0}};
//	fail.get_min(); // no operator< for Circle <- Compiler error

	cout << "\n\n";
}

void default_tmpl_param_example()
{
	using std::cout, std::format;

	cout << "\n*** default_tmpl_param_example() ***\n";

	MyPair<> real_pair{19.73, 10.26};
	double min_val = real_pair.get_min();

	cout << format("Min real value ={}\n\n", min_val);
}


 //Deprecated?
//void template_intro_with_vectors()
//{
//	// Templates - introduction
//	using std::vector, std::cout, std::format;
//
//	cout << "\n*** template_intro_with_vectors()  ***\n";
//
//	// A vector of real numbers:
//	vector<double> v{1.0, 2.0, 3.0};
//
//	/*
//	BlackScholes(double strike, double spot, double rate,
//		double time_to_exp, PayoffType pot);
//
//	BlackScholes bsp_otm_tv{strike, spot, time_to_exp, corp, rate};
//
//	*/
//
//	/*
//	// A vector of BlackScholes objects
//	vector<BlackScholes> opts
//	{
//		{100.0, 106.0, 0.5, PayoffType::Call, 0.04},
//		{110.0, 106.0, 0.5, PayoffType::Put, 0.04},
//		{95.0, 106.0, 0.25, PayoffType::Call, 0.032},
//		{105.0, 106.0, 0.02, PayoffType::Put, 0.032},
//		{115.0, 106.0, 1.5, PayoffType::Put, 0.045}
//	};
//
//	cout << "Contents of vector v:\n";
//	for (double x : v)
//	{
//		cout << x << " ";
//	}
//	cout << "\n\n";
//
//	// Values tie with https://goodcalculators.com/black-scholes-calculator/
//	// for verification:
//	cout << "Option values:\n";
//	for (BlackScholes& bsc : opts)
//	{
//		cout << bsc(0.35) << " ";
//	}
//	*/
//	cout << "\n\n";
//}