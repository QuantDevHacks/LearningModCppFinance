/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "ExampleDeclarations.h"
#include "Timer.h"
#include "Payoffs.h"
#include "MCOptionValuation.h"

#include <memory>
#include <utility>
#include <iostream>
#include <iomanip>
#include <format>

void mc_option_examples_parallel()		// Top calling function
{
	perf_test_results_with_barrier();
	// You can add your own tests if you like...
}

// These are the only ones used in the book:
void perf_test_results_with_barrier()
{
	using std::cout;
	cout << "\n*** perf_test_results_with_barrier() ***\n";
	cout << "--- One year ITM put option ---\n\n";

	cout << "\nOne year ITM put option 12 time steps:\n";
	perf_tests_euro_with_barrier_examples(1.0, 12, 20'000);
	perf_tests_euro_with_barrier_examples(1.0, 12, 50'000);

	cout << "\nOne year ITM put option 120 time steps:\n";
	perf_tests_euro_with_barrier_examples(1.0, 120, 20'000);
	perf_tests_euro_with_barrier_examples(1.0, 120, 50'000);

	cout << "\nOne year ITM put option 360 time steps:\n";
	perf_tests_euro_with_barrier_examples(1.0, 360, 20'000);
	perf_tests_euro_with_barrier_examples(1.0, 360, 50'000);

	cout << "--- 10 year ITM put option ---\n\n";
	cout << "\n10 year ITM put option Annual time steps:\n";
	perf_tests_euro_with_barrier_examples(10.0, 10, 20'000);
	perf_tests_euro_with_barrier_examples(10.0, 10, 50'000);


	cout << "\n10 year ITM put option Quarterly time steps:\n";
	perf_tests_euro_with_barrier_examples(10.0, 10 * 4, 20'000);
	perf_tests_euro_with_barrier_examples(10.0, 10 * 4, 50'000);

	cout << "\n10 year ITM put option Monthly time steps:\n";
	perf_tests_euro_with_barrier_examples(10.0, 10 * 12, 20'000);
	perf_tests_euro_with_barrier_examples(10.0, 10 * 12, 50'000);

	cout << "\n10 year ITM put option Weekly time steps:\n";
	perf_tests_euro_with_barrier_examples(10.0, 10 * 52, 20'000);
	perf_tests_euro_with_barrier_examples(10.0, 10 * 52, 50'000);
	//perf_tests_euro_with_barrier_examples(10.0, 10 * 52, 250'000);

	cout << "\n10 year ITM put option Daily time steps:\n";
	perf_tests_euro_with_barrier_examples(10.0, 10 * 360, 20'000);
	perf_tests_euro_with_barrier_examples(10.0, 10 * 360, 50'000); 
}

void perf_tests_euro_with_barrier_examples(double time_to_exp, int num_time_steps, int num_scenarios)
{
	using std::cout, std::format;
	cout << "\n" << "*** perf_tests_euro_with_barrier_examples() ***" << "\n";
	cout << format("Time to exp = {}, num time steps = {}, num scenarios = {}\n\n",
		time_to_exp, num_time_steps, num_scenarios);

	double strike = 105.0;
	double spot = 100.0;
	double vol = 0.25;		// Volatility
	double rate = 0.05;		// Risk-free interest rate
	double div = 0.08;		// Dividend
	unsigned seed = 42;

	BarrierType barr_type = BarrierType::down_and_out;
	double barr_val = 70.5;		

	OptionInfo vo_put_itm_not_exp{std::move(std::make_unique<PutPayoff>(strike)), time_to_exp};
	MCOptionValuation val_put_itm_not_exp{std::move(vo_put_itm_not_exp), num_time_steps,
		vol, rate, div, barr_type, barr_val};

	print_this("Euro option with barrier sequential start clock\n");
	Timer tmr{};
	tmr.start();
	double opt_val = val_put_itm_not_exp.calc_price(spot, num_scenarios, seed);
	tmr.stop();
	double msec_elapsed = tmr.milliseconds();
	cout << std::fixed << std::setprecision(2) << "Option Value (no async) = " << opt_val << "\n";
	cout << format("Time elapsed (msec) = {}\n", msec_elapsed);

	print_this("Euro option with barrier async start clock\n");
	tmr.start();
	opt_val = val_put_itm_not_exp.calc_price_par(spot, num_scenarios, seed);
	tmr.stop();
	msec_elapsed = tmr.milliseconds();
	cout << std::fixed << std::setprecision(2) << "Option Value (with async) = " << opt_val << "\n";
	cout << format("Time elapsed (msec) = {}\n\n", msec_elapsed);
}