/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */ 

#include "YieldCurve.h"
#include <stdexcept>
#include <cstddef>		// std::size_t
#include <cmath>		// std::exp

// Yield Curve Abstract Base Class:
YieldCurve::YieldCurve(ChronoDate settle_date) :settle_{std::move(settle_date) } {}

ChronoDate YieldCurve::settle_date() const 
{ 
	return settle_; 
}

Act365 YieldCurve::act_365() const
{
	return act_365_;
}

double YieldCurve::discount_factor(const ChronoDate& date_01, const ChronoDate& date_02) const
{
	using std::exp;

	if (date_02 < date_01)
	{
		throw std::invalid_argument
			{"YieldCurve::discount_factor(.) invalid inequality: d2 < d1"};
	}

	if (date_01 < settle_date() || date_02 < settle_date())
	{
		throw std::invalid_argument
			{"YieldCurve::discount_factor(.): dates must fall on or after settle date"};
	}

	if (date_01 == date_02)
	{
		return 1.0;		// exp(0.0)
	}

	// P(t1, t2) = exp( -(t2-t1) * f(t1, t2) )
	double t2 = act_365().year_fraction(settle_date(), date_02);
	double y2 = yield_curve_(t2);

	// if d1 == settle_ then P(t1,t2) = P(0,t2) = exp(-t2 * y2 )
	if (date_01 == settle_date())
	{
		return exp(-t2 * y2);
	}

	double t1 = act_365().year_fraction(settle_date(), date_01);
	double y1 = yield_curve_(t1);	 

	// (t2-t1) f(t1,t2) = t2 * y2 - t1 * y1
	return exp(t1 * y1 - t2 * y2);
}


//
// Linearly Interpolated Yield Curve
//

LinearInterpYieldCurve::LinearInterpYieldCurve(const ChronoDate& settle_date,
	const std::vector<ChronoDate>& maturity_dates, 
	const std::vector<double>& unit_prices):YieldCurve{settle_date}
{
	using std::size_t;

	// C.42: If a constructor cannot construct a valid object, throw an exception
	// https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#c42-if-a-constructor-cannot-construct-a-valid-object-throw-an-exception

	if (maturity_dates.size() != unit_prices.size())
		throw std::invalid_argument{
			"LinearInterpYieldCurve: maturity_dates and spot_discount_factors different lengths"};

	if (maturity_dates.front() < this->settle_date())
		throw std::invalid_argument{"LinearInterpYieldCurve: first maturity date before settle date"};

	// Prevent vector memory reallocation -- use reserve(.):
	maturities_.reserve(maturity_dates.size());
	yields_.reserve(maturity_dates.size());

	// Assume maturity dates in are in ascending order	
	for (size_t i = 0; i < maturity_dates.size(); i++)
	{
		double t = act_365().year_fraction(this->settle_date(), maturity_dates[i]);
		maturities_.push_back(t);
		yields_.push_back(-std::log(unit_prices[i]) / t);
	}
}

double LinearInterpYieldCurve::yield_curve_(double t) const
{
	// interp_yield called from discount_factor, so maturities_front() <= t
	using std::size_t;

	if (t >= maturities_.back())
	{
		return yields_.back();
	}

	// We now know maturities_front() <= t < maturities_.back() 
	size_t indx = 0;
	while (maturities_[indx + 1] < t)
	{
		++indx;
	}

	return yields_[indx] + (yields_[indx + 1] - yields_[indx])
		/ (maturities_[indx + 1] - maturities_[indx]) * (t - maturities_[indx]);
}