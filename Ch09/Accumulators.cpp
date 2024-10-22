/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "ExampleDeclarations.h"

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>
#include <boost/accumulators/statistics/rolling_mean.hpp>
#include <boost/accumulators/statistics/rolling_variance.hpp>

//#include <boost/accumulators/statistics.hpp>	// Contrary to "The Boost C++ Libraries"
												// this does not appear to suffice.

#include <Eigen/Dense>

#include <vector>
#include <cmath>
#include <random>
#include <exception>
#include <cstdlib>		// std::size_t

#include <format>
#include <iostream>
#include <iomanip>
using std::format;
using std::cout;
using std::size_t;

namespace bacc = boost::accumulators;

// Top level calling function
void accumulator_examples()
{
	min_max_accumulator();
	mean_and_var_accumulator();
	intro_rolling_windows();
	bollinger_bands(1.5, 25, 5);
	ma_cross(5, 10, 25);
}

void min_max_accumulator()
{
	// No compiler warnings
	cout << "*** min_max_accumulator() ***" << "\n";	

	bacc::accumulator_set<double, bacc::stats<bacc::tag::min, bacc::tag::max>> acc{};

	// push in some data . . .
	acc(5.8);
	acc(-1.7);
	acc(2.9);
	// Display the range
	cout << '(' << bacc::extract::min(acc) << ", " << bacc::extract::max(acc) << ")\n";
	cout << "\n";

	// Now, add an element to acc (ie, we are modifying its *state*),
	// and check that the min and max statistics also get updated:
	acc(524.0);

	// Display the range
	cout << "Updated: (" << bacc::extract::min(acc) << ", " << bacc::extract::max(acc) << ")\n";
	cout << "Type: " << typeid(bacc::extract::min(acc)).name() << "\n";
	cout << "\n\n";
}

void mean_and_var_accumulator()
{
	cout << "*** mean_and_var_accumulator() ***" << "\n";

	bacc::accumulator_set<double, bacc::stats<bacc::tag::mean, bacc::tag::variance>> mv_acc{};
	// push in some data . . .
	mv_acc(1.0);
	mv_acc(2.0);
	mv_acc(3.0);

	// Display the results:
	cout << "(" << bacc::extract::mean(mv_acc) << ", " << bacc::extract::variance(mv_acc) << ")\n";

	mv_acc(4.0);
	mv_acc(5.0);

	cout << "(" << bacc::extract::mean(mv_acc) << ", " << bacc::extract::variance(mv_acc) << ")\n";

	mv_acc(16.0);
	mv_acc(17.0);
	mv_acc(18.0);

	cout << bacc::extract::mean(mv_acc) << ", " << bacc::extract::variance(mv_acc) << "\n\n";
}

void intro_rolling_windows()
{
	cout << "\n*** intro_rolling_windows() ***\n";
	cout << "\nNote that rolling_variance is based on the sample variance:\n ";

	// Note:  `rolling_variance` returns the *sample variance* (divide by n - 1),
	// while the `variance` accumulator in the previous example returns
	// the *population variance* (divide by n).
	bacc::accumulator_set<double, bacc::stats<bacc::tag::rolling_mean,
		bacc::tag::rolling_variance>> roll_acc{ bacc::tag::rolling_window::window_size = 5 };

	roll_acc(1.0);
	roll_acc(2.0);
	roll_acc(3.0);
	
	// These results are the mean and sample variance of the three elements
	// (we haven't yet reached the full five in the rolling window):
	cout << bacc::extract::rolling_mean(roll_acc) << ", " 
		<< bacc::extract::rolling_variance(roll_acc) << "\n\n";

	roll_acc(4.0);
	roll_acc(5.0);

	// We now get the rolling mean and sample variance of all five values:
	cout << bacc::extract::rolling_mean(roll_acc) << ", " 
		<< bacc::extract::rolling_variance(roll_acc) << "\n\n";

	roll_acc(16.0);
	roll_acc(17.0);
	roll_acc(18.0);

	// Finally, we get the rolling mean and sample variance of the last five values:
	cout <<  bacc::extract::rolling_mean(roll_acc) << ", " 
		<< bacc::extract::rolling_variance(roll_acc) << "\n\n";
}

