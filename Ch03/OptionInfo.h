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

#pragma once

#include "Payoffs.h"
#include <memory>

// --- OptionInfo03 declaration (C++03 version of RAII) ---
// (The 03 version of C++ (2003) and Chapter 03 are coincidental).
// The class name has been slightly modified so that the code will compile.

// This is similar to the Joshi 2E (2008) example, C++03 version of RAII,
// but we are assuming we can use `default` and `delete` from C++11.

class OptionInfo03		
{
public:
	OptionInfo03(const Payoff03& payoff, double time_to_exp);

	// Strawman: Don't do this!
	OptionInfo03(Payoff03* payoff, double time_to_exp) : payoff_ptr_{payoff}, time_to_exp_ {time_to_exp} {}

	double option_payoff(double spot) const;
	double time_to_expiration() const;
	void swap(OptionInfo03& rhs);

	OptionInfo03(const OptionInfo03& rhs);
	OptionInfo03& operator =(const OptionInfo03& rhs);

	~OptionInfo03();

private:
	Payoff03* payoff_ptr_;
	double time_to_exp_;

	// If we don't need to copy the object, declare 
	// copy operations private (prior to C++11), and
	// comment out the user-defined implementations in OptionInfo.cpp:
	//OptionInfo03(const OptionInfo03& rhs);
	//OptionInfo03& operator =(const OptionInfo03& rhs);

};

// In what follows, OptionInfo demonstrates how to implement RAII in a 
// modern (C++11/C++14) manner, using a unique pointer member, instead 
// of a raw pointer and "virtual constructor" as seen in the C++03 version above.

class OptionInfo
{
public:
	OptionInfo(std::unique_ptr<Payoff> payoff, double time_to_exp);
	double option_payoff(double spot) const;
	double time_to_expiration() const;
	void swap(OptionInfo& rhs) noexcept;

	OptionInfo(const OptionInfo& rhs);
	OptionInfo& operator =(const OptionInfo& rhs) noexcept;

	// Remaining Rule of Five:
	OptionInfo(OptionInfo&& rhs) = default;				// Default move constructor
	OptionInfo& operator =(OptionInfo&& rhs) = default;	// Default move assignment

	~OptionInfo() = default;							// Default destructor

private:
	std::unique_ptr<Payoff> payoff_ptr_;
	double time_to_exp_;
};