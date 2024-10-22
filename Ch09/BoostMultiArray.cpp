/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "ExampleDeclarations.h"		// Also includes test function declarations
#include "BinomialLatticePricer.h"		// BinomialLatticePricer class

// Boost exception handling:
#include <boost/exception/exception.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <format>

using std::unique_ptr, std::make_unique;
using std::vector;
using std::cout, std::format;


void multi_array()			// Top level calling function
{
	simple_multi_array();	// Simple 1st example using Boost Multiarray
	amer_itm_put();			// No dividend (default = 0)
	lattice_pricing_convergence();
}

void simple_multi_array()
{
	using std::string, std::format;

	boost::multi_array<string, 2> ma{boost::extents[2][3]};
	ma[0][0] = "Carl";
	ma[0][1] = "Friedrich";
	ma[0][2] = "Gauss";
	ma[1][0] = "John";
	ma[1][1] = "von";
	ma[1][2] = "Neumann";

	cout << format("Number of extents (dimensions) = {}\n", ma.num_dimensions());
	cout << format("Row size = {}, Col size = {}\n\n", ma.shape()[0], ma.shape()[1]);


	for (unsigned i = 0; i < ma.shape()[0]; ++i)		// ma.shape()[0] = 2
	{
		for (unsigned j = 0; j < ma.shape()[1]; ++j)	// ma.shape()[1] = 3
		{
			cout << std::setw(10) << ma[i][j] << "\t";
		}
		cout << "\n";
	}

	cout << "\n\n";
}

void amer_itm_put()		
{
	cout << format("\n*** amer_itm_put() ***") << "\n";
	cout << format("ITM American put option (no dividend):") << "\n\n";

	// Dividend default is 0, but you can put it in if you like (uncomment the last argument):
	double strike = 40.0, rf_rate = 0.06, mkt_vol = 0.2, time_to_exp = 1.0/*, div_rate{0.0}*/;
	int time_steps = 4;
	double spot = 36.0;

	auto put_ptr = make_unique<PutPayoff>(strike);		// put_ptr = "put pointer"
	OptionInfo put{std::move(put_ptr), time_to_exp};

	// `put` contains a unique_ptr -- must move
	BinomialLatticePricer put_pricer{std::move(put), mkt_vol, rf_rate, time_steps};

	double opt_price = put_pricer.calc_price(spot, OptType::American);
	std::cout << std::setprecision(2) << format("strike = {}, spot = {}",
		strike, spot) << "\n\n";

	put_pricer.display_lattice_nodes();

	cout << format("American option price = {}\n\n", opt_price);

	// Compare with European case:
	cout << format("Compare: ITM European put option (no dividend):") << "\n\n";
	opt_price = put_pricer.calc_price(spot, OptType::Euro);
	std::cout << std::setprecision(2) << format("strike = {}, spot = {}",
		strike, spot) << "\n" << "\n";

	put_pricer.display_lattice_nodes();

	cout << format("European option price = {}\n\n", opt_price);
}

void lattice_pricing_convergence()
{
	cout << std::format("\n*** lattice_pricing_convergence() ***") << "\n";
	cout << std::format("Price convergence of an American put option (no dividend):\n\n");
	double strike = 40.0, rf_rate = 0.06, mkt_vol = 0.2, time_to_exp = 1.0;
	//double strike = 40.0, rf_rate{0.06}, mkt_vol{0.2}, time_to_exp{1.0}, div_rate{0.0};
	double spot = 36.0;

	cout << format("American Put Option: strike = {}, rf rate = {}, vol = {}",
		strike, rf_rate, mkt_vol) << "\n";
	cout << format("time to exp (yr) = {}, ann div rate = {}, spot = {}",
		time_to_exp, 0, spot) << "\n\n";

	int start_iter = 44, max_iterations =  5;

	vector<double> avg_values;
	double new_price = 0.0, last_price = 0.0;
	for (int n = start_iter; n <= start_iter + max_iterations; ++n)
	{
		auto pp = make_unique<PutPayoff>(strike);		// pp = "call pointer"
		OptionInfo put(std::move(pp), time_to_exp);

		// call contains a unique_ptr -- must move
		BinomialLatticePricer put_pricer{std::move(put), mkt_vol, rf_rate, n};

		//double val = call_pricer.calc_price(spot, OptType::American);
		new_price = put_pricer.calc_price(spot, OptType::American);
		cout << format("Time Points = {}, Call option price = {}\n",
			n + 1, new_price);

		if (last_price > 0)
		{
			avg_values.push_back((new_price + last_price) / 2.0);
		}

		last_price = new_price;
	}

	cout << "\nAverage values over (n, n + 1):\n";
	cout << std::fixed << std::setprecision(5);
	for (double x : avg_values)
	{
		cout << x << "\n";
	}

	cout << "\n\nOption value = last average value: " << avg_values.back() << "\n\n";
}