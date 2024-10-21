// This file is licensed under the Mozilla Public License, v. 2.0.
// You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.

#pragma once

#include <cmath>

class BankAccount
{
public:
	BankAccount(double init_value, double continuous_rate) :
		init_value_{init_value}, continuous_rate_{continuous_rate} {}

	BankAccount() = default;

	/*
		// To verify push_back(.) will attempt to invoke the copy constructor,
		// set it to delete and note we get a compiler error:
		BankAccount(const BankAccount&) = delete;

		// Similarly, note that element-by-element assignment will
		// attempt to invoke the copy assignment operator:
		BankAccount& operator =(const BankAccount&) = delete;

		// Rule of Five now applies if copy operations are removed -- need to
		// ensure move operations are enabled:
		BankAccount(BankAccount&&) = default;
		BankAccount& operator =(BankAccount&&) = default;
		~BankAccount() = default;

		// By activating this code, you can also verify that emplace_back(.)
		// does not result in object copy.
	*/

	double value(double time) const
	{
		return init_value_ * std::exp(continuous_rate_ * time);
	}

	// Alternatively, for use as a convenient auxiliary function in an
	// STL algorithm, we can define a functor on the class:

	double operator ()(double time) const
	{
		return value(time);
	}

	double init_value() const
	{
		return init_value_;
	}

	double continuous_rate() const
	{
		return continuous_rate_;
	}

private:
	double init_value_{1.0}, continuous_rate_{0.0};

};
