/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "ConceptsExamples.h"

#include <Eigen/Dense>
#include <vector>
#include <string>
#include <iostream>
#include <format>

void concepts_examples()		// Top level calling program
{
	add_stuff();
	square_stuff();
	quadratic_stuff();
}

void add_stuff()
{
	using std::vector;
	using Eigen::VectorXd;
	using std::cout;
	using std::format;

	cout << "\n" << "*** add_stuff() ***" << "\n";

	int sum_ints = add_them(81, 84);			// add_them(.) is defined		
	double sum_reals = add_them(53.15, 72.07);	// in ConceptsExamples.h

	cout << format("Sum of ints = {}, sum of doubles = {}", sum_ints, sum_reals) << "\n\n";

	VectorXd xv(3);
	VectorXd yv(3);

	xv << 1.0, 2.0, 3.0;
	yv << 4.0, 5.0, 6.0;

	VectorXd sum_eig = add_them(xv, yv);

	cout << "Sum of Eigen::Vector Xd objects:\n" << sum_eig << "\n";

	vector<int> u{1, 2, 3};
	vector<int> v{4, 5, 6};
	//vector<int> sum_vec = add_them(u, v);		// Error: + not defined (intentionally
												// included for demonstration - can verify)
	cout << "\n\n";
}

void square_stuff()
{
	using std::vector;
	using std::string;
	using Eigen::MatrixXd;
	using std::cout;

	cout << "\n" << "*** square_stuff() ***" << "\n";
	// Demonstrates using a concept as a template
	// argument in the function template square_it(.).

	MatrixXd xm(2, 2);
	xm << 1.0, 2.0, 2.0, 1.0;

	MatrixXd square_mtx = square_it(xm);	// square_it(.) is defined in
											// ConceptsExamples.h

	cout << "Square of Eigen::MatrixXd:\n" << square_mtx << "\n";
	
	vector<int> v{1, 2, 3};
	//auto square_vec = square_it(v);		// Compiler error (intentionally
											// included for demonstration)

	cout << "\n\n";
}

void quadratic_stuff()	// Using pre-defined Standard Library concepts
{
	using std::string;
	using std::vector;
	using std::cout;
	using std::format;

	cout << "\n" << "*** quadratic_stuff() ***" << "\n";

	//FPQuadratic<int> fp_quad_int(2, 4, 2);		// Will not compile - floating point enforced by concept.
	FPQuadratic<double> fp_quad_real(2.0, 4.0, 2.0);
	double x = 1.0;

	cout << format("Value of floating point quadratic at x = {} is {}", x, fp_quad_real(x)) << "\n";

	// class Quadratic will accept integral and floating point type parameters.
	Quadratic<int> quad_int{2, 4, 2};
	Quadratic<double> quad_real{2.0, 4.0, 2.0};

	int k = 1;
	cout << format("Value of generic quadratic at k = {} is {}", k, quad_int(k)) << "\n";
	cout << format("Value of generic quadratic at x = {} is {}", x, quad_real(x)) << "\n";

	// These will not compile due to Number concept (intentionally
	// included for demonstration):
	//Quadratic<string> quad_string{"this", "won't", "compile"};
	//Quadratic<SimpleClass> quad_simple{SimpleClass{0}, SimpleClass{1}, SimpleClass{2}};

	//vector u{0.0, 1.0};
	//vector v{2.0, 3.0};
	//vector w{4.0, 5.0};

	//Quadratic<vector<double>> quad_vector{u, v, w};

	cout << "\n\n";
}