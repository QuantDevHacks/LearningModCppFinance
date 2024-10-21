#include "StlAlgorithms.h"

#include <vector>
#include <deque>
#include <array>
#include <set>
#include <algorithm>
#include <ranges>

#include <iostream>
#include <format>

void algo_examples_in_chptr_summary()
{
	using std::cout, std::format;
	cout << "\n*** algo_examples_in_chptr_summary()  ***\n\n";

	std::array<int, 5> a{1, 2, 3, 4, 5};
	std::deque<int> d{1, 2, 3, 4, 5};
	std::set<int> s{1, 2, 3, 4, 5};
	std::vector<int> v(a.size());

	auto square_that = [](int x) {return x * x; };

	std::transform(a.begin(), a.end(), a.begin(), square_that);
	std::ranges::transform(d, d.begin(), square_that);
	std::transform(s.cbegin(), s.cend(), v.begin(), square_that);

	// Results:
	// a = {1, 4, 9, 16, 25}
	// d = {1, 4, 9, 16, 25}
	// s = {1, 2, 3, 4, 5}		(s not modified)
	// v = {1, 4, 9, 16, 25}	(v contains the squares of the elems of s)

	for (int k : a)
	{
		cout << k << " ";
	}
	cout << "\n\n";

	for (int k : d)
	{
		cout << k << " ";
	}
	cout << "\n\n";

	for (int k : s)
	{
		cout << k << " ";
	}
	cout << "\n\n";

	for (int k : v)
	{
		cout << k << " ";
	}
	cout << "\n\n";

	// Results:
	// a = {1, 4, 9, 16, 25}
	// d = {1, 4, 9, 16, 25}
	// s = {1, 2, 3, 4, 5}
	// v = {1, 4, 9, 16, 25}
}