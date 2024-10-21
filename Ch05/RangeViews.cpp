/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "StlAlgorithms.h"
#include "UserDefinedTemplates.h"		// print_this(.)

// DH: Change this file name and top level function name --
// may or may not require an auxiliary function.
// Most examples are `_if` generalizations.

// DH: Not sure if all these are needed, but for now they're included...
#include <vector>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <iostream>
#include <format>
#include <iomanip>

void range_view_examples()
{
	indiv_views();
	chain_views();
	range_based_views();
	views_containers_loops();
}

void indiv_views()
{
	using std::vector, std::cout, std::format;
	cout << "\n*** indiv_views() ***\n\n";

	std::vector<double> w(10);
	std::iota(w.begin(), w.end(), -5.5);
	cout << "Original vector w:\n";
	for (double x : w) 
	{
		print_this(x); 
	}

	print_this("\n\n");

	auto take_five = std::views::take(w, 5);
	auto two_below = std::views::filter(take_five, [](double x) {return x < -2.0;});
	auto squares = std::views::transform(two_below, [](double x) {return x * x;});
	auto drop_two = std::views::drop(squares, 2);
		
	// This is another reason why the auto keyword is so helpful:
	print_this(typeid(take_five).name());
	print_this("\n\n");

	auto print_range = [](auto rng) 
	{
		for (double x : rng) {print_this(x);}
	};

	print_this("In order: take_five, two_below, squares, drop_two:\n");
	print_range(take_five);
	print_this("\n");
	print_range(two_below);
	print_this("\n");
	print_range(squares);
	print_this("\n");
	print_range(drop_two);
	print_this("\n\n");
}

void chain_views()
{
	using std::vector, std::cout, std::format;
	cout << "\n*** chain_views() ***\n\n";

	// Start with the same data as before:
	std::vector<double> w(10);
	std::iota(w.begin(), w.end(), -5.5);
	cout << "Original vector w:\n";
	for (double x : w) {print_this(x);}
	print_this("\n\n");

	// We can now chain the views in a functional programming way:
	auto drop_two = w | std::views::take(5)
		| std::views::filter([](double x) {return x < -2.0;})
		| std::views::transform([](double x) {return x * x;})
		| std::views::drop(2);
	
	auto print_range = [](auto rng) 
	{
		for (double x : rng) {print_this(x);}
	};

	print_range(drop_two);
	print_this("\n\n");
}

void range_based_views()
{
	using std::vector, std::cout, std::format;
	cout << "\n*** range_based_views() ***\n\n";

	// Start with the same data as before:
	std::vector<double> w(10);
	std::iota(w.begin(), w.end(), -5.5);

	auto take_five = std::views::take(w, 5);

	std::vector<double> u;
	u.reserve(take_five.size());	// Note there is a size() member function

	for (auto x : take_five)
	{
		u.push_back(x);
	}

	for (auto x : w | std::views::take(5))
	{
		print_this(x);
	}
	print_this("\n\n");

	for (auto x : w | std::views::take(5)
		| std::views::transform([](double x) {return x * x;}))
	{
		print_this(x);
	}
	print_this("\n\n");
}

void views_containers_loops()
{
	using std::vector, std::cout, std::format;
	cout << "\n*** views_containers_loops() ***\n\n";

	// Start with the same data as before:
	std::vector<double> w(10);
	std::iota(w.begin(), w.end(), -5.5);

	auto take_five = std::views::take(w, 5);

	std::vector<double> u;
	u.reserve(take_five.size()); // Note there is a size() member function
	for (auto x : take_five)
	{
		u.push_back(x);
	}

	for (auto x : u)
	{
		print_this(x); 
	}

	cout << "\n\n";

	for (auto x : w | std::views::take(5))
	{
		print_this(x); // -5.5 -4.5 -3.5 -2.5 -1.5
	}

	cout << "\n\n";

	for (auto x : w | std::views::take(5)
		| std::views::transform([](double x) {return x * x; }))
	{
		print_this(x); // 30.25 20.25 12.25 6.25 2.25
	}

	cout << "\n\n";
}