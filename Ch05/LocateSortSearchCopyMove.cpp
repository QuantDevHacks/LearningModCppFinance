/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "StlAlgorithms.h"
#include "UserDefinedTemplates.h"		// print_this(.)
#include "Fraction.h"

#include <vector>
#include <deque>
#include <list>
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

void locate_sort_search_copy_move()		// Top calling function
{
	using std::vector, std::cout, std::format;
	cout << "\n*** algos_with_no_aux_fcn() (top calling fcn) ***\n\n";
	
	count_and_count_if();
	max_min_elems();
	sorting_values();
	searching_containers();
	binary_search_examples();
	copy_and_move();
}

void count_and_count_if()
{
	using std::vector, std::cout, std::format;
	cout << "\n*** count_and_count_if() ***\n\n";

	vector<int> v{1, 2, 3, 4, 2, 5, 2, 6, 7}; // three occurrences of value 2

	// (no auxiliary function)
	auto count_val_2 = std::count(v.begin(), v.end(), 2);	// count_val_2 = 3
	cout << format("Occurences of element 2 with no aux fcn = {}\n", count_val_2);

	// (more general case taking an aux function)
	auto count_if_val_2 = std::count_if(v.begin(), v.end(), [](int n) {return n == 2;});
	cout << format("Occurences of element 2 _with_ aux fcn = {}\n", count_if_val_2);

	// Can also use ranges version:
	count_val_2 = std::ranges::count(v, 2);
	cout << format("Occurences of element 2 with no aux fcn, ranges version = {}\n", count_val_2);

	count_if_val_2 = std::ranges::count_if(v, [](int n) {return n == 2;});
	cout << format("Occurences of element 2 _with_ aux fcn, ranges version = {}\n\n", count_if_val_2);
}

void max_min_elems()
{
	using std::vector, std::cout, std::format;
	cout << "\n*** max_min_elems() ***\n";

	vector<int> v = {6, 7, 3, 5, 4, 1, 2, 9, 8};
	auto max_elem = std::max_element(v.begin(), v.end());
	auto min_elem = std::min_element(v.begin(), v.end());

	cout << std::format("max = {}, min = {}", *max_elem, *min_elem);
	cout << "\n\n";
}

void sorting_values()
{
	using std::vector, std::cout, std::format;
	cout << "\n*** sorting_and_unique_values() ***\n";

	// Pre-C++20 with begin and end iterators:
	cout << "\nSort elements in deque:\n";
	std::deque<int> dq{6, 7, 3, 5, 4, 1, 2, 9, 8};
	std::sort(dq.begin(), dq.end());
	for (int k : dq) print_this(k);
	print_this("\n\n");

	// With ranges:
	cout << "\nSort elements in deque with ranges version:\n";
	std::vector<int> dq_ranges{6, 7, 3, 5, 4, 1, 2, 9, 8};
	std::ranges::sort(dq_ranges);
	for (int k : dq_ranges) print_this(k);
	print_this("\n\n");

	// More generally, sorting can be done according
	// to a condition in an auxiliary function:
	std::deque<int> dq_aux{6, 7, 3, 5, 4, 1, 2, 9, 8};
	std::vector<int> v_ranges_aux{6, 7, 3, 5, 4, 1, 2, 9, 8};

	// sort in _decreasing_ order
	auto gt = [](int a, int b) {return a > b;};

	std::sort(dq_aux.begin(), dq_aux.end(), gt);
	cout << format("With aux fcn condition (deque):\n");
	for (int k : dq_aux) {print_this(k);}
	print_this("\n\n");

	// With ranges:
	cout << format("With aux fcn condition and ranges version (vector):\n");
	std::ranges::sort(v_ranges_aux, gt);
	for (int k : v_ranges_aux) print_this(k);
	print_this("\n\n");
}

void searching_containers()
{
	using std::vector, std::cout, std::format;
	cout << "\n*** searching_containers() ***\n";

	// std::find
	vector<int> ints{747, 377, 707, 757, 727, 787, 777, 717, 247, 737, 767};
	int n = 757;

	auto ipos = std::find(ints.begin(), ints.end(), n);
	if (ipos != ints.end())
	{
		cout << format("Found value {} at index {}\n",
			n, std::distance(ints.begin(), ipos));
	}

	// std::find_if
	vector<double> reals{0.5, 1.6, -2.3, 0.85, -3.2, 2.5, 1.8, -0.72};
	// Look for the first occurrence of a negative real value x in reals:
	auto rpos = std::find_if(reals.begin(), reals.end(), 
		[](double x){return x < 0.0;});

	if (rpos != reals.end())
	{
		std::cout << std::format("First negative value is {}\n", *rpos);
	}	

	ipos = std::ranges::find(ints, n);
	if (ipos != ints.end())
	{
		cout << format("Found value {} at index {} (with std::ranges::find)\n",
			n, std::distance(ints.begin(), ipos));
	}

	// range versions of find and find_if:
	rpos = std::ranges::find_if(reals, [](double x){return x < 0.0;});

	if (rpos != reals.end())
	{
		std::cout << std::format("First negative value is {} (with std::ranges::find_if)\n", *rpos);
	}

	cout << "\n\n";
}

void binary_search_examples()
{
	using std::vector, std::cout, std::format;
	cout << "\n*** binary_search_examples() ***\n";

	vector<int> ints{747, 377, 707, 757, 727, 787, 777, 717, 247, 737, 767};
	int n = 757;

	std::sort(ints.begin(), ints.end());
	if (std::binary_search(ints.begin(), ints.end(), n))
	{
		cout << format("Found value {}\n", n);
	}

	cout << "\n\n";

	if (std::ranges::binary_search(ints, n))
	{
		cout << format("Found value {} using range version\n", n);
	}

	cout << "\n\n";
}

void copy_and_move()
{
	using std::cout, std::format;
	cout << "\n*** copy_and_move() ***\n\n";

	std::vector<int> v{1, 2, 3, 4, 5};
	std::vector<int> w = v; 	// copy constructor (w did not exist before)
	std::transform(w.begin(), w.end(), w.begin(), [](int n) {return -n;});
	for (int n : v) // 1 2 3 4 5
	{
		std::cout << n << ' ';
	}
	std::cout << "\n";
	for (int n : w) // -1 -2 -3 -4 -5
	{
		std::cout << n << ' ';
	}
	std::cout << '\n';
	
	std::vector<int> a;

	// vector a is populated with data...
	a.push_back(6);
	a.push_back(7);
	a.push_back(8);
	a.push_back(9);
	a.push_back(10);

	cout << "\nContents of vector a:\n";
	for (int x : a) print_this(x);
	print_this("\n\n");

	// a is cleared, then makes a copy of v by assignment, so a is now {1, 2, 3, 4, 5}
	a = v;	

	cout << "Contents of vector a after copy assignment\n";
	for (int x : a) print_this(x);
	print_this("\n\n");

	// Can also move same container type with same element type:
	std::vector<int> u = std::move(w);
 
	// This would not compile if u and w were *different* container types.
	// These lines will not compile!
	/*
	std::deque<int> dv_copy = v;				// v is a vector, dv_copy is a deque
	std::list<int> list_v_copy = v;				// v is a vector, list_u_copy is a list
	std::deque<int> dv_move = std::move(v);		// v is a vector, dv_move is a deque
	std::list<int> list_u_move = std::move(u);	// u is a vector, list_u_move is a dv_move
	*/

	// For copying, one can use sequence constructors which take a pair 
	// of iterators as arguments:	
	std::deque<int> dv_copy(std::begin(v), std::end(v));
	std::list<int> list_v_copy(v.begin(), v.end());

	cout << "\nCopy from vector<int> u to deque<int> :\n";
	for (unsigned k : dv_copy)
	{
		print_this(k);
	}
	print_this("\n\n");

	cout << "\nCopy from vector<int> to list<unsigned>:\n";
	for (unsigned k : list_v_copy)
	{
		print_this(k);
	}
	print_this("\n\n");

	// Use the std::move _algorithm_ to move to different container types:

	std::deque<int> dv_move;
	std::list<int> list_u_move;

	cout << "\nMove from vector<int> to deque<int>:\n";
	std::move(v.begin(), v.end(), std::back_inserter(dv_move));
	for (unsigned k : dv_move)
	{
		print_this(k);
	}
	print_this("\n\n");

	cout << "\nMove from deque<unsigned> to list<unsigned>:\n";
	std::ranges::move(u, std::back_inserter(list_u_move));
	for (unsigned k : list_u_move)
	{
		print_this(k);
	}
	print_this("\n\n");

	// Suppose we have a vector of Fraction objects that we want to copy to a deque:
	cout << "\nCopy from vector<Fraction> to deque<Fraction>:\n";
	std::vector<Fraction> vec_fracs{{1, 2}, {5, 8}, {3, 4}, {2, 3}, {1, 9}};
	std::deque<Fraction> dq_fracs(vec_fracs.begin(), vec_fracs.end());

	for (const Fraction& f : dq_fracs)
	{
		cout << format("{}/{}\t", f.numer(), f.denom());
	}
	print_this("\n\n");

	// Suppose we then want to move from a deque to a list:
	cout << "\nMove from deque<Fraction> to list<Fraction>:\n";
	std::list<Fraction> list_fracs;
	std::ranges::move(dq_fracs, std::back_inserter(list_fracs));

	for (const Fraction& f : list_fracs)
	{
		cout << format("{}/{}\t", f.numer(), f.denom());
	}
	print_this("\n\n");
}