#include "ExampleDeclarations.h"

#include <vector>
#include <array>
#include <utility>
#include <random>
#include <numeric>			// std::accumulate
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <format>

#include <boost/math/distributions.hpp>

// Boost exception handling:
#include <boost/exception/exception.hpp>
#include <boost/exception/diagnostic_information.hpp>

using std::cout, std::format;

template<typename T>
void print_this(T t)
{
	std::cout << t << " ";
};

// Used for Noncentral t distribution example
template<typename T = double>
class rand_non_central_t_dist
{
public:
	// nu = degrees of freedom
	// delta = non-centrality parameter
	rand_non_central_t_dist(T nu, T delta) :nu_{nu}, delta_{delta} {}

	// Functor similar to <random> in the Standard Library:
	T operator()(std::mt19937_64& mt)
	{
		auto unif = ud_(mt);	// Next in pseudorandom unif(0, 1) sequence
		boost::math::non_central_t_distribution nctd(nu_, delta_);
		return quantile(nctd, unif);
	}

private:
	T nu_, delta_;
	std::mt19937_64 mt_;
	std::uniform_real_distribution<T> ud_{0.0, 1.0};
};

void statistical_distributions()	// Top level calling function
{
	density_and_distribution_fcns();	
	drawdown_revisited();
	non_central_t();
}

void density_and_distribution_fcns()
{
	using boost::math::students_t;
	using boost::math::normal;

	cout << "***** density_and_distribution_fcns() *****" << "\n";
	// See  http://www.boost.org/doc/libs/1_82_0/libs/math/doc/html/math_toolkit/stat_tut/overview/objects.html

	// Construct a standard normal distribution 
	// (mean 0 and variance 1 are default ctor args):
	normal std_normal{};

	// Construct a non-standard normal distribution with mean 0.08 and variance 0.25:
	normal non_std_normal{0.08, 0.25};

	// Construct a students_t distribution with 4 degrees of freedom: 
	students_t stu_t{4};

	// Examples of member functions:
	double mean = std_normal.mean();
	double sd = std_normal.standard_deviation();

	cout << "Mean of standard normal distribution = " << mean << "\n";
	cout << "Standard deviation of standard normal distribution = " << sd << "\n";

	mean = non_std_normal.mean();
	sd = non_std_normal.standard_deviation();
	cout << "Mean of non-standard normal distribution = " << mean << "\n";
	cout << "Standard deviation of non-standard normal distribution = " << sd << "\n";

	double dof = stu_t.degrees_of_freedom();
	cout << "Degrees of freedom for t-distribution = " << dof << "\n\n";

	// PDF and CDF values for the standard normal
	// and Student’s t distributions at x = 0:
	double n_pdf = pdf(std_normal, 0.0);		// 0.3989
	double n_cdf = cdf(std_normal, 0.0);		// 0.5
	double t_pdf = pdf(stu_t, 0.0);				// 0.375
	double t_cdf = cdf(stu_t, 0.0);				// 0.5

	// The lower fifth percentile of the standard normal distribution:
	double five_pctle = quantile(std_normal, 0.05);	// -1.64485

	cout << format("N(0, 1) pdf(0) = {}\n N(0, 1) cdf(0) = {}\n t(4) pdf(0) = {}\n t(4) cdf(0) = {}\n",
		n_pdf, n_cdf, t_pdf, t_cdf);
	cout << format("lower fifth percentile of the standard normal distribution = {}\n\n", five_pctle);

	// Care when computing quantiles at extreme upper limit -- use complement:
	students_t stu_t_100{100};
	//double very_high_quantile = quantile(stu_t_100, 1.0 - 1e-100);		// Will crash program
	double very_high_quantile = quantile(complement(stu_t_100, 1e-100));
	cout << "t-dist((1 - 1e-100) x 100%-tile (using complement) = " << very_high_quantile << "\n\n";

}

