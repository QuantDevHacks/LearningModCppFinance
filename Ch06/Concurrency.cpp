/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "ExampleDeclarations.h"

#include <vector>
#include <future>
#include <algorithm>
#include <numeric>
#include <random>
#include <execution>
#include <utility>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <format>

void concurrency_examples()
{
	squares();
}

void squares()
{
	using std::cout, std::format;
	cout << "\n" << "*** squares() ***" << "\n";

	// Using a lambda here in place of the square_val(.) 
	// function in the book:
	auto square_val = [](int x)
	{
		return x * x;
	};

	int i = 5;
	auto ftr = std::async(square_val, i);
	cout << std::format("Square of {} is {}\n\n", i, ftr.get());

	// Place std::future objects into a vector:
	std::vector<int> x(25);
	std::iota(x.begin(), x.end(), 0);	   // 0, 1, 2, ..., 24
	std::vector<std::future<int>> v;
	
	for (auto k : x)
	{
		v.push_back(std::async(square_val, k));
	}

	std::vector<int> y(v.size());
	std::ranges::transform(v, y.begin(), 
		[](std::future<int>& fut) 
		{
			return fut.get();
		});

	cout << "Results using a `vector` of `future` types (vector<future>):\n";
	for (int k : y)
	{
		print_this(k);
	}
	print_this("\n\n");

}
