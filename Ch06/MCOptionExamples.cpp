/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "ExampleDeclarations.h"
#include "Payoffs.h"
#include "MCOptionValuation.h"

#include <random>				// To check default seed
#include <memory>
#include <utility>
#include <iostream>
#include <iomanip>
#include <format>

void mc_option_examples()		// Top calling function
{
	euro_no_barrier_examples();
	euro_with_barrier_examples();
}

void euro_no_barrier_examples()
{
	using std::cout, std::format;
	cout << "\n" << "*** euro_no_barrier_examples() ***" << "\n";

	double strike = 75.0;
	double spot = 100.0;
	double vol = 0.25;
	double rate = 0.05;
	double div = 0.075;			// Dividend rate
	double time_to_exp = 0.5;
	int num_time_steps = 12;	
	int num_scenarios = 20'000;
	unsigned seed = 42;

	// This is the example used in the chapter.  
	// BSch Call Option price = 23.52 (BlackScholes class)
	// See https://goodcalculators.com/black-scholes-calculator/
	
	cout << format("ITM call option with time to exp = {}, dividend rate = {}:\n", time_to_exp, div);
	OptionInfo opt_call_itm{std::move(std::make_unique<CallPayoff>(strike)), time_to_exp};
	MCOptionValuation val_call_itm{std::move(opt_call_itm),
		num_time_steps, vol, rate, div};

	double opt_value = val_call_itm.calc_price(spot, num_scenarios, seed);

	cout << std::fixed << std::setprecision(2) << "ITM call option price = " << opt_value << "\n";

	cout << "\n\n";
}

void euro_with_barrier_examples()
{
	using std::cout, std::format;
	cout << "\n" << "*** euro_with_barrier_examples() ***" << "\n";

	double strike = 75.0;
	double spot = 100.0;
	double vol = 0.25;
	double rate = 0.05;
	double div = 0.075;
	double time_to_exp = 0.5;
	int num_time_steps = 12;
	int num_scenarios = 20'000;
	unsigned seed = 42;

	BarrierType barr_type = BarrierType::up_and_out;
	double barr_val = 110.0;		

	// Theoretical price: $5.61 (https://coggit.com/freetools)
		
	cout << format("ITM Call option with time value = {}, dividend rate = {}, up/out barr_val = {}):\n", 
		time_to_exp, div, barr_val);

	OptionInfo opt_call_itm_barr{std::make_unique<CallPayoff>(strike), time_to_exp};
	MCOptionValuation val_call_itm_barr{std::move(opt_call_itm_barr),
		num_time_steps, vol, rate, div, barr_type, barr_val};

	//double val = val_call_itm_barr.calc_price(spot, num_scenarios, seed);
	//cout << "val = " << val << "\n\n";

	cout << "Option Value = " << val_call_itm_barr.calc_price(spot, seed, num_scenarios) << "\n\n";

	// Increase number of scenarios and time steps:
	num_time_steps = 2'400;
	num_scenarios = 50'000;

	OptionInfo opt_call_itm_barr_2{std::make_unique<CallPayoff>(strike), time_to_exp};
	MCOptionValuation val_call_itm_barr_2{std::move(opt_call_itm_barr_2),
		num_time_steps, vol, rate, div, barr_type, barr_val};

	cout << "Option Value = " << val_call_itm_barr_2.calc_price(spot, num_scenarios, seed) << "\n\n";
	cout << "Analytic solution price = 5.61" << "\n\n";


	// Extra example: put option.  This is not in this section of the book,
	// but the example does return in the async tests (task-based concurrency).
	// In this case, it just uses the serial (non-parallelized) code:
	strike = 105.0;
	spot = 100.0;
	vol = 0.25;
	rate = 0.05;
	div = 0.08;		// Dividend
	seed = 42;

	barr_type = BarrierType::down_and_out;
	barr_val = 70.5;

	time_to_exp = 1.0;
	num_time_steps = 24;
	num_scenarios = 20'000;

	cout << format("ITM Put option with time value = {}, dividend rate = {}, up/out barr_val = {}):\n",
		time_to_exp, div, barr_val);

	OptionInfo vo_put_itm_not_exp{std::make_unique<PutPayoff>(strike), time_to_exp};
	MCOptionValuation val_put_itm_not_exp{std::move(vo_put_itm_not_exp), num_time_steps,
		vol, rate, div, barr_type, barr_val};

	double opt_val = val_put_itm_not_exp.calc_price(spot, num_scenarios, seed);
	cout << std::fixed << std::setprecision(2) << "Option Value= " << opt_val << "\n\n";

	// Increase number of scenarios and time steps:
	num_time_steps = 4800;	// '000;
	num_scenarios = 50'000;

	OptionInfo vo_put_itm_not_exp_2{std::make_unique<PutPayoff>(strike), time_to_exp};
	MCOptionValuation val_put_itm_not_exp_2{std::move(vo_put_itm_not_exp_2), num_time_steps,
		vol, rate, div, barr_type, barr_val};

	opt_val = val_put_itm_not_exp_2.calc_price(spot, num_scenarios, seed);
	cout << std::fixed << std::setprecision(2) << "Option Value= " << opt_val << "\n\n";

	cout << "Analytic solution price = 6.29" << "\n\n";
}

