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

#include "Payoffs.h"
#include "Ch03_example_functions.h"
#include <algorithm>

// --- CallPayoff03 and PutPayoff03 implementations (C++03 version of RAII) ---
// // The first Payoff/CallPayoff/PutPayoff versions use an '03' suffix 
// in the sample code to avoid conflict with the modernized versions
// that follow later.  '03' refers to C++03, before unique_ptr(s) 
// and move semantics were part of the C++ Standard.  These use RAII
// in a C++03 context, using "virtual constructors" similar to those 
// described in the Joshi (2008) text.  This "03" and Chapter "03"
// are coincidental.

// --- CallPayoff03 implementation ---
CallPayoff03::CallPayoff03(double strike) :strike_{strike} {}

double CallPayoff03::payoff(double spot) const
{
	return std::max(spot - strike_, 0.0);		// #include <algorithm>
}

CallPayoff03* CallPayoff03::clone() const
{
	return new CallPayoff03(*this);
}


// --- PutPayoff03 implementation ---
PutPayoff03::PutPayoff03(double strike) :strike_{strike} {}

double PutPayoff03::payoff(double spot) const
{
	return std::max(strike_ - spot, 0.0);
}

PutPayoff03* PutPayoff03::clone() const
{
	return new PutPayoff03(*this);
}

// The following implementations are used to demonstrate 
// the modern (C++11/C++14) method of implementing RAII for option payoffs.

// --- CallPayoff implementation ---
CallPayoff::CallPayoff(double strike) :strike_{strike} {}

double CallPayoff::payoff(double spot) const
{
	return std::max(spot - strike_, 0.0);
}

std::unique_ptr<Payoff> CallPayoff::clone() const
{
	return std::make_unique<CallPayoff>(*this);
}


// --- PutPayoff implementation ---
PutPayoff::PutPayoff(double strike) :strike_{strike} {}

double PutPayoff::payoff(double spot) const
{
	return std::max(strike_ - spot, 0.0);
}

std::unique_ptr<Payoff> PutPayoff::clone() const
{
	return std::make_unique<PutPayoff>(*this);
}