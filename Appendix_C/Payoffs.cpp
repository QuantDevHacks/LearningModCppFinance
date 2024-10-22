/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "Payoffs.h"
#include <algorithm>

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