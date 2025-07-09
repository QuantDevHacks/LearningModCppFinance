/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */ 

#pragma once

#include "ChronoDate.h"
#include "DayCounts.h"

// Yield Curve Abstract Base Class:
class YieldCurve
{
public:
	// d1 <= d2 < infinity:
	double discount_factor(const ChronoDate& d1, const ChronoDate& d2) const;	
	virtual ~YieldCurve() = default;	

	ChronoDate settle_date() const;

protected:	
	YieldCurve(ChronoDate settle_date);	
	Act365 act_365() const;

private:	
	// Every derived class is responsible for setting the value of 
	// settle_date_ and implementing the function yield_curve_(.).	
	virtual double yield_curve_(double t) const = 0;

	ChronoDate settle_;
	inline static Act365 act_365_{};		// The yields are continuously compounded 
											// with Actual/365 day count basis 
};

class LinearInterpYieldCurve final : public YieldCurve
{
public:
	LinearInterpYieldCurve(const ChronoDate& settle_date,
		const std::vector<ChronoDate>& maturity_dates,
		const std::vector<double>& unit_prices);

private:	
	double yield_curve_(double t) const override;

	std::vector<double> maturities_; // maturities in years/year fractions
	std::vector<double> yields_;	
};
