/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

export module ModulesExamples;

import SingleFileModule;
import Templates;
import ExportImportBusiness;
import BlackScholesExamples;		// Separate interface and implementation modules

import <vector>;
import <numbers>;
import <string>;
import <iostream>;
import <format>;

// import std;		// C++23, but not ready for prime time

void simple_class();
void vector_fcn_example();
void math_fcn_example();
void template_examples();
void export_import_example();
void separate_interface_and_implementation_modules();

// Top level calling function (exported to Main.cpp):
export void modules_examples()
{
	simple_class();
	
	vector_fcn_example();
	math_fcn_example();
	template_examples();
	export_import_example();
	separate_interface_and_implementation_modules();
}

void simple_class()		// some_fcn() in the book
{
	using std::cout, std::format;

	SimpleClass sc{10};		// Exported from SingleFileExample module
	cout << format("SimpleClass member k_ = {}", sc.get_val()) << "\n\n";
}

void vector_fcn_example()
{
	using std::cout, std::format;

	std::vector<double> v{1.0, 2.0, 3.0};
	//std::vector w = vector_fcn_helper(v);		// Will not compile!

	// Need to call exported function:
	std::vector<double> w = vector_fcn(v);

	for (double x : v)
	{
		cout << x << " ";
	}

	cout << "\n\n";

}

void math_fcn_example()
{
	using std::numbers::pi, std::vector, std::cout;

	vector<double> pie{pi, 2 * pi, 3 * pi, 4 * pi};
	vector<double> v = math_fcn(pie);

	cout << "math_fcn(pie) = \n";
	for (double x : v)
	{
		cout << x << " ";
	}

	cout << "\n\n";

}

void template_examples()
{
	using std::cout;

	int n = factored_polynomial(1, 1, 1, 1);

	cout << "factored_polynomial(1, 1, 1, 1) = \n";
	print_this(n);

	cout << "\n\n";
}

void export_import_example()
{
	Latex latex{2, 10.50};		// exported from ExportImport module
	std::string order_code{"X120264"};
	order(order_code, latex);

	std::cout << "\n\n";
}

void separate_interface_and_implementation_modules()
{
	// To demonstrate using separate interface and implementation modules:
	black_scholes_examples();
	implied_volatility_examples();		// Not in book -- provided as an extra
}
