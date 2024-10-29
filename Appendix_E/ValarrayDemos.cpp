/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "ValarrayDemos.h"

#include <valarray>
#include <algorithm>
#include <iostream>
#include <format>

using std::cout;
using std::format;

void val_array_demos()				// Top function called from main()
{
	valarray_operators();	
	valarray_slices();
	valarray_slice_products();
}

void valarray_operators()
{
	cout << "\n*** valarray_operators() ***\n";
	std::valarray<double> v1{ 1.0, 2.0, 3.0,
				  1.5, 2.5 };

	std::valarray<double> v2{ 10.0, -20.0, 30.0,
				 -15.0, 25.0 };


	// CTAD:
	std::valarray vec_sum = 3.0 * v1 + 0.5 * v2;			// vec_sum is also a valarray
	std::valarray scal_div_sum = v1 / 3.0 + 0.5 * v2;		// Can also do scalar division

	cout << format("Type of vec_sum: {}, elements follow:", typeid(vec_sum).name()) << "\n";
	for (auto elem : vec_sum)		// Can use range-based for loop with valarray
	{
		cout << format("{} ",elem);
	}
	cout << "\n\n";

	// Extra example, not in book:
	cout << format("Type of scal_div_sum: {}, elements follow:", typeid(scal_div_sum).name()) << "\n";
	for (auto elem : scal_div_sum)
	{
		print_this(elem);
	}
	cout << "\n\n";

	// Lighten the notation:
	using std::valarray;

	// Element by element product:
	std::valarray prod = v1 * v2;		// CTAD
	cout << format("Type of prod: {}, elements follow:", typeid(prod).name()) << "\n";
	for (auto elem : prod)
	{
		print_this(elem);
	}
	cout << "\n\n";

	// Get dot (inner) product by summing 
	// individual products already computed:
	double dot_prod = prod.sum();
	cout << std::format("Dot (inner) product = {}", dot_prod);
	cout << "\n\n";

	//	auto square = [](double x) -> double {return x * x; };
	double v1_max = v1.max();
	double v1_min = v1.min();

	cout << format("max(v1) = {}, min(v1) = {}", v1_max, v1_min) << "\n" << "\n";

	auto u = v1.apply([](double x) {return x * x; });
	auto w = v1.apply([](double x) {return std::sin(x) + std::cos(x); });

	cout << format("Type of u: {}, elements follow:", typeid(u).name()) << "\n";
	for (auto elem : u)
	{
		print_this(elem);
	}
	cout << "\n\n";

	cout << format("Type of w: {}, elements follow:", typeid(w).name()) << "\n";
	for (auto elem : w)
	{
		print_this(elem);
	}
	cout << "\n\n";

	cout << format("Verify v1 is preserved:") << "\n";
	for (auto elem : v1)
	{
		print_this(elem);
	}
	cout << "\n\n";

	// Mathematical functions for valarray a la cmath:
	valarray sine_val = std::sin(v1);
	valarray log_val = std::log(v1);
	valarray neg_val = -v1;
	valarray abs_val = std::abs(neg_val);
	valarray exp_val = std::exp(neg_val);

	// Output for each of the above is here, but not listed in the book:
	for (auto elem : sine_val)
	{
		cout << std::format("sine(.) = {}\n", elem);
	}
	cout << "\n\n";

	for (auto elem : log_val)
	{
		cout << std::format("log(.) = {}\n", elem);
	}
	cout << "\n\n";

	for (auto elem : neg_val)
	{
		cout << std::format("{} --> {}\n", -elem, elem) << "\n";
	}
	cout << "\n\n";

	for (auto elem : abs_val)
	{
		cout << std::format("|-{}| = {}\n", elem, elem);
	}
	cout << "\n\n";

	for (auto elem : exp_val)
	{
		cout << std::format("exp(.) = {}\n", elem);
	}
	cout << "\n\n";

	// Can also use std::for_each with std::begin(w) and std::end(w)
	// where w is valarray:
	cout << "std::for_each(.) with valarray:\n";
	std::for_each(std::begin(w), std::end(w), print_this<double>);

	// Note: Member function forms, however, are not available; this will not compile:
	// std::for_each(w.begin(), w.end(), print_this<double>);

	cout << "\n\n";
}

