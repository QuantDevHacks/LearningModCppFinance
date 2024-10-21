// This file is licensed under the Mozilla Public License, v. 2.0.
// You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.

#include "BlackScholes.h"

#include <cmath>
#include <numbers>
#include <algorithm>
#include <iostream>					// This would not go into production code.
#include <format>					// Same for this.
using std::cout, std::format;		// Only for demonstration.

/*

	BlackScholes(double strike, double spot, double time_to_exp,
		PayoffType payoff_type, double rate, double div = 0.0);

*/

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

std::map<RiskValues, double> BlackScholes::risk_values(double vol)
{
	using std::exp, std::sqrt;

	std::map<RiskValues, double> results;
	compute_norm_args_(vol);
	int phi = static_cast<int>(payoff_type_);

	auto norm_args = compute_norm_args_(vol);
	double d1 = norm_args[0];
	double d2 = norm_args[1];

	double nd_1 = norm_cdf_(phi * d1);		// N(d1) 
	double nd_2 = norm_cdf_(phi * d2);		// N(d2) 
	double disc_fctr = exp(-rate_ * time_to_exp_);

	// N'(x): Standard Normal pdf:
	auto norm_pdf = [](double x) -> double
	{
		using namespace std::numbers;
		return (inv_sqrtpi / sqrt2) * exp(-x * x/2.0);
	};

	double delta = phi * exp(-div_ * time_to_exp_) * nd_1;
	double gamma = exp(-div_ * time_to_exp_) * norm_pdf(d1) 
		/ (spot_ * vol * sqrt(time_to_exp_));
	double vega = spot_ * spot_ * gamma * vol * time_to_exp_;
	double rho = phi * time_to_exp_ * strike_ * disc_fctr * nd_2;
	double theta = phi * div_ * spot_ * exp(-div_ * time_to_exp_) * nd_1
		- phi * rate_ * strike_ * exp(-rate_ * time_to_exp_) * nd_2 
		- spot_ * exp(-div_ * time_to_exp_) * norm_pdf(d1) 
		* vol / (2.0 * sqrt(time_to_exp_));

	// DELTA, GAMMA, VEGA, RHO, THETA
	results.insert({RiskValues::Delta, delta});
	results.insert({RiskValues::Gamma, gamma});
	results.insert({RiskValues::Vega, vega});
	results.insert({RiskValues::Rho, rho});
	results.insert({RiskValues::Theta, theta});

	return results;
}

std::array<double, 2> BlackScholes::compute_norm_args_(double vol) const
{
	double numer = log(spot_ / strike_) + (rate_ - div_ + 0.5 * vol * vol) * time_to_exp_;
	double d1 = numer / (vol * sqrt(time_to_exp_));
	double d2 = d1 - vol * sqrt(time_to_exp_);
	return std::array<double, 2>{d1, d2};
}

double BlackScholes::norm_cdf_(double x) const
{
	return (1.0 + std::erf(x / std::numbers::sqrt2)) / 2.0;
}


double implied_volatility(const BlackScholes& bsc, double opt_mkt_price, double x0, double x1,
	double tol, unsigned max_iter)
{
	//cout << "\n" << "*** implied_volatility_with_move(.) ***" << "\n\n";

	auto f = [&bsc, opt_mkt_price](double x) -> double
	{
		return bsc(x) - opt_mkt_price;
	};

	// x -> vol, y -> BSc opt price - mkt opt price
	double y0 = f(x0);
	double y1 = f(x1);

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
			y1 = f(x1);
		}
		else
		{
			return x1;
		}
	}

	return std::nan("");		// std::nan(" ") in <cmath>
}


