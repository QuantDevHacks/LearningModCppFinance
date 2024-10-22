/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "OptionInfo.h"
#include <utility>		// std::move, std::swap
#include <stdexcept>
#include <iostream>
#include <format>

#include <string>

// In what follows, OptionInfo demonstrates how to implement RAII in a 
// modern (C++11/C++14) manner, using a unique pointer member, instead 
// of a raw pointer and "virtual constructor" as seen in the C++03 version above.

OptionInfo::OptionInfo(std::unique_ptr<Payoff> payoff, double time_to_exp) :
	payoff_ptr_{std::move(payoff)}, time_to_exp_{time_to_exp} {}

double OptionInfo::option_payoff(double spot) const
{
	return payoff_ptr_->payoff(spot);
}

double OptionInfo::time_to_expiration() const
{
	return time_to_exp_;
}

void OptionInfo::swap(OptionInfo& rhs) noexcept
{
	using std::swap;
	swap(payoff_ptr_, rhs.payoff_ptr_);
	swap(time_to_exp_, rhs.time_to_exp_);
	//throw std::runtime_error{"Error:..."};	// Compiler warning, runtime error (for demonstration)
}

// Copy Constructor:
OptionInfo::OptionInfo(const OptionInfo& rhs) :
	payoff_ptr_{rhs.payoff_ptr_->clone()}, 
	time_to_exp_{rhs.time_to_expiration()} {}

// Copy Assignment:
OptionInfo& OptionInfo::operator =(const OptionInfo& rhs) noexcept
{
	OptionInfo{rhs}.swap(*this);
	return *this;
}