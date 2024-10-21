/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <vector>
#include <random>

class EquityPriceGenerator
{
public:		
	EquityPriceGenerator(double spot, int num_time_steps,
		double time_to_expiration, double volatility, double rf_rate, double div_rate);

	std::vector<double> operator()(int seed) const;

private:	
	double spot_;
	int num_time_steps_;
	double time_to_expiration_;
	double volatility_;
	double rf_rate_;		// Continuous risk-free rate
	double div_rate_;		// Continuous dividend rate
	double dt_;				// dt_ = "delta t"
};