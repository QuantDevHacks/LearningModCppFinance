/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "ExampleDeclarations.h"

#include <random>
#include <algorithm>
#include <ranges>
#include <numeric>
#include <iterator>		// std::back_inserter
#include <functional>	// std::greater
#include <cmath>		// std::sqrt, std::lround 

#include <vector>
#include <array>
#include <iostream>
#include <iomanip>
#include <format>

void engines_and_distributions()
{
	std::cout << "----- ENGINES AND DISTRIBUTIONS -----\n";
	default_random_engine();
	mersenne_twister_and_normal_dist();
	other_distributions();
	shuffle_algo_example();
	max_drawdown_sim();
}


// See https://godbolt.org/z/hEhP68E5T for random values using the 
// gcc/libstdc++ Stl Library release (Compiler Explorer).
// These will be different than what one gets with the MSVC version, but both are
// fully compliant with the C++ Standard.

void default_random_engine()
{
	using std::vector, std::cout;

	cout << "\n*** default_random_engine() ***\n";

	// def is a functor, created with seed 100 and callable with no argument
	std::default_random_engine def{100};

	for (int i = 0; i < 10; ++i)
	{
		// Random integers generated directly from the engine, but
		// without specifying a distribution:
		cout << def() << " ";	// 
	}
	cout << "\n\n";

	// MSVC: 2333906440 2882591512 1195587395 1769725799 1823289175 
	//       2260795471 3628285872 638252938 20267358 673068980
	// gcc:  1680700 330237489 1203733775 1857601685 594259709 
	//		 1923970613 1512819812 1903683451 1996387951 1007791729

	std::vector<double> unifs(10);

	// Use def_2 so that the function will compile (`def` reused in book)
	std::default_random_engine def_2{30}; // seed = 30
	std::uniform_real_distribution<double> unif_rand_dist{0.0, 1.0};
	// Generate 10 uniform variates from [0.0, 1.0) and set
	// as elements in the vector 'unifs':
	for (double& x : unifs)
	{
		x = unif_rand_dist(def_2);
	}
	for (double& x : unifs)
	{
		cout << x << " ";
	}

	// MSVC:  0.916978 0.218257 0.409313 0.643062 0.706414
	//        0.600071 0.927394 0.696651 0.0235988 0.440224

	// gcc:   0.946134 0.759504 0.568776 0.365942 0.0407869
	//	      0.582491 0.0371633 0.891006 0.230946 0.00526713

	cout << "\n\n";
}

void mersenne_twister_and_normal_dist()
{
	using std::cout;
	cout << "\n*** mersenne_twister_and_normal_dist() ***\n";

	std::vector<double> norms(10);
	std::mt19937_64 mt{40};
	std::normal_distribution<> st_norm_rand_dist{};
	// Generate 10 standard normal variates and set as elements in the vector 'norms':
	for (double& x : norms)
	{
		x = st_norm_rand_dist(mt);
	}
	for (double x : norms)
	{
		cout << x << " ";
	}

	cout << "\n\n";

}

void other_distributions() 
{
	using std::cout;
	cout << "\n*** other_distributions() ***\n";

	std::vector<double> t_draws(10);
	std::vector<double> p_draws(10);
	std::mt19937_64 mt{25};

	std::student_t_distribution<> stu{3};
	std::poisson_distribution<> pssn{7.5};

	for (auto& x : t_draws)
	{
		x = stu(mt);
	}

	for (auto& x : p_draws)
	{
		x = pssn(mt);
	}

	cout << "random draws from t(3):\n";
	for (double x : t_draws)
	{
		cout << x << " ";
	}
	cout << "\n\n";

	cout << "random draws from Poisson(7.5):\n";
	for (double x : p_draws)
	{
		cout << x << " ";
	}
	cout << "\n\n";
}

void shuffle_algo_example()
{
	using std::vector, std::cout;
	cout << "\n*** shuffle_algo_example() ***\n";

	// Intro example
	vector<int> v{1, 2, 3};		
	std::mt19937_64 mt{0};

	// Shuffle three times and show results:
	for (unsigned j = 0; j < 3; ++j)
	{
		std::shuffle(v.begin(), v.end(), mt);
		for (int k : v)
		{
			print_this(k);		// In ExampleDeclarations.h (Same as in Ch 4)
		}
		print_this(";  ");
	}
	print_this("\n\n");

	// Ranges version of shuffle:
	std::ranges::shuffle(v, mt);
	
	print_this("\n\n");

}

void max_drawdown_sim()
{
	using std::vector, std::cout, std::format;
	cout << "\n*** max_drawdown_sim() ***\n";

	// Suppose we have a set of realized P/L trade values from a 
	// trading strategy over 10 years (daily bars):

	// This data would in practice come via an interface, not hard-coded:
	vector<double> pnl
	{
		-149'299.30, -673'165.13,    3'891'123.79,  1'061'346.21, -578'464.00,
		-260'855.99,  1'102'167.76,  509'764.96,   -276'786.46,   -11'947.13,
		-277'781.70, -318'603.05,    57'747.06,     151'336.99,    267'826.14,
		-198'132.05, -175'232.84,   -5'973.61,      177'224.51,   -711'878.76,
		-276'786.56,  116'489.15,   -185'188.30,    1'183'810.47,  1'563'147.38,
		-83'632.98,   527'687.09,   -307'650.96,   -321'589.76,   -1'434'711.00,
		 742'743.96, -245'921.36,   -198'131.26,    399'250.02,   -311'632.90,
		 326'569.83,  437'084.67,   -297'694.80,   -379'336.81,   -173'240.27,
		-62'724.74,  -363'407.14,   -142'375.97,   -103'546.19,    187'179.27,
		-161'293.14, -131'423.78,    1'195'759.19,  198'131.95,   -229'991.59,
		-109'519.00, -148'348.69,    1'447'621.95
	};

	for (double pl : pnl)
	{
		cout << std::fixed << std::setprecision(2) << pl << ", ";
	}
	print_this("\n\n");

	// The cumulative P/L from a backtest of the trading strategy 
	// can be found by applying the partial_sum algorithm:
	cout << "Cumulative P/L values from the backtest, using std::partial_sum:\n";
	std::vector<double> cum_pnl;
	cum_pnl.reserve(pnl.size());
	std::partial_sum(pnl.begin(), pnl.end(), std::back_inserter(cum_pnl));

	for (double ps : cum_pnl)
	{
		cout << std::fixed << std::setprecision(2) << ps <<", ";
	}
	print_this("\n\n");

	// A vector of drawdowns is generated, which can
	// be used in backtest analysis.  The max DD can then be easily 
	// obtained from the resulting vector:
	double peak = cum_pnl.front();
	vector<double> drawdowns;
	drawdowns.reserve(cum_pnl.size());

	for (auto pos = cum_pnl.begin() + 1; pos != cum_pnl.end(); ++pos)
	{
		if (peak < *pos )
		{
			peak = *pos;
		}

		drawdowns.push_back(peak - *pos);
		//drawdowns.push_back(1.0 - *pos/peak);		// For percentage form
	}

	// if (high > 0) drawdowns->emplace_back(price / high - 1);

	double max_dd = *std::ranges::max_element(drawdowns);
	
	//double max_dd = 100.0 * (*std::ranges::max_element(drawdowns));		// For percentage form
	//cout << std::fixed << std::setprecision(2) << "Max DD from backtest = "
	//	<< 100 * max_dd << "%, Net P/L = " << cum_pl.back() << "\n";

	// A useful metric that is often found is Net Profit/Max DD:
	double net_pl_over_max_dd = cum_pnl.back() / max_dd;
	
	// Results:
	cout << std::fixed << std::setprecision(2)
		<< "Max DD from backtest = $" << max_dd << ", Net P/L = $" << cum_pnl.back()
		<< ", Net Profit/MaxDD = " << net_pl_over_max_dd << "\n\n";

	// For percentage form:
/*	cout << std::fixed << std::setprecision(2)
		<< "Max DD from backtest = " << max_dd << "%" << "\n\n";*/		// 39.56%


	// In order to get a measure of the risk due to drawdown, we will
	// apply the shuffle algorithm n - 1 times to the pnl vector, 
	// (total of n paths including the original backtest) to obtain 
	// the maximum drawdown on each run, and store each result in a new
	// vector.  This will give us a distribution for the maximum drawdown.
	// Because this process is repeated, it will be easier to 
	// wrap the previous code into a reusable lambda expression, and 
	// return the maximum drawdown.

	auto max_dd_lam = [](const vector<double>& v)
	{
		std::vector<double> cum_pnl;
		cum_pnl.reserve(v.size());
		double max_dd = 0.0;
		std::partial_sum(v.begin(), v.end(), std::back_inserter(cum_pnl));
		double peak = cum_pnl.front();

		for (auto pos = cum_pnl.cbegin() + 1; pos != cum_pnl.cend(); ++pos)
		{
			if (*pos < peak)
			{
				max_dd = std::max(peak - *pos, max_dd);
			}
			else if (peak < *pos)
			{
				peak = *pos;
			}
			else
			{
				continue;
			}

			// else:  continue
		}
		return max_dd;
	};

	// Choose n = 100 for the book example (in practice,
	// this would be determined dynamically at runtime)
	unsigned n = 100;

	// We will store each maximum drawdown from each set of randomly reordered 
	// P/L values in the following vector;
	vector<double> max_drawdowns;
	max_drawdowns.reserve(n);

	// First, append the maximum drawdown from the original backtest order:
	max_drawdowns.push_back(max_dd);

	// Next, define a 64-bit Mersenne Twister random engine to use
	// in the shuffle algorithm:
	std::mt19937_64 mt{10};

	// Then, generate a different random permutation of the pnl vector
	// n-1 times by shuffling the pnl vector and then inputting it into
	// the lambda to get the max DD for each run, and store it 
	// in the drawdowns vector:
	for (unsigned k = 1; k <= n - 1; ++k)
	{
		std::ranges::shuffle(pnl, mt);
		auto max_dd_in_scen = max_dd_lam(pnl);
		max_drawdowns.push_back(max_dd_in_scen);
	}

	// Then, including the max drawdown from the first backtest before shuffling,
	// we have the following distribution of maximum drawdown values:
	print_this("\nMax dd's in each scen:\n");
	auto print_dec_form = [](double x) {cout << std::fixed << std::setprecision(2) << x << " ";};
	std::ranges::for_each(max_drawdowns, print_dec_form);


	cout << "\n\nSmallest max dd = " << *(std::ranges::min_element(max_drawdowns)) 
		<< "  Largest max dd = " << *(std::ranges::max_element(max_drawdowns)) << "\n\n";

	// In Jaekle and Tomasini, the results are fitted to a normal distribution.  
	// Maximum likelihood estimators of the mean and standard deviation are
	// obtained as follows:

	using std::array;

	auto norm_params = [](const vector<double>& v) ->std::array<double, 2>
	{
		double mean = (1.0 / v.size()) * std::accumulate(v.begin(), v.end(), 0.0);

		double sum_sq = 0.0;
		for (double val : v)
		{
			sum_sq += (val - mean) * (val - mean);
		}

		return array<double, 2> {mean, (1.0 / std::sqrt(v.size()))* std::sqrt(sum_sq)};
	};

	//auto params = norm_params(max_drawdowns);
	double mean = norm_params(max_drawdowns)[0];
	double sd = norm_params(max_drawdowns)[1];

	// 95%: 1.64485, 97.5%: 1.96, 99%: 2.33

	const double z_val = 1.64485;
	double upper_conf_intvl_norm = mean + /*norm_params(max_drawdowns)*/ sd * z_val;
	cout << std::fixed << std::setprecision(2)
		<< "Upper confidence level with normal assumption = " << upper_conf_intvl_norm;
	print_this("\n\n");

	// To get a nonparametric confidence level for worst possible maximum
	// drawdown, we need to sort the resulting drawdowns vector
	std::ranges::sort(max_drawdowns);

	// For output in fixed form rounded to two dp:

	cout << "\n\nSorted max Drawdowns:\n\n";
	std::ranges::for_each(max_drawdowns, print_dec_form);
	print_this("\n\n");

	// The (1 - alpha)% upper confidence level cutoff will be approximately 
	// at the (1 - alpha) * drawdowns.size() position. (Check for rounding function)

	double alpha = 0.05;		// Upper 5%-tile (approximately)
	long upper_loc = std::lround(alpha * max_drawdowns.size());		// Rounding function from C++11	
	double max_dd_conf_lev = *(max_drawdowns.end() - upper_loc -1);
	cout << "Worst possible maximum drawdown at 95% confidence = ";
	print_dec_form(max_dd_conf_lev);
	print_this("\n\n");

	// Check for 95.7% and 99% CL's (extras -- not in book):
	alpha = 0.025;		// Upper 2.5%-tile (approximately)
	upper_loc = std::lround(alpha * max_drawdowns.size());		// Rounding function from C++11	
	max_dd_conf_lev = *(max_drawdowns.end() - upper_loc - 1);
	cout << "Worst possible maximum drawdown at 97.5% confidence = ";
	print_dec_form(max_dd_conf_lev);
	print_this("\n\n");

	alpha = 0.01;		// Upper 1%-tile (approximately)
	upper_loc = std::lround(alpha * max_drawdowns.size());		// Rounding function from C++11	
	max_dd_conf_lev = *(max_drawdowns.end() - upper_loc - 1);
	cout << "Worst possible maximum drawdown at 99% confidence = ";
	print_dec_form(max_dd_conf_lev);
	print_this("\n\n");
}