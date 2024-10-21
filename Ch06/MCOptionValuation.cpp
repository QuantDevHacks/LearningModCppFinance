/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "MCOptionValuation.h"
#include "EquityPriceGenerator.h"

#include <utility>			// std::move
#include <cmath>
#include <vector>
#include <algorithm>		// std::find_if
#include <ranges>			// std::ranges::find_if
#include <numeric>			// std::accumulate
#include <random>
#include <future>

// This is the form of the constructor for the European option
// example as shown at the outset of the section in the Chapter.
// It was generalized to include knockout barriers in the 
// (non-commented out) constructor that follows after.
/*
MCOptionValuation::MCOptionValuation(OptionInfo&& opt, int time_steps, double vol,
	double int_rate, double div_rate) : opt_{std::move(opt)}, time_steps_{time_steps},
	vol_{vol}, int_rate_{int_rate}, div_rate_{div_rate} {}
*/

// This is the more general form of the constructor, which allows for barrier options:
MCOptionValuation::MCOptionValuation(OptionInfo&& opt, int time_steps, double vol,
	double int_rate, double div_rate, BarrierType barrier_type, double barrier_value) :
	opt_{std::move(opt)}, time_steps_{time_steps},
	vol_{vol}, int_rate_{int_rate}, div_rate_{div_rate}, 
	barrier_type_{barrier_type}, barrier_value_{barrier_value} {}

// calc_price_euro(.) contains the first iteration of calc_price(.) 
// with steps (1)-(8), as shown in the chapter.
double MCOptionValuation::calc_price_euro(double spot, int num_scenarios, unsigned unif_start_seed)
{
	if (opt_.time_to_expiration() > 0.0)							// (1)
	{
		using std::vector;

		std::mt19937_64 mt_unif{unif_start_seed};
		std::uniform_int_distribution<unsigned> unif_int_dist{};	// (2)

		vector<double> discounted_payoffs;							// (3)
		discounted_payoffs.reserve(num_scenarios);
		const double disc_factor = std::exp(-int_rate_ * opt_.time_to_expiration());

		for (int i = 0; i < num_scenarios; ++i)						// (4)
		{
			EquityPriceGenerator epg{spot, time_steps_, opt_.time_to_expiration(), vol_,
				int_rate_, div_rate_};								// (5)
			vector scenario = epg(unif_int_dist(mt_unif));	// (unif_int_dist(mt_unif): next seed)

			discounted_payoffs.push_back(disc_factor
				* opt_.option_payoff(scenario.back())); 			// (6)
		}

		return (1.0 / num_scenarios) * std::accumulate(discounted_payoffs.cbegin(),
			discounted_payoffs.cend(), 0.0);						// (7)
	}
	else
	{
		return opt_.option_payoff(spot);							// (8)		
	}
}

double MCOptionValuation::calc_price(double spot, int num_scenarios, unsigned unif_start_seed)
{
	bool barrier_hit =
		(barrier_type_ == BarrierType::up_and_out && spot >= barrier_value_) ||
		(barrier_type_ == BarrierType::down_and_out && spot <= barrier_value_);		// (1)

	if (barrier_hit) return 0.0;	// Option is worthless		// (2)

	// Case where barrier has not (yet) been crossed

	if (opt_.time_to_expiration() > 0 )							// (3)
	{		
		std::mt19937_64 mt_unif{unif_start_seed};				// (4)
		std::uniform_int_distribution<unsigned> unif_int_dist{};
		const double disc_factor = std::exp(-int_rate_ * opt_.time_to_expiration());
		
		using std::vector;
		vector<double> discounted_payoffs;						// (5)
		discounted_payoffs.reserve(num_scenarios);

		// Iteration starts with barrier_hit = false
		for (int i = 0; i < num_scenarios; ++i)					// (6)
		{
			EquityPriceGenerator epg{spot, time_steps_, opt_.time_to_expiration(), vol_,
				int_rate_, div_rate_};
			vector scenario = epg(unif_int_dist(mt_unif));		// (7)

			switch (barrier_type_)								// (8)
			{
				case BarrierType::none: break;					// (9)					

				case BarrierType::up_and_out:					// (10)
				{
					auto barrier_hit_pos = std::find_if(scenario.cbegin(), scenario.cend(),
						[this](double sim_eq) {return sim_eq >= barrier_value_;});	// (11)
					if (barrier_hit_pos != scenario.cend()) barrier_hit = true;		// (12)
				}
				break;

				case BarrierType::down_and_out:					// (13)
				{
					auto barrier_hit_pos = std::ranges::find_if(scenario,
						[this](double sim_eq) {return sim_eq <= barrier_value_;});	// (14)
					if (barrier_hit_pos != scenario.cend()) barrier_hit = true;
				}
				break;				
			
			}	// end of switch statement					

			if (barrier_hit)
			{
				discounted_payoffs.push_back(0.0);				// (15)
			}
			else
			{
				discounted_payoffs.push_back(disc_factor * opt_.option_payoff(scenario.back())); // (16)
			}

			barrier_hit = false;								// (17)
		}		

		// Option value = mean of discounted payoffs
		return (1.0 / num_scenarios) * std::accumulate(discounted_payoffs.cbegin(),
			discounted_payoffs.cend(), 0.0);					// (18)		
	}
	else														// (19)
	{
		// At expiration, barrier_hit == false
		return opt_.option_payoff(spot);						// (20)
	}
}


