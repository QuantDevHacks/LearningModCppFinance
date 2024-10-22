#pragma once

#include <boost/multi_array.hpp>
#include "OptionInfo.h"

enum class OptType
{
	Euro,
	American
};

enum class KnockoutType
{
	None,
	Up,
	Down
};

struct Node
{
	double underlying;
	double payoff;
};

class BinomialLatticePricer
{
public:
	BinomialLatticePricer(OptionInfo opt,
		double vol, double int_rate, int time_points,
		double div_rate = 0.0);
	
	double calc_price(double spot, OptType opt_type);

	// Convenience function to display the projected
	// price and payoff at each node:
	void display_lattice_nodes() const;

private:
	OptionInfo opt_;
	int time_points_;
	double div_rate_;

	// Will be calculated and reassigned in the constructor::
	double u_{0.0}, d_{0.0}, p_{0.0};	// up and down factors, and probability of up move
	double disc_fctr_;		// Discount factor	

	boost::multi_array<Node, 2> grid_;

	void project_underlying_prices_(double spot);
	double calculate_node_payoffs_(OptType opt_type);

	// Helper functions called from calculate_discounted_expected_payoffs_(.):
	double disc_expected_val_(int i, int j) const;
	void american_payoffs_();
	void european_payoffs_();
};