void drawdown_revisited()
{
	using std::vector, std::array;
	using boost::math::normal;
	using boost::math::students_t;

	cout << "\n*** drawdown_revisited() ***\n";

	// From Ch 6 (same data):
	vector<double> max_drawdowns
	{
		1745348.76, 1753314.05, 1811060.31, 1819025.50, 1828980.34, 1864824.72, 1872790.60, 1945470.30, 1951446.10, 1954544.56,
		1958408.76, 1983258.00, 2059969.17, 2081873.15, 2161523.09, 2176458.22, 2185418.74, 2192386.49, 2196369.22, 2228343.52, 
		2259050.25, 2275024.54, 2285932.00, 2320940.62, 2320940.63, 2346711.28, 2378568.62, 2392511.81, 2407445.06, 2407446.45, 
		2421385.24, 2473268.96, 2496055.11, 2511104.90, 2514048.68, 2541852.33, 2592631.65, 2609671.92, 2612498.83, 2635514.21, 
		2638433.09, 2690205.91, 2696131.21, 2698284.17, 2703144.29, 2715096.71, 2726999.31, 2731168.85, 2762008.53, 2775829.66, 
		2778883.83, 2794744.74, 2800719.66, 2803706.67, 2862401.25, 2874398.27, 2892317.42, 2956156.44, 2958145.23, 2996863.24, 
		3017838.81, 3082486.39, 3085470.21, 3125295.92, 3177021.15, 3215855.22, 3293628.46, 3318451.61, 3326412.50, 3398051.61, 
		3423107.39, 3431069.86, 3461772.68, 3498659.10, 3511675.10, 3517694.65, 3526652.87, 3534618.15, 3538483.24, 3617251.60, 
		3876004.37, 3881100.39, 3982648.04, 3983648.65, 4003448.50, 4052305.27, 4065245.92, 4081058.35, 4132948.45, 4146771.07, 
		4146966.17, 4184716.71, 4295229.34, 4417633.14, 4443583.81, 4744315.84, 4761240.15, 4777120.20, 5317756.56, 5687134.51
	};
		
	auto norm_params = [](const vector<double>& v) -> array<double, 2>	
	{		
		double mean = (1.0 / v.size())* std::accumulate(v.begin(), v.end(), 0.0);
		 
		double sum_sq = 0.0;		
		for (double val : v)
		{
			sum_sq += (val - mean) * (val - mean);
		}

		return array<double, 2> {mean, (1.0 / std::sqrt(v.size()))* std::sqrt(sum_sq)};
	};

	auto params = norm_params(max_drawdowns);
	double mean = params[0];
	double sd = params[1];

	// Instead of hard-coding as in Ch 6, 
	// use the boost::math::normal quantile function:

	cout << quantile(normal{}, 0.95) << "\n\n";		// 1.64485

	double z_val = 1.64485;
	double upper_conf_intvl_norm = mean + sd * z_val;
	cout << std::fixed << std::setprecision(2);
	cout << "Hard-coded Std Norm 95%-tile = " << z_val
		<< ", Brute Force Max DD upper 95% Conf Lev (Ch 6) = " << upper_conf_intvl_norm << "\n\n";

	// we can use the Boost normal and quantile functions for a Standard Normal distribution:
	normal std_normal{0.0, 1.0};

	double z_95 = quantile(std_normal, 0.95);
	double upper_conf_intvl_norm_95 = mean + sd * z_95;

	double z_97_5 = quantile(std_normal, 0.975);
	double upper_conf_intvl_norm_97_5 = mean + sd * z_97_5;

	double z_99 = quantile(complement(std_normal, 0.01));
	double upper_conf_intvl_norm_99 = mean + sd * z_99;

	cout << std::fixed << std::setprecision(2);
	cout << "Std Norm 95%-tile ~= " << z_95 
		<< ",\nUsing Boost stat dist: Max DD upper 95% Conf Level = " 
		<< upper_conf_intvl_norm_95 << "\n\n";

	cout << "Std Norm 97.5%-tile ~= " << z_97_5
		<< ",\nUsing Boost stat dist: Max DD upper 97.5% Conf Level = " 
		<< upper_conf_intvl_norm_97_5 << "\n\n";

	cout << "Std Norm 95%-tile ~= " << z_99 
		<< ",\nUsing Boost stat dist: Max DD upper 95% Conf Level = " 
		<< upper_conf_intvl_norm_99 << "\n\n";	
}

void non_central_t()
{
	cout << "\n*** non_central_t() ***\n";

	// Similar look and feel as functions in <random>:
	std::mt19937_64 mt{100};
	rand_non_central_t_dist<> st{3.0, -0.05};

	// Wrap in lambda for use in the generate algo:
	auto nc_t_gen = [&mt, &st]()
	{
		return st(mt);
	};

	// Generate random draws and place in a vector:
	std::vector<double> nc_t_vals(20);	// nc = non-central
	std::ranges::generate(nc_t_vals, nc_t_gen);

	cout << std::fixed << std::setprecision(4);
	// View results:
	for (auto s : nc_t_vals)
	{
		print_this(s);	
	}

	cout << "\n\n";
}