/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

export module BlackScholesClass;
import <array>;

export namespace OptionValuation
{

	// Instead of exporting the entire namespace, you can 
	// export individual classes, functions, and scoped enumerators
	// (uncomment the `export` keyword for each in this case):
	/*export*/ enum class PayoffType	// Alternatively can be exported 
	{									// individually from within a namespace.
		Call = 1,
		Put = -1
	};

	/*export*/ class BlackScholes		// Same here (individual export, if desired)
	{
	public:
		BlackScholes(double strike, double spot, double time_to_exp,
			PayoffType pot, double rate, double div = 0.0);

		double operator()(double vol);

	private:

		std::array<double, 2> compute_norm_args_(double vol);	// d1 and d2;

		double strike_, spot_, time_to_exp_;
		PayoffType payoff_type_;
		double rate_, div_;
	};

	// Not in book -- provided as an extra:
	/*export*/ double implied_volatility(BlackScholes&& bsc, double opt_mkt_price, double x0, double x1,
		double tol, unsigned max_iter);
}
