/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "OptionInfo.h"

enum class BarrierType
{
	none,
	up_and_out,
	down_and_out	
};

class MCOptionValuation
{
public:
	// This is the form of the constructor for the European option
	// example as shown at the outset of the section in the Chapter.
	// `opt` holds the time to expiration:
	/*
	MCOptionValuation(OptionInfo&& opt, int time_steps, double vol, 
		double int_rate, double div_rate = 0.0);	
	*/

	// This is the more general form of the constructor, which allows for barrier options:
	MCOptionValuation(OptionInfo&& opt, int time_steps, double vol, 
		double int_rate, double div_rate = 0.0, BarrierType barrier_type = BarrierType::none,
		double barrier_value = 0.0);

	// calc_price_euro(.) contains the first iteration of calc_price(.) 
	// with steps (1)-(8), as shown in the chapter:
	double calc_price_euro(double spot, int num_scenarios, unsigned unif_start_seed);

	// calc_price(.) is generalized further to accommodate up/down-and-out barriers:
	double calc_price(double spot, int num_scenarios, unsigned unif_start_seed);

	// calc_price_par(.) accommodates up/down-and-out barriers and
	// will generate equity price scenarios in parallel:
	double calc_price_par(double spot, int num_scenarios, unsigned unif_start_seed);

private:
	OptionInfo opt_;
	int time_steps_;
	double vol_, int_rate_, div_rate_;	
	BarrierType barrier_type_;
	double barrier_value_;
};