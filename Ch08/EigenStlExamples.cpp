#include "ExampleDeclarations.h"

#include <Eigen/Dense>
#include <cmath>
#include <complex>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <ranges>
#include <functional>
#include <random>
#include <vector>
#include <deque>
#include <list>

#include <utility>		// std::move
#include <format>
#include <iostream>
#include <iomanip>

using std::vector;
using std::format;
using std::cout;

using Eigen::MatrixXd;
using Eigen::MatrixXi;		// For testing
using Eigen::VectorXd;

void eigen_stl_examples()
{
	stl_and_eigen();
	rowwise_and_colwise();
	log_rtns_rowwise_colwise();	
	eigen_mtx_unary_expr();
	eigen_math_array_fcns();
}

void stl_and_eigen()
{
	VectorXd u{12};			// 12 elements
	std::mt19937_64 mt{100};	// Mersenne Twister engine, seed = 100
	std::student_t_distribution<> tdist{5};

	// v 3.4:
	std::generate(u.begin(), u.end(), [&mt, &tdist]() {return tdist(mt);});

	// Prior to v 3.4, had to use .data() and .size() member functions:
//	std::generate(u.data()), u.data() + u.size(), [&mt, &tdist]() {return tdist(mt);});

	cout << "t-dist variates in VectorXd:\n" << u << "\n\n";

	// Non-modifying std::max_element algo:
	auto max_u = std::max_element(u.begin(), u.end());		// max_element(.) returns an iterator
	cout << std::format("Max t-dist variate in VectorXd: {}", *max_u) << "\n\n";

	std::vector<double> v(u.size());
	std::generate(v.begin(), v.end(), [&mt, &tdist]() {return tdist(mt);});
	double dot_prod = std::inner_product(u.begin(), u.end(), v.begin(), 0.0);
	cout << std::format("Dot product of VectorXd and std::vector<double>: {}\n", dot_prod);

	// Not in book: Examples including other STL container types to show 
	// we are not limited to std::vector:
	std::deque<double> d(u.size());
	std::generate(d.begin(), d.end(), [&mt, &tdist]() {return tdist(mt);});
	dot_prod = std::inner_product(u.begin(), u.end(), d.begin(), 0.0);
	cout << std::format("Dot product of VectorXd and std::deque<double>: {}\n", dot_prod);

	std::list<double> s(u.size());
	std::generate(s.begin(), s.end(), [&mt, &tdist]() {return tdist(mt);});
	dot_prod = std::inner_product(u.begin(), u.end(), s.begin(), 0.0);
	cout << std::format("Dot product of VectorXd and std::list<double>: {}\n\n", dot_prod);

	// Can also use range versions of algorithms:
	VectorXd w(v.size());
	std::ranges::transform(u, v, w.begin(), std::plus{});
	cout << "Vector sum of u (Eigen::VectorXd) and v (std::vector) using std::transform:\n";
	for (double x : w)
	{
		cout << x << " ";
	}

	cout << "\n\n"; 
}

void rowwise_and_colwise()
{
	cout << format("\n*** rowwise_and_colwise() ***") << "\n";

	// Easy example:
	MatrixXd vals_01
	{
		{9.0, 8.0, 7.0},
		{3.0, 2.0, 1.0},
		{9.5, 8.5, 7.5},
		{3.5, 2.5, 1.5}
	};

	cout << "Original mtx: " << "\n";
	cout << vals_01 << "\n" << "\n";

	cout << "Type returned by rowwise(): " << typeid(vals_01.rowwise()).name() << "\n" << "\n";
	cout << "Type individual row returned from row(i) of a MatrixXd: " << typeid(vals_01.row(0)).name() << "\n" << "\n";

	cout << "Square each element (rowwise) (not shown in book): " << "\n";
	for (auto row : vals_01.rowwise())
	{
		std::ranges::transform(row, row.begin(), [](double x) {return x * x;});
	}
	
	cout << vals_01 << "\n\n";

	cout << "Sort elements in each column (colwise) -- result in book: " << "\n";
	for (auto col : vals_01.colwise())
	{
		std::ranges::sort(col);
	}

	cout << vals_01 << "\n\n";
}