// n = num of std dev's, obs = num observations, win_size = num obs in rolling window
void bollinger_bands(double n, unsigned obs, unsigned win_size)
{
	cout << "*** bollinger_bands(.) ***\n";

	using std::vector, Eigen::MatrixXd;

	vector<double> prices
	{
		100.00, 103.49, 102.82, 106.86, 104.91,
		107.38, 107.46, 111.01, 112.01, 114.11,
		116.91, 121.74, 120.04, 120.24, 120.12,
		120.61, 121.31, 119.25, 118.11, 120.36,
		117.36, 119.12, 119.36, 123.54, 123.42
	};
		
	bacc::accumulator_set<double, bacc::stats<bacc::tag::rolling_mean, bacc::tag::rolling_variance>>
		prices_acc(bacc::tag::rolling_window::window_size = win_size);

	MatrixXd indicators{ prices.size(), 4 };
	
	for (size_t rec = 0; rec < prices.size(); ++rec)
	{
		// Columns of matrix:  price, ma, ma + n*sig, ma - n*sig
		indicators(rec, 0) = prices[rec];
		prices_acc(prices[rec]);
		if (rec >= win_size - 1)
		{
			indicators(rec, 1) = bacc::extract::rolling_mean(prices_acc);
			double dev = n * std::sqrt(bacc::extract::rolling_variance(prices_acc));
			indicators(rec, 2) = indicators(rec, 1) + dev;
			indicators(rec, 3) = indicators(rec, 1) - dev;			
		}
		else
		{
			indicators(rec, 1) = 0.0;
			indicators(rec, 2) = 0.0;
			indicators(rec, 3) = 0.0;
		}
	}

	cout << "Indicator Info (Bollinger Bands): " << "\n";
	cout << std::fixed << std::setprecision(2) << indicators << "\n\n";
}

void ma_cross(unsigned fast_ma_win, unsigned slow_ma_win, unsigned obs)
{
	cout << "*** ma_cross(.) ***" << "\n";

	using std::vector, Eigen::MatrixXd;

	vector<double> prices
	{
		100.00, 103.49, 102.82, 106.86, 104.91,
		107.38, 107.46, 111.01, 112.01, 114.11,
		116.91, 121.74, 120.04, 120.24, 120.12,
		120.61, 121.31, 119.25, 118.11, 120.36,
		117.36, 119.12, 119.36, 123.54, 123.42
	};

	bacc::accumulator_set<double, bacc::stats<bacc::tag::rolling_mean>>
		fast_ma_acc(bacc::tag::rolling_window::window_size = fast_ma_win);

	bacc::accumulator_set<double, bacc::stats<bacc::tag::rolling_mean>>
		slow_ma_acc(bacc::tag::rolling_window::window_size = slow_ma_win);

	MatrixXd indicators{ prices.size(), 3 };	// Price, Short (Fast) MA, Long (Slow) MA
	
	for (size_t rec = 0; rec < prices.size(); ++rec)
	{
		// Columns of matrix:  price, ma, ma + n*sig, ma - n*sig
		indicators(rec, 0) = prices[rec];
		fast_ma_acc(prices[rec]);
		slow_ma_acc(prices[rec]);

		if (rec >= fast_ma_win - 1 && rec >= slow_ma_win - 1)
		{
			indicators(rec, 1) = bacc::extract::rolling_mean(fast_ma_acc);
			indicators(rec, 2) = bacc::extract::rolling_mean(slow_ma_acc);
		}
		else if (rec >= fast_ma_win - 1 && rec < slow_ma_win - 1)
		{
			indicators(rec, 1) = bacc::extract::rolling_mean(fast_ma_acc);
			indicators(rec, 2) = 0.0;
		}
		else
		{
			indicators(rec, 1) = 0.0;
			indicators(rec, 2) = 0.0;
		}
	}

	cout << "Indicator Info (MA Cross): " << "\n";
	cout << indicators << "\n\n";
}