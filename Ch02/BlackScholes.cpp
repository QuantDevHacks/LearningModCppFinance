/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "BlackScholes.h"

#include <cmath>
#include <numbers>
#include <algorithm>
#include <iostream>					// This would not go into production code.
#include <format>					// Same for this.
using std::cout, std::format;		// Only for demonstration.

BlackScholes::BlackScholes(double strike, double spot, double time_to_exp, 
	PayoffType payoff_type, double rate, double div) :
	strike_{strike}, spot_{spot}, time_to_exp_{time_to_exp}, 
	payoff_type_{payoff_type}, rate_{rate}, div_{div}
{
	// Optional:
	//cout << "\n" << "BlackScholes user-defined constructor" << "\n";
}

double BlackScholes::operator()(double vol) const
{
	using std::exp;
	// phi, as in the James book:
	const int phi = static_cast<int>(payoff_type_);			// (1)

	//double opt_price = 0.0;
	if (time_to_exp_ > 0.0)							// (2)
	{
		auto norm_args = compute_norm_args_(vol);	// (3)
		double d1 = norm_args[0];
		double d2 = norm_args[1];

		auto norm_cdf = [](double x) -> double		// (4)
		{
			return (1.0 + std::erf(x / std::numbers::sqrt2)) / 2.0;
		};

		double nd_1 = norm_cdf(phi * d1);			// N(d1) (5)
		double nd_2 = norm_cdf(phi * d2);			// N(d2) (5)
		double disc_fctr = exp(-rate_ * time_to_exp_);		// (6)

		return phi * (spot_ * exp(-div_ * time_to_exp_) * nd_1 - disc_fctr * strike_ * nd_2);	// (7)
	}
	else
	{
		return std::max(phi * (spot_ - strike_), 0.0);  // (8) (std::max in <algorithm>)
	}
}

std::array<double, 2> BlackScholes::compute_norm_args_(double vol) const
{
	double numer = log(spot_ / strike_) + (rate_ - div_ + 0.5 * vol * vol) * time_to_exp_;
	double d1 = numer / (vol * sqrt(time_to_exp_));
	double d2 = d1 - vol * sqrt(time_to_exp_);
	return std::array<double, 2>{d1, d2};
}


double implied_volatility(const BlackScholes& bsc, double opt_mkt_price, double x0, double x1,
	double tol, unsigned max_iter)
{
	auto diff = [&bsc, opt_mkt_price](double x) 
	{
		return bsc(x) - opt_mkt_price;
	};

	// x -> vol, y -> BSc opt price - mkt opt price
	double y0 = diff(x0);
	double y1 = diff(x1);

	double impl_vol = 0.0;
	unsigned count_iter = 0;
	for (count_iter = 0; count_iter <= max_iter; ++count_iter)
	{
		if (std::abs(x1 - x0) > tol)
		{
			impl_vol = x1 - (x1 - x0) * y1 / (y1 - y0);

			// Update x1 & x0:
			x0 = x1;
			x1 = impl_vol;
			y0 = y1;
			y1 = diff(x1);
		}
		else
		{
			return x1;
		}
	}

	return std::nan("");		// std::nan(" ") in <cmath>
}


