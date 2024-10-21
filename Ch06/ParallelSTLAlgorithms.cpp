/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "ExampleDeclarations.h"
#include "Timer.h"

#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <execution>
#include <utility>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <format>
#include <numbers>		// Might not need this in final version

void parallel_stl_algorithms()
{
	parallel_min_max_mean();
	parallel_dot_product();
	exp_approx(100, 10);
	exp_approx(500, 50);
	exp_approx(500'000, 200);
	exp_approx(1'000'000, 200);
	exp_approx(5'000'000, 200);
	exp_approx(10'000'000, 200);
}

void parallel_min_max_mean()
{
	using std::cout, std::format;
	cout << "\n" << "*** parallel_min_max() ***" << "\n";

	// Generate large number of normal variates:
	std::mt19937_64 mt(25);
	std::normal_distribution<> nd;

	auto next_norm = [&mt, &nd]() -> double
	{
		return nd(mt);
	};
	
	unsigned n = 500;	

	std::vector<double> norms(n);
	std::generate(norms.begin(), norms.end(), next_norm);
	
	// Using STL algorithms with parallel execution policies:

	auto max_norm = std::max_element(std::execution::par, norms.begin(), norms.end());
	auto min_norm = std::min_element(std::execution::par, norms.begin(), norms.end());
	double mean_parallel = 1.0 / norms.size()
		* std::reduce(norms.begin(), norms.end(), 0.0);

	cout << format("max value = {}, min value = {}\n", *max_norm, *min_norm);
	
	cout << "\n\n";
}

void parallel_dot_product()
{
	using std::cout, std::format;
	cout << "\n" << "*** parallel_dot_product() ***" << "\n";

	std::vector<int> v{4, 5, 6};
	std::vector<int> w{1, 2, 3};

	double dot_prod = std::transform_reduce(v.begin(), v.end(), w.begin(), 0);

	cout << format("dot_prod = {}\n", dot_prod);

	// The above is shorthand for:
	dot_prod = std::transform_reduce(v.begin(), v.end(), w.begin(), 0,
		std::plus<int>{}, std::multiplies<int>{});

	// transform_reduce also admits the more general form where other operations 
	// can be used, such as taking the sum of differences between two vectors:
	int sum_diff = std::transform_reduce(v.begin(), v.end(), w.begin(), 0,
		std::plus<int>{}, std::minus<int>{});

	cout << format("sum_diff = {}\n", sum_diff);

	cout << "\n\n";
}

void exp_approx(unsigned num_elements, unsigned n)
{
	// NOTE: Make sure to compile in release (optimized) mode when 
	// running performance tests here.
	using std::cout, std::format;
	cout << format("\n*** exp_approx({}, {}) ***\n", num_elements, n);

	std::mt19937_64 mtre{100};
	std::normal_distribution<long double> nd{};
	std::vector<long double> v(num_elements);

	auto next_norm = [&mtre, &nd](double x)
	{
		return nd(mtre);
	};

	std::transform(v.begin(), v.end(), v.begin(), next_norm);
	double mean_z = (1.0 / v.size()) * std::accumulate(v.cbegin(), v.cend(), 0.0);
	cout << format("Mean of z values in vector = {}\n\n", mean_z);
	auto u = v;

	auto exp_series = [&n](double x)
	{
		// x = A standard normal variate
		double num = x;			// numerator in the term
		double den = 1.0;		// denominator in the term
		double res = 1.0 + x;	// sum of first two terms

		for (unsigned k = 2; k < n; ++k)
		{
			num *= x;
			den *= static_cast<double>(k);
			res += num / den;
		}
		return res;
	};

	cout << "Sanity check: " << "\n";
	cout << "exp(0) = " << exp_series(0.0) << " -- should = approx 1.0" << "\n";
	cout << "exp(1) = " << exp_series(1.0) << " -- should = approx 2.71828" << "\n" << "\n";
	cout << "Number of elements = " << num_elements << "; number of terms = " << n << "\n" << "\n";


	// NOTE:  The user-defined Timer class is included (in Timer.h) 
	// to provide run times for tmr.start()/tmr.stop() blocks.  It returns
	// times in either milliseconds or seconds, based on the member function used.
	// You need not be concerned with its implementation (not covered in the book).
	// It is just provided for the examples in the book, and/or if you wish to
	// use it elsewhere.
	Timer tmr{};

	// Sequential case (no parallel execution policy):	
	tmr.start();
	std::transform(u.begin(), u.end(), u.begin(), exp_series);
	tmr.stop();
	double msec_elapsed = tmr.milliseconds();
	double mean_val = (1.0 / u.size()) * std::accumulate(u.cbegin(), u.cend(), 0.0);

	cout << format("No parallel exec policy: Time Elapsed = {}, mean value = {}\n",
		msec_elapsed, mean_val);

	// Parallel case (WITH parallel execution policy):	
	tmr.start();
	std::transform(std::execution::par, v.begin(), v.end(), v.begin(), exp_series);
	tmr.stop();
	msec_elapsed = tmr.milliseconds();
	mean_val = (1.0 / u.size()) * std::accumulate(u.cbegin(), u.cend(), 0.0);

	cout << format("With parallel exec policy: Time Elapsed = {}, mean value = {}\n",
		msec_elapsed, mean_val);
}