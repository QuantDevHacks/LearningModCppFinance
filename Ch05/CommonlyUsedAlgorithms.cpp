/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "StlAlgorithms.h"
#include "UserDefinedTemplates.h"		// print_this(.)

#include <vector>
#include <deque>
#include <list>
#include <array>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <iterator>		// std::back_inserter
#include <functional>	// std::divides
#include <utility>		// std::pair
#include <cmath>

#include <iostream>
#include <format>
#include <iomanip>

// Change to for_each_and_transform_examples()
void commonly_used_algorithms()		// Top calling function
{
	for_each_examples();
	transform_examples_same_container();
	transform_examples_different_container();
	std_begin_and_std_end();	
	class_functor_as_aux_fcn();
	member_fcn_as_aux_fcn();
	algo_examples_in_chptr_summary();
}


void for_each_examples()
{
	using std::vector, std::string, std::cout, std::format;
	cout << "\n*** for_each_examples()  ***\n";

	std::deque<int> q{1, 2, 3, 4, 5, 6, 7, 8, 9};	
	std::vector<string> s{"Fender", "Rickenbacker", "Alembic", "Gibson"};

	std::for_each(q.begin(), q.end(), print_this<int>); cout << "\n";
	std::for_each(s.begin(), s.end(), print_this<string>);
	cout << "\n\n";

	auto prn = [](const auto& x) {print_this(x);};		

	std::for_each(q.begin(), q.end(), prn); cout << "\n";
	std::for_each(s.begin(), s.end(), prn);
	cout << "\n\n";

	// Using ranges:
	std::ranges::for_each(q, prn); cout << "\n";
	std::ranges::for_each(s, prn);
	cout << "\n\n";
}

void transform_examples_same_container()
{
	using std::vector, std::cout, std::format;
	cout << "\n*** transform_examples_same_container()  ***\n\n";

	cout << "Modify and place results in same container:\n";
	std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9};
	auto w = v;			// Backup
	auto prn = [](const auto& x) {print_this(x);};		// As before

	std::transform(v.begin(), v.end(), v.begin(), [](int k) {return tmpl_square(k);});
	std::for_each(v.cbegin(), v.cend(), print_this<int>);
	cout << "\n\n";
	
	// Use w here instead of v as in the chapter, since
	// v gets modified in 1st transform above.
	std::ranges::transform(w, w.begin(), [](int k) {return tmpl_square(k);});
	std::ranges::for_each(w, prn);
	cout << "\n\n";	
}

void transform_examples_different_container()
{
	using std::vector, std::cout, std::format;
	cout << "\n*** transform_examples_different_container()  ***\n\n";

	std::vector<int> v{1, 2, 3, 4, 5, 6, 7, 8, 9};
	std::deque<double> dq;

	cout << "Modify and place results in different container:\n";

	std::transform(v.begin(), v.end(),
		std::back_inserter(dq), [](int n)
		{
			return tmpl_square(n) + 0.5;
		});
	std::for_each(dq.cbegin(), dq.cend(), print_this<double> );
	cout << "\n\n";

	// Use `dq2` in place of ranges version with `dq` in the book,
	// so that the code will compile:
	std::deque<double> dq2;
	std::ranges::transform(v, 
		std::back_inserter(dq2), [](int n) 
		{
			return tmpl_square(n) + 0.5;
		});
	std::ranges::for_each(dq2, print_this<double>);
	cout << "\n\n";
}

void std_begin_and_std_end()
{
	using std::cout, std::format, std::vector;
	cout << "\n*** std_begin_and_std_end()  ***\n";

	int c_array_ints[] = {10, 20, 30, 45, 50, 60};
	vector<int> v_ints_old_way(c_array_ints + 0, c_array_ints + 6);

	cout << "v_ints_old_way copied from c_array:\n";
	for (int k : v_ints_old_way)
	{
		print_this(k);
	}
	print_this("\n\n");

	// we can replace this clunky syntax with a cleaner and more expressive version:
	vector<int> v_ints(std::begin(c_array_ints), std::end(c_array_ints));

	const std::vector<int> v_ints_const(std::cbegin(c_array_ints), std::cend(c_array_ints));

	cout << "v_ints and v_ints_const copied from c_array:\n";
	for (int k : v_ints)
	{
		print_this(k);
	}
	print_this("\n\n");

	// We can also use this form for copying an STL container to another
	std::list list_ints(std::begin(v_ints), std::end(v_ints));
	cout << "STL container list_ints copied from v_ints:\n";
	for (int k : list_ints)
	{
		print_this(k);
	}
	print_this("\n\n");

	std::transform(std::begin(c_array_ints), std::end(c_array_ints),
		std::begin(c_array_ints), [](int k) {return tmpl_square(k); });

	std::transform(std::begin(v_ints), std::end(v_ints),
		std::begin(v_ints), [](int k) {return tmpl_square(k); });

	cout << "c_array_ints and v_ints squared in transform algorithm:\n";
	for (int k : c_array_ints)
	{
		print_this(k);
	}
	print_this("\n\n");

	for (int k : v_ints)
	{
		print_this(k);
	}
	print_this("\n\n");	
}

void class_functor_as_aux_fcn()
{
	using std::vector, std::cout, std::format;
	cout << "\n*** class_functor_as_aux_fcn()  ***\n\n";

	// This function uses the Quadratic class defined in
	// the StlAlgorithms.h header file.

	Quadratic q{2.0, 4.0, 2.0};		// See StlAlgorithms.h

	std::vector<double> v{-1.4, -1.3, -1.2, -1.1, 0.0, 1.1, 1.2, 1.3, 1.4};
	std::deque<double> y;
	std::ranges::transform(v, std::back_inserter(y), q);

	for (double q : y)
	{
		print_this(q);
	}
	cout << "\n\n";
}

void member_fcn_as_aux_fcn()
{
	using std::vector, std::cout, std::format;
	cout << "\n*** member_fcn_as_aux_fcn()  ***\n\n";

	Quadratic q{2.0, 4.0, 2.0};

	std::vector<double> v{-1.4, -1.3, -1.2, -1.1, 0.0, 1.1, 1.2, 1.3, 1.4};
	std::deque<double> y;

	auto quad = [&q](double x) -> double
	{
		return q.value(x);
	};

	std::ranges::transform(v, std::back_inserter(y), quad);

	for (double q : y)
	{
		print_this(q);
	}
	cout << "\n\n";
}