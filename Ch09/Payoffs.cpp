#include "Payoffs.h"
#include <algorithm>


// The following implementations (from ch 3) are used to demonstrate 
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