double MCOptionValuation::calc_price_par(double spot, int num_scenarios, unsigned unif_start_seed)
{
	bool barrier_hit =
		(barrier_type_ == BarrierType::up_and_out && spot >= barrier_value_) ||
		(barrier_type_ == BarrierType::down_and_out && spot <= barrier_value_);		

	if (barrier_hit) return 0.0;	// Option is worthless		

	//barrier_hit = false;

	double opt_val = 0.0;
	if (opt_.time_to_expiration() > 0)
	{
		using std::vector;

		std::mt19937_64 mt_unif{unif_start_seed};
		std::uniform_int_distribution<unsigned> unif_int_dist{};

		vector<double> discounted_payoffs;
		discounted_payoffs.reserve(num_scenarios);
		const double disc_factor = std::exp(-int_rate_ * opt_.time_to_expiration());

		vector<std::future<vector<double>>> ftrs;							// (1)
		ftrs.reserve(num_scenarios);

		if (barrier_hit != true)
		{
			for (int i = 0; i < num_scenarios; ++i)
			{
				// Replace with std::async/std::future here:
				EquityPriceGenerator epg{spot, time_steps_, opt_.time_to_expiration(), vol_,
					int_rate_, div_rate_};

				// Each scenario is now asynchronous.
				ftrs.push_back(std::async(epg, unif_int_dist(mt_unif)));	// (2)
				
				// We actually should use std::launch::async launch policy -- 
				// see "Concluding Remarks" in book.  You can comment out (2) and replace 
				// with the following:
				// ftrs.push_back(std::async(std::launch::async, epg, unif_int_dist(mt_unif)));
			}

			for (auto& ftr : ftrs)
			{
				vector scenario = ftr.get();			// (3) (Also note we can use CTAD)

				switch (barrier_type_)
				{
					case BarrierType::none: break;		// do nothing, proceed to appending discounted terminal price
					
					case BarrierType::up_and_out:
					{
						auto barrier_hit_pos = std::find_if(scenario.cbegin(), scenario.cend(),
							[this](double sim_eq) {return sim_eq >= barrier_value_;});
						if (barrier_hit_pos != scenario.cend()) barrier_hit = true;
					}
					break;

					case BarrierType::down_and_out:
					{
						auto barrier_hit_pos = std::ranges::find_if(scenario,
							[this](double sim_eq) {return sim_eq <= barrier_value_;});
						if (barrier_hit_pos != scenario.cend()) barrier_hit = true;
					}
					break;
				}				// end of switch statement	

				if (barrier_hit)
				{
					discounted_payoffs.push_back(0.0);
				}
				else
				{
					discounted_payoffs.push_back(disc_factor * opt_.option_payoff(scenario.back()));
				}

				barrier_hit = false;
			}
		}

		return (1.0 / num_scenarios) * std::accumulate(discounted_payoffs.cbegin(),
			discounted_payoffs.cend(), 0.0);
	}
	else
	{
		return opt_.option_payoff(spot);
	}
}