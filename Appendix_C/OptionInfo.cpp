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

// C++11 (and after) version:
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

OptionInfo::OptionInfo(OptionInfo&& rhs) noexcept :
	payoff_ptr_{std::exchange(rhs.payoff_ptr_, nullptr)},
	time_to_exp_{rhs.time_to_exp_} 
{
	std::cout << "\n-OptionInfo move ctor called-\n";
}

OptionInfo& OptionInfo::operator =(OptionInfo&& rhs) noexcept		// Implemented move assignment
{
	OptionInfo{std::move(rhs)}.swap(*this);

	std::cout << "\n-OptionInfo move assignment op called-\n";	
	return *this;
}