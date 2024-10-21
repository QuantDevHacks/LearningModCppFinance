// This file is licensed under the Mozilla Public License, v. 2.0.
// You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.

#pragma once
#include <array>
#include <map>



enum class PayoffType
{
	Call = 1,
	Put = -1	
};

enum class RiskValues
{
	Delta,
	Gamma,
	Vega,
	Rho,
	Theta
};

class BlackScholes
{
public:
	BlackScholes(double strike, double spot, double time_to_exp, 
		PayoffType payoff_type, double rate, double div = 0.0);

	double operator()(double vol) const;

	// Added to Ch 4 version:
	std::map<RiskValues, double> risk_values(double vol);

private:
	std::array<double, 2> compute_norm_args_(double vol) const;		// d1 and d2;
	double norm_cdf_(double x) const;		// Added to Ch 4 version

	double strike_, spot_, time_to_exp_;
	PayoffType payoff_type_;
	double rate_, div_;
};

double implied_volatility(const BlackScholes& bsc, double opt_mkt_price, double x0, double x1,
	double tol = 1e-6, unsigned max_iter = 1000);