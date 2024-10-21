/*
	A portion of this sample code is based on sample code provided on CD
	to accompany the book _C++ Design Patterns and Derivatives Pricing (2E)_,
	by Mark Joshi, 2008.

	Its accompanying license agreement is as follows:

	The code is released under an artistic license. This means that you
	can do what you like with it, provided that if you redistribute the
	source code you allow the receiver to do what they like with it too.

	Otherwise, this file is licensed under the Mozilla Public License, v. 2.0.
	You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "OptionInfo.h"
#include "Ch03_example_functions.h"
#include <utility>		// std::move, std::swap
#include <stdexcept>
#include <iostream>
#include <format>

#include <string>

// --- OptionInfo03 implementation (C++03 version of RAII) ---
// The 03 version of C++ (2003) and Chapter 03 are coincidental.
// The class name has been slightly modified so that the code will compile.

OptionInfo03::OptionInfo03(const Payoff03& payoff, double time_to_exp) :
	payoff_ptr_{payoff.clone()}, time_to_exp_{time_to_exp} {}

double OptionInfo03::option_payoff(double spot) const
{
	return payoff_ptr_->payoff(spot);
}

double OptionInfo03::time_to_expiration() const
{
	return time_to_exp_;
}

void OptionInfo03::swap(OptionInfo03& rhs)
{
	using std::swap;
	swap(payoff_ptr_, rhs.payoff_ptr_);
	swap(time_to_exp_, rhs.time_to_exp_);
}

// Comment out with #if 0...#endif if copying not needed,
// and declare the copy operations private in OptionInfo.h (prior to C++11)
//#if 0
OptionInfo03::OptionInfo03(const OptionInfo03& rhs): 
	payoff_ptr_{rhs.payoff_ptr_->clone()}, 
	time_to_exp_{rhs.time_to_expiration()} {}

OptionInfo03& OptionInfo03::operator =(const OptionInfo03& rhs)
{
	OptionInfo03{rhs}.swap(*this);
	return *this;
}
//#endif

OptionInfo03::~OptionInfo03()
{
	delete payoff_ptr_;
}

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