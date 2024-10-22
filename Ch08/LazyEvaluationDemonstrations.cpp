/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "ExampleDeclarations.h"

#include <utility>
#include <vector>
#include <random>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <format>

// Put using statements here for test functions
using std::vector;
using std::cout;
using std::format;

void lazy_evaluation_demonstration()		// Top calling function
{
	cout << "***** lazy_evaluation_demonstration() *****" << "\n";
	naive_vector_addition();	
	sum_by_element();
	lazy_vector_addition();

	cout << "\n\n";
}

// operator + defined for two vector<double> types
std::vector<double> operator +(const std::vector<double>& a,
	const std::vector<double>& b)
{
	assert(a.size() == b.size());
	std::vector<double> result;
	result.reserve(a.size());
	for (size_t i = 0; i < a.size(); ++i)
	{
		result.push_back(a[i] + b[i]);
	}

	return result;
}

void naive_vector_addition()
{
	// Calls operator + (const std::vector<double>& a, const std::vector<double>& b)
	// See definition just above.
	cout << "*** naive_vector_addition() ***" << "\n";
	vector<double> v_01{1.0, 2.0, 3.0};
	vector<double> v_02{1.5, 2.5, 3.5};
	vector<double> v_03{4.0, 5.0, 6.0};
	vector<double> v_04{4.5, 5.5, 6.5};

	auto y = v_01 + v_02 + v_03 + v_04;

	for (double x : y)
	{
		cout << x << " ";
	}

	cout << "\n\n";
}

std::vector<double> sum_four_vectors(const std::vector<double>& a, 
	const std::vector<double>& b, const std::vector<double>& c, 
	const std::vector<double>& d)
{
	assert(a.size() == b.size());
	assert(b.size() == c.size());
	assert(c.size() == d.size());

	std::vector<double> sum(a.size());
	for (size_t i = 0; i < a.size(); ++i)
	{
		sum[i] = a[i] + b[i] + c[i] + d[i];
	}

	return sum;
}

void sum_by_element()
{
	cout << "*** sum_by_element() ***" << "\n";
	vector<double> v_01{1.0, 2.0, 3.0};
	vector<double> v_02{1.5, 2.5, 3.5};
	vector<double> v_03{4.0, 5.0, 6.0};
	vector<double> v_04{4.5, 5.5, 6.5};

	// This function is defined just above
	auto y = sum_four_vectors(v_01, v_02, v_03, v_04);

	for (double x : y)
	{
		cout << x << " ";
	}

	cout << "\n\n";
}

class SumOfFourVectors 
{
public:

	SumOfFourVectors(const std::vector<double>& a, const std::vector<double>& b,
		const std::vector<double>& c, const std::vector<double>& d) :
		a_{a}, b_{b}, c_{c}, d_{d} 
	{ 
		assert(a.size() == b.size());
		assert(b.size() == c.size());
		assert(c.size() == d.size());
	}

	double operator[](size_t i) const
	{
		return a_[i] + b_[i] + c_[i] + d_[i];
	}

private:

	const std::vector<double>& a_, b_, c_, d_;
};

void lazy_vector_addition()
{
	cout << "*** lazy_vector_addition() ***" << "\n";
	vector<double> v_01{1.0, 2.0, 3.0};
	vector<double> v_02{1.5, 2.5, 3.5};
	vector<double> v_03{4.0, 5.0, 6.0};
	vector<double> v_04{4.5, 5.5, 6.5};

	// The sum object is created, but no addition
	// operations take place yet:
	SumOfFourVectors y{v_01, v_02, v_03, v_04};

	// It is only when the [] operator is invoked that any addition 
	// is performed (lazily).  In this case, the sum of the vectors 
	// is calculated element by element.  Depending on what we need
	// the results for, we can use them to form a new vector:
	vector<double> vec_sum{y[0], y[1], y[2]};
	cout << "Lazy sum of all four vectors:\n";
	for (double x : vec_sum)
	{
		cout << x << " ";
	}
	cout << "\n\n";

	// or as an example, use them as arguments for another function:
	// auto w = f(y[0], y[1], y[2]);

	// Alternatively, suppose we only require the sum over the
	// 1st and 3rd elements.  This way, there is no overhead
	// from summing other elements when they're not needed,
	// nor is there overhead from creating a separate vector instance
	// if it is not needed.

	// In this example the cost savings are trivial, but for cases of
	// very large data vectors, they can become meaningful:
	double sum_of_1st_elems = y[0];
	double sum_of_3rd_elems = y[2];
	cout << format("Lazy sums of 1st and 3rd elements only: {}, {}\n",
		sum_of_1st_elems, sum_of_3rd_elems);

	cout << "\n\n";
}