void log_rtns_rowwise_colwise()
{
	cout << format("*** log_rtns_rowwise_colwise() ***") << "\n";

	// Pretend we have prices from three funds:
	MatrixXd prices_to_returns
	{
		{25.5, 8.0, 70.5},
		{31.0, 7.5, 71.0},
		{29.5, 8.5, 77.5},
		{33.5, 5.5, 71.5},
		{26.5, 9.5, 72.5},
		{34.5, 8.5, 75.5},
		{28.5, 9.0, 72.0},
		{23.5, 7.5, 73.5},
		{28.0, 8.0, 72.5},
		{31.5, 9.0, 73.0},
		{32.5, 9.5, 74.5}
	};

	cout << "Original price data mtx: " << "\n";
	cout << prices_to_returns << "\n" << "\n";

	// Take natural log of each price:

	for (auto row : prices_to_returns.rowwise())
	{
		std::ranges::transform(row, row.begin(), [](double x) {return std::log(x);});
	}

	cout << "Logs of prices: " << "\n";
	cout << std::fixed << std::setprecision(6) << std::right << prices_to_returns << "\n\n";

	for (auto col : prices_to_returns.colwise())
	{
		std::adjacent_difference(col.begin(), col.end(), col.begin());
	}

	cout << "Adjacent differences of logs of prices (but 1st row still contains prices): " 
		<< "\n" << prices_to_returns << "\n\n";

	// Lop off first row of prices so we are left with returns only:
	MatrixXd returns_mtx = prices_to_returns(Eigen::seq(1, Eigen::last), Eigen::seq(0, Eigen::last));
	cout << "Adjacent differences with 1st row sliced, type =  " << typeid(returns_mtx).name() << "\n";
	cout << returns_mtx << "\n\n";

	cout << "Monthly ETF returns, %-form: = \n";
	for (const auto& row : returns_mtx.rowwise())
	{
		for (double element : row)
		{
			std::cout << std::right << std::setw(7) << std::fixed << std::setprecision(2)
				<< element * 100 << "%\t";
		}
		std::cout << "\n";
	}
	cout << "\n\n";

	// Suppose the allocations are fixed at 35%, 40%, 25%.  
	// We can get the monthly returns:
	VectorXd allocations{{0.35, 0.40, 0.25}};
	VectorXd monthly_returns = returns_mtx * allocations;

	cout << "Monthly portfolio returns, %-form: = \n";
	for (auto j = 0; j < monthly_returns.size(); ++j)
	{
		cout << std::right << std::setw(7) << std::fixed << std::setprecision(2)
			<< monthly_returns(j) * 100 << "%\n";
	}

	cout << "\n\n";
}

// This is used in the next function: eigen_mtx_unary_expr()
class SquareRoot
{
public:
	double operator()(double x) const
	{
		return std::sqrt(x);
	}
};

void eigen_mtx_unary_expr()
{
	cout << format("\n*** eigen_mtx_unary_expr() ***\n\n");

	MatrixXd vals_01
	{
		{9.0, 8.0, 7.0},
		{3.0, 2.0, 1.0},
		{9.5, 8.5, 7.5},
		{3.5, 2.5, 1.5}
	};

	cout << "Original mtx: " << "\n";
	cout << vals_01 << "\n\n";

	cout << "Square each element with unaryExpr(.): " << "\n";
	vals_01 = vals_01.unaryExpr([](double x) {return x * x;});
	cout << "Type of vals_01 (mtx): " << typeid(vals_01).name() << "vals now = \n" << vals_01 << "\n\n";

	cout << "Now take square root of each element in the updated vals mtx with unaryExpr(.): " << "\n";
	SquareRoot sq{};		// Defined just above the current function
	vals_01 = vals_01.unaryExpr(sq);
	cout << "vals type: " << typeid(vals_01).name() << "\n" << vals_01 << "\n\n";

	// Could also copy to a new matrix:
	cout << "Results stored as new MatrixXd objects: " << "\n";
	MatrixXd vals_02{vals_01.unaryExpr([](double x) {return x * x;})};
	cout << vals_02 << "\n\n";

	MatrixXd vals_03 = vals_02.unaryExpr(sq);
	cout << vals_03 << "\n\n";
}

void eigen_math_array_fcns()
{
	cout << "\n" << std::format("\n*** eigen_math_array_fcns() ***") << "\n";

	// Same vals matrix as before:
	MatrixXd vals_01
	{
		{9.0, 8.0, 7.0},
		{3.0, 2.0, 1.0},
		{9.5, 8.5, 7.5},
		{3.5, 2.5, 1.5}
	};

	cout << "Square each element with .array.square(.): " << "\n";
	vals_01 = vals_01.array().square();
	cout << vals_01 << "\n\n";

	cout << "Now take square root of each element with .array().sqrt(): " << "\n";
	vals_01 = vals_01.array().sqrt();
	cout << vals_01 << "\n\n";

	// Could also copy to a new matrix:
	cout << "Results stored as new MatrixXd objects: " << "\n";
	MatrixXd vals_squared{vals_01.array().square()};
	cout << vals_squared << "\n\n";

	// Check that .array().square() is non-mutating:
	cout << "Was the vals_01 matrix modified in the process? (ans: no)\n" << vals_01 << "\n\n";

	MatrixXd vals_squared_sqrt = vals_squared.array().sqrt();
	cout << vals_squared_sqrt << "\n\n";

	// Not explicitly shown in the book; 
	// provided as a related example:
	MatrixXd prices_to_returns
	{
		{25.5, 8.0, 70.5},
		{31.0, 7.5, 71.0},
		{29.5, 8.5, 77.5},
		{33.5, 5.5, 71.5},
		{26.5, 9.5, 72.5},
		{34.5, 8.5, 75.5},
		{28.5, 9.0, 72.0},
		{23.5, 7.5, 73.5},
		{28.0, 8.0, 72.5},
		{31.5, 9.0, 73.0},
		{32.5, 9.5, 74.5}
	};

	// Take natural log of each price:
	prices_to_returns = prices_to_returns.array().log();
	cout << "Logs of prices using .array().log()\n";
	cout << "(However, would need to apply std::adjacent_difference etc):\n";
	cout << prices_to_returns << "\n\n";		
}