void valarray_slices()
{
	cout << "\n*** valarray_slices() ***\n";
	using mtx_array = std::valarray<double>;

	mtx_array val{ 1.0, 2.0, 3.0,
	   			   1.5, 2.5, 3.5,
				   7.0, 8.0, 9.0,
				   7.5, 8.5, 9.5 };

	auto slice_row01 = val[std::slice(0, 3, 1)];		
	mtx_array row01{ slice_row01 };		// Row 1 in the mathematical sense,
										// 0-indexed in C++

	// The output of the code examples is provided here
	// (most is not listed in the book):
	cout << std::format("std::slice return type: {}", typeid(slice_row01).name());
	cout << "\n\n";

	cout << "valarray constructed from slice: " << "\n";
	for (auto elem : row01)
	{
		print_this(elem);
	}
	cout << "\n\n";

	auto slice_col02 = val[std::slice(1, 4, 3)];
	mtx_array col02{ slice_col02 };

	for (auto elem : col02)
	{
		print_this(elem);
	}
	cout << "\n\n";

	cout << "Original valarray elements: " << "\n";
	for (auto elem : val)
	{
		print_this(elem);
	}
	cout << "\n\n";

	cout << "Original valarray after row 1 slice applies -= to itself: " << "\n";
	slice_row01 -= slice_row01;
	for (auto elem : val)
	{
		print_this(elem);
	}
	cout << "\n\n";

	// Can we copy a slice and modify it 
	// without modifying the original (no)?
	cout << "copy_slice += slice_col02" << "\n";
	auto copy_slice = slice_col02;
	copy_slice += slice_col02;
	for (auto elem : val)
	{
		print_this(elem);
	}
	cout << "\n\n";

	// Finally, check what happens to valarray if a 
	// slice is modified?
	cout << "slice_col02 -= slice_col02" << "\n";
	slice_col02 -= slice_col02;
	for (auto elem : val)
	{
		print_this(elem);
	}
	cout << "\n\n";

	//  Note: Compiler error!  Operator not defined for slice objects.
	//	auto sum_slice = slice_row01 + slice_col02;
}

void valarray_slice_products()
{
	cout << "\n*** valarray_slice_products() ***\n";
	using mtx_array = std::valarray<double>;

	mtx_array va01{ 1.0, 2.0, 3.0,
				   1.5, 2.5, 3.5,
				   4.0, 5.0, 6.0,
				   4.5, 5.5, 6.5,
				   7.0, 8.0, 9.0 };
	const int va01_cols = 3;

	mtx_array va02{ 1.0, 2.0, 3.0, 4.0, 5.0,
				   1.5, 2.5, 3.5, 4.5, 5.5,
				   5.0, 6.0, 7.0, 8.0, 8.5 };

	const int va02_rows = 3, va02_cols = 5;


	auto slice_01_row_03 = va01[std::slice(6, va01_cols, 1)];			// rtn type is slice_array
	auto slice_02_col_02 = va02[std::slice(1, va02_rows, va02_cols)];	// rtn type is slice_array

	// Need full valarray<double> (mtx_array types) to perform math operations:
	mtx_array va01_row03 = slice_01_row_03;
	mtx_array va02_col02 = slice_02_col_02;

	// The output of the code examples is provided here
	// (most is not listed in the book):

	for (auto elem : va01_row03)
	{
		print_this(elem);
	}
	cout << "\n\n";

	for (auto elem : va02_col02)
	{
		print_this(elem);
	}
	cout << "\n\n";

	// Element-by-element product:
	auto elem_by_elem = va01_row03 * va02_col02;
	cout << "Element-by-element product:" << "\n";
	for (auto elem : elem_by_elem)
	{
		print_this(elem);
	}
	cout << "\n\n";

	double dot_prod = (va01_row03 * va02_col02).sum();
	cout << std::format("Dot product = {}", dot_prod);
	cout << "\n\n";

	// Alternate approach (implicit conversion to valarray):
	mtx_array va01_row03_alt = va01[std::slice(6, va01_cols, 1)];
	mtx_array va02_col02_alt = va02[std::slice(1, va02_rows, va02_cols)];

	// Element-by-element product:
	mtx_array elem_by_elem_alt = va01_row03_alt * va02_col02_alt;
	cout << "Element-by-element product using alternative (implicit conversion from slice to valarray):" << "\n";
	for (auto elem : elem_by_elem_alt)
	{
		print_this(elem);
	}
	cout << "\n\n";

	dot_prod = (va01_row03_alt * va02_col02_alt).sum();
	cout << std::format("Dot product (alt) = {}", dot_prod);
	cout << "\n\n";
}