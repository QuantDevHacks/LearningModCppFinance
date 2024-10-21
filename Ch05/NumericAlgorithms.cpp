/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "StlAlgorithms.h"
#include "UserDefinedTemplates.h"		// print_this(.)

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
#include <functional>	// std::multiplies, std::divides etc
#include <utility>		// std::pair
#include <cmath>

#include <iostream>
#include <format>
#include <iomanip>

void numerical_algorithms()		// Top calling function
{
	iota_examples();
	accumulate_and_inner_product_examples();
	adj_diff_examples();
	partial_sum_examples();
	fold_left_examples();
	vwap_example();
	logs_of_adj_diff_divisions();
	adj_diffs_of_logs();	
}

void iota_examples()
{
	using std::vector, std::cout, std::format;
	cout << "\n*** iota_examples() ***\n\n";

	vector<int> v(6);
	vector<double> w(6);

	std::iota(v.begin(), v.end(), 101);
	std::iota(w.begin(), w.end(), -2.5);

	for (int k : v) {print_this(k);}
	print_this("\n\n");

	for (double x : w) {print_this(x);}
	print_this("\n\n");

	// days in November = 1, 2, . . ., 30
	vector<unsigned> days_in_november(30);
	std::iota(days_in_november.begin(), days_in_november.end(), 1);
	for (int d : days_in_november) {print_this(d);}
	print_this("\n\n");
}

void accumulate_and_inner_product_examples()
{
	using std::vector, std::cout, std::format;
	cout << "\n*** accumulate_and_inner_product_examples() ***\n\n";

	// std::accumulate (use same v and w as generated before with std::iota):
	//vector<int> v{101, 102, 103, 104, 105, 106};
	//vector<double> w{-2.5, -1.5, -0.5, 0.5, 1.5, 2.5};

	vector<int> v(6);
	vector<double> w(6);
	std::iota(v.begin(), v.end(), 101);
	std::iota(w.begin(), w.end(), -2.5);

	int sum_of_ints = std::accumulate(v.begin(), v.end(), 0);		// 621
	double sum_of_reals = std::accumulate(w.begin(), w.end(), 0.0);		// 0

	cout << format("Sum of ints = {}, sum of reals = {}\n\n",
		sum_of_ints, sum_of_reals);

	// The 2nd example will result in bogus result (and possible compiler warning) 
	// if you add 0 (and not 0.0) to double types.
	// If you are curious to see what happens, you can uncomment out the next three lines.

	//cout << "Putting int 0 as 1st term in accumulate sum of `double` types:\n";
	//auto mystery_sum = std::accumulate(w.begin(), w.end(), 0);		// Possibly 2
	//cout << format("Mystery sum = {}\n", mystery_sum);
	// Note:  You may get a compiler *warning* (a good thing), but the code will still run with bogus results.

	// Can also use std::accumulate for products (or with other arithmetic operations):
	double prod_of_reals = std::accumulate(w.begin(), w.end(), 1.0, std::multiplies<double>());
	cout << format("Product of reals = {}\n\n", prod_of_reals);		// -3.515625

	// std::inner_product:
	std::vector<int> u(6);
	std::iota(u.begin(), u.end(), -3);		// -3, -2, . . ., 2
	int dot_prod = std::inner_product(v.begin(), v.end(), u.begin(), 0);	// -293
	cout << format("Dot product of v and u = {}\n", dot_prod);

	// Abstracted for sum of differences:
	vector<double> y(w.size());		// 6 elements
	std::iota(y.begin(), y.end(), 1.5);
	double sum_diffs = std::inner_product(w.begin(), w.end(), y.begin(), 0.0,
		std::plus<double>(), std::minus<double>());		// -24
	cout << format("inner_product abstracted for sum of differences of w and y = {}\n", 
		sum_diffs);	
}

void adj_diff_examples()			
{
	using std::cout, std::vector;
	cout << "\n***adj_diff_examples()***\n";

	std::vector<int> u{100, 101, 103, 106, 110, 115, 121};
	std::deque<int> adj_diffs;

	std::adjacent_difference(u.begin(), u.end(), std::back_inserter(adj_diffs));
	// adj_diffs = 100 1 2 3 4 5 6
	cout << "\nAdjacent differences with original 1st element in deque:\n";
	for (int diff : adj_diffs)
	{
		cout << diff << " ";
	}
	cout << "\n\n";

	adj_diffs.pop_front();
	// adj_diffs now = 1 2 3 4 5 6 (differenced values only)
	cout << "\nAdjacent differences with 1st element removed from front of deque (diff values only):\n";
	for (int diff : adj_diffs)
	{
		cout << diff << " ";
	}
	cout << "\n\n";

	std::deque<double> adj_sums{-2.5, -1.5, -0.5, 0.5, 1.5, 2.5};
	std::adjacent_difference(adj_sums.begin(), adj_sums.end(), adj_sums.begin(), std::plus<double>());
	adj_sums.pop_front();		// -4 -2 0 2 4
	cout << "\nAdjacent sums using std::adjacent_difference algo with std::plus<double>, 1st element removed from front of deque (diff values only):\n";
	for (double sum : adj_sums)
	{
		cout << sum << " ";
	}
	cout << "\n\n";
}

void partial_sum_examples()
{
	using std::cout, std::vector;
	cout << "\n***partial_sum_examples()***\n\n";

	vector<int> z{10, 11, 13, 16, 20, 25, 31};
	vector<int> part_sums;
	part_sums.reserve(z.size());
	std::partial_sum(z.begin(), z.end(), std::back_inserter(part_sums));

	// Result: 10 21 34 50 70 95 126
	for (int ps : part_sums)
	{
		cout << ps << " ";
	}

	cout << "\n\n";

	vector<int> part_prods(z.size());
	//part_prods.reserve(z.size());
	std::partial_sum(z.begin(), z.end(), part_prods.begin(), std::multiplies<int>());
	
	// Result: 10 110 1430 22880 457600 11440000 354640000
	for(int pp : part_prods) {print_this(pp);}
	cout << "\n\n";
}

void fold_left_examples()	// See the NOTE in the chapter following std::partial_sum.
{
	using std::cout, std::format, std::vector;
	cout << "\n***std::accumulate functionality in ranges: std::ranges::fold_left_example()***\n\n";

	vector<int> v(6);
	vector<double> w(6);
	std::iota(v.begin(), v.end(), 101);
	std::iota(w.begin(), w.end(), -2.5);

	// Same vectors as in preceding examples:
	//vector<int> v{101, 102, 103, 104, 105, 106};
	//vector<double> w{-2.5, -1.5, -0.5, 0.5, 1.5, 2.5};

	int sum_of_ints = std::ranges::fold_left(v, 0, std::plus<int>());		// 621
	double sum_of_reals = std::ranges::fold_left(w.begin(), w.end(), 0.0, std::plus<double>());		// 0

	// Can compare if you wish:
	//int sum_of_ints = std::accumulate(v.begin(), v.end(), 0);			// 621
	//double sum_of_reals = std::accumulate(w.begin(), w.end(), 0.0);	// 0

	cout << format("Sum of ints = {}, sum of reals = {}\n",
		sum_of_ints, sum_of_reals);

	double prod_of_reals = std::ranges::fold_left(w, 1.0, std::multiplies<double>());

	// Again, can compare if you wish: 
	//double prod_of_reals = std::accumulate(w.begin(), w.end(), 1.0, std::multiplies<double>());

	cout << format("Product of reals = {}\n\n", prod_of_reals);		// -3.515625
}

void vwap_example()
{
	using std::cout, std::vector;
	cout << "\n***vwap_example()***\n\n";

	vector v{376000, 365000, 344000, 346000, 345000, 336000, 335000, 339000, 340000, 340000, 343000, 367000, 37400};
	vector p{208.59, 206.93, 207.75, 209.21, 208.58, 208.63, 207.92, 208.87, 208.16, 209.49, 208.53, 209.12, 209.05};

	double daily_volume = std::accumulate(v.cbegin(), v.cend(), 0.0);
	double raw_wgt_price = std::inner_product(v.cbegin(), v.cend(), p.cbegin(), 0.0);
	double vwap = raw_wgt_price / daily_volume;

	cout << "Hypothetical Boeing (BA) Daily VWAP = " << vwap << "\n\n";		// 208.48
}

void logs_of_adj_diff_divisions()
{
	using std::cout, std::vector;
	cout << "\n***Calculating log returns: logs_of_adj_diff_divisions()***\n\n";

	vector<double> prices
	{
		25.5, 28.0, 30.5, 31.0, 27.5, 31.0, 29.5, 28.5, 37.5,
		33.5, 25.5, 31.5, 26.5, 29.5, 32.5, 34.5, 28.5, 35.5,
		28.5, 29.0, 32.0, 23.5, 27.5, 33.5, 28.0, 28.0, 32.5,
		31.5, 29.0, 33.0, 32.5, 29.5, 34.5
	};

	std::adjacent_difference(prices.begin(), prices.end(), prices.begin(), std::divides<double>());

	std::deque<double> log_rtns(prices.size());
	std::ranges::transform(prices, log_rtns.begin(), [](double x) {return std::log(x); });

	// Note: The following will not compile.  Instead, wrap in a lambda as shown just above.
	// Explanation is given in the chapter:
	//std::ranges::transform(prices, log_rtns.begin(), std::log);

	cout << "Remove first price -- only want the returns:\n";
	log_rtns.pop_front();
	for (double r : log_rtns)
	{
		cout << std::setprecision(3) << r << " ";
	}

	cout << "\n\n";
}

void adj_diffs_of_logs()
{
	using std::cout, std::vector;
	cout << "\n***Calculating log returns: adj_diffs_of_logs()***\n\n";

	vector<double> prices
	{
		25.5, 28.0, 30.5, 31.0, 27.5, 31.0, 29.5, 28.5, 37.5,
		33.5, 25.5, 31.5, 26.5, 29.5, 32.5, 34.5, 28.5, 35.5,
		28.5, 29.0, 32.0, 23.5, 27.5, 33.5, 28.0, 28.0, 32.5,
		31.5, 29.0, 33.0, 32.5, 29.5, 34.5
	};

	std::ranges::transform(prices, prices.begin(), [](double x) {return std::log(x);});

	// Alternatively, std::transform alone:
	//std::transform(prices.begin(), prices.end(), prices.begin(), std::log<double>);

	std::deque<double> log_rtns;
	std::adjacent_difference(prices.begin(), prices.end(), std::back_inserter(log_rtns));

	for (double r : log_rtns)
	{
		cout << r << " ";
	}

	cout << "\n\n";

	cout << "Remove first price -- only want the returns:\n";
	log_rtns.pop_front();	
	for (double r : log_rtns)
	{
		cout << std::setprecision(3) << r << " ";
	}

	cout << "\n\n";
}


