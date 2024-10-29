/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

export module BlackScholesExamples;
import <iostream>;
import <format>;
import <utility>;		// std::move

import BlackScholesClass;

export void black_scholes_examples()
{
	using std::cout; 
	using OptionValuation::BlackScholes, OptionValuation::PayoffType;

	cout << "\n" << "*** black_scholes_examples() ***" << "\n";

	cout << "Test case 1 - ITM Call option at expiration:" << "\n";

	double strike = 75.0;
	auto corp = PayoffType::Call;	// corp = "call or put"
	double spot = 100.0;
	double rate = 0.05;
	double vol = 0.25;
	double time_to_exp = 0.0;

	// Same as ITM at expiration example in Ch 2:
	BlackScholes bsc_itm_exp{strike, spot, time_to_exp, corp, rate};
	cout << "Call Option price = "
		<< bsc_itm_exp(vol)
		<< "\n";

	// Extra example:
	cout << "\n" << "Test case 2 - ITM Call option with time value:" << "\n";
	time_to_exp = 0.3;
	BlackScholes bsc_itm_tv{strike, spot, time_to_exp, corp, rate};
	cout << "Call Option price = "
		<< bsc_itm_tv(vol)
		<< "\n";

	// Same as OTM put with time remaining example in Ch 2:
	cout << "\n" << "Test case 3 - OTM Put option with time value:" << "\n";
	time_to_exp = 0.25;
	double dividend = 0.075;
	corp = PayoffType::Put;
	BlackScholes bsp_otm_tv{strike, spot, time_to_exp, corp, rate, dividend};
	cout << "Put Option price = "
		<< bsp_otm_tv(vol)
		<< "\n";

	// Extra example:
	cout << "\n" << "Test case 4 - OTM Put option at expiration:" << "\n";
	time_to_exp = 0.0;
	BlackScholes bsp_otm_exp{strike, spot, time_to_exp, corp, rate};
	cout << "Put Option price = "
		<< bsp_otm_exp(vol)
		<< "\n\n";
}

export void implied_volatility_examples()	// Not in book -- provided as an extra
{
	using std::cout;
	using std::format;

	using OptionValuation::BlackScholes, OptionValuation::PayoffType;

	cout << "\n" << "*** implied_volatility_examples() ***" << "\n";

	double strike = 95.0;
	double mkt_opt_price = 6.2;
	auto corp = PayoffType::Call;	// corp = "call or put"
	double spot = 100.0;
	double rate = 0.05;
	double init_vol_guess_1 = 0.15;
	double init_vol_guess_2 = 0.1;
	double tol = 1e-6;
	unsigned max_iter = 1000;

	double time_to_exp = 0.25;

	BlackScholes bsc_itm_tv{strike, spot, time_to_exp, corp, rate};

	//double implied_volatility(BlackScholes bsc, double x0, double x1, double tol, int maxIter)

	double impl_vol = implied_volatility(std::move(bsc_itm_tv), mkt_opt_price, 
		init_vol_guess_1, init_vol_guess_2, tol, max_iter);
	cout << format("Call ITM, time to expiration = {}, implied vol = {} ", time_to_exp, impl_vol) << "\n";
	double opt_val = bsc_itm_tv(impl_vol);
	cout << format("Value of option at implied vol = {}\n", opt_val);

	cout << "\n\n";
}
