#include "ExampleDeclarations.h"

// Really good Eigen reference:
// https://www.iamfaisalkhan.com/matrix-manipulations-using-eigen-cplusplus/
// Also good:
// https://www.quantstart.com/articles/Eigen-Library-for-Matrix-Algebra-in-C/

#include <Eigen/Dense>
#include <cmath>
#include <complex>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <functional>
#include <random>
#include <vector>

#include <utility>		// std::move
#include <format>
#include <iostream>

using std::vector;
using std::cout;

using Eigen::MatrixXd;
using Eigen::VectorXd;
using Eigen::RowVectorXd;

void eigen_intro_examples()		// Top calling function used in main()
{			
	fixed_dim_matrices();
	construct_dynamic_matrix();
	vector_and_row_vector();
	products();
	problem_with_auto();
	portfolio_example();
	dot_products();
	addition_and_subtraction();
}

void fixed_dim_matrices()
{
	cout << "*** fixed_dim_matrices() *** " << "\n\n";
	Eigen::Matrix3d dbl_mtx
	{
		{10.64, 41.28, 21.63},
		{41.95, 87.45, 13.68},
		{22.47, 57.34, 8.631}
	};

	Eigen::Matrix4i int_mtx
	{
		{24, 0, 23, 13},
		{8, 75, 0, 98},
		{11, 60, 1, 3},
		{422, 55, 11, 55}
	};

	cout << dbl_mtx << "\n\n";
	cout << int_mtx << "\n\n";

	// Individual rows and columns can also be accessed, using zero-based indexing.  The first column of the first matrix, and the third column of the second, for example, are obtained with respective accessor functions:

	cout << "First column of double prec mtx:\n" << dbl_mtx.col(0) << "\n\n";
	cout << "Third row of int mtx:\n" << int_mtx.row(2) << "\n\n";
}

void construct_dynamic_matrix()
{
	cout << "*** construct_dynamic_matrix() *** " << "\n\n";

	// We will simply use the MatrixXd type for a matrix, and 
	// VectorXd for a vector in Eigen

	MatrixXd mtx_01
	{
		{1.0, 2.0, 3.0},
		{4.0, 5.0, 6.0},
		{7.0, 8.0, 9.0},
		{10.0, 11.0, 12.0}
	};

	cout << "Contents of the matrix mtx are:\n" << mtx_01 << "\n\n";

	// We can also use the default ctor, and then set the number of rows and columns
	// after the fact, using the resize member function:
	MatrixXd mtx_02{};
	mtx_02.resize(2, 2);
	mtx_02 << 10.0, 12.0, 14.0, 16.0;
	cout << "Contents of the matrix mtx_03 are:\n" << mtx_02 << "\n\n";

	MatrixXd mtx_03{4, 3};		// 4 rows, 3 columns

	// stream operator << has been defined for MatrixXd to take a stream
	// of doubles as input, row major order:
	mtx_03 << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0;

	// cout has been overloaded to output the contents of an Eigen MatrixXd to the console:
	cout << "Contents of the matrix mtx1 are:\n" << mtx_03 << "\n\n";

	// Finally, we can also set matrix values element by element, using 
	// its mutator function:
	MatrixXd mtx_04{2, 2};
	// 0-index as is the case in C++ generally:
	mtx_04(0, 0) = 3.0;
	mtx_04(1, 0) = 2.5;
	mtx_04(0, 1) = -1.0;
	mtx_04(1, 1) = mtx_04(1, 0) + mtx_04(0, 1);
	cout << "Contents of the matrix mtx_04 are:\n" << mtx_04 << "\n\n";

	// --- Matrix rows and columns ---
	// Individual columns and rows returned by their respective
	// accessors are Eigen::Block types:
	cout << mtx_01.col(0) << "\n\n";
	cout << mtx_01.row(2) << "\n\n";

	// Block types can be implicitly converted to dynamic Eigen Matrix (or Vector) types:
	VectorXd first_col_vec{mtx_01.col(0)};
	RowVectorXd third_row_vec = mtx_01.row(2);

	cout << "First column of mtx as VectorXd:\n";
	cout << first_col_vec << "\n\n";

	cout << "Third row of mtx as RowVectorXd:\n";
	cout << third_row_vec << "\n\n";

	// Not in book, but the following show how .row(.) (and .col(.)) return
	// *mutating* views of a row (or column) of an Eigen::Matrix:
	cout << "Is a block owning or a mutating view? (1st show original matrix):\n";
	cout << mtx_01 << "\n\n";
	auto first_row = mtx_01.row(0);
	first_row(0) = -100.0;
	cout << "Was the original matrix modified: (row 1, col 1) = -100.0? (ans: yes)\n";
	cout << mtx_01 << "\n\n";

	cout << "What if we construct a MatrixXd from a block?\n";
	MatrixXd first_row_matrix{first_row};
	cout << first_row_matrix << "\n\n";
	cout << "What if we change the 1st element of first_row to -200.0, what happens to first_row_matrix? (ans: nothing)\n";
	first_row(0) = -200.0;
	cout << first_row_matrix << "\n\n";
	cout << "What about the original mtx from which the row block was taken? (ans: it is modified (-200))\n";
	cout << mtx_01 << "\n";

	cout << "\n\n";
}

void vector_and_row_vector()
{
	cout << "*** vector_and_row_vector() *** " << "\n\n";

	// --- VectorXd analogs are as follows ---:
	VectorXd vec_01{{1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0}};
	// cout has also been overloaded to output the contents of an Eigen VectorXd to the console:
	cout << "Contents of the VectorXd vec are:\n" << vec_01 << "\n\n";

	VectorXd vec_02{12};		// 12 elements
	// stream operator << has also been defined for VectorXd to take a stream
	// of doubles as input:
	vec_02 << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0;
	cout << "Contents of the VectorXd vec_02 are:\n" << vec_02 << "\n\n";

	// Similar for VectorXd:
	VectorXd vec_03{};
	vec_03.resize(4);
	vec_03 << 10.0, 12.0, 14.0, 16.0;
	cout << "Contents of the VectorXd vec_03 are:\n" << vec_03 << "\n\n";

	// Again, similar for VectorXd:
	VectorXd vec_04{3};			// 3 elements
	vec_04(0) = 3.19;
	vec_04(1) = 2.58;
	vec_04(2) = 10.87;
	cout << "Contents of the VectorXd vec_04 are:\n" << vec_04 << "\n\n";

	RowVectorXd b{{1.0, 2.0, 3.0, 4.0}};     // A row-vector with 4 coefficients

	cout << "RowVectorXd b = " << b << "\n";

	cout << "\n\n";
}

void products()
{
	cout << "*** products() *** " << "\n\n";
	MatrixXd A
	{
		{1.0, 2.0, 3.0},
		{1.5, 2.5, 3.5},
		{4.0, 5.0, 6.0},
		{4.5, 5.5, 6.5},
		{7.0, 8.0, 9.0}
	};

	MatrixXd B
	{
		{1.0, 2.0, 3.0, 4.0, 5.0},
		{1.5, 2.5, 3.5, 4.5, 5.5},
		{5.0, 6.0, 7.0, 8.0, 8.5}
	};

	cout << "Matrix A:\n " << A << "\n\nMatrix B:\n" << B << "\n\n";

	MatrixXd prod_ab = A * B;	// Eigen::Product<Matrix<double,...>, Matrix<double,...>> type
	cout << "A (5 x 3) * B (3 x 5) = \n" << prod_ab << "\n";
	cout << "Type = " << typeid(prod_ab).name() << "\n\n";

	// Scalar multiplication is also available:
	MatrixXd scale_a = 0.5 * A;
	cout << "Scalar multiplication: 0.5 * A:\n";
	cout << scale_a << "\n\n";

	// Scalar multiplication assignment can be applied to an individual row or column:
	B.col(1) *= 2.0;
	cout << "Multiply 2nd column of B by 2:\n";
	cout << B << "\n\n";
	cout << "Type = " << typeid(B).name() << "\n\n";

	// Multiplication assignment can also be applied to 
	// an entire matrix:
	B *= 3.0;
	cout << "Scalar multiplication of matrix B by 3:\n";
	cout << B << "\n\n";
	cout << "Type = " << typeid(B).name() << "\n\n";


	// cwise:  https://eigen.tuxfamily.org/dox/group__QuickRefPage.html#title6
	MatrixXd cwise_prod = A.cwiseProduct(B.transpose());
	cout << "Elem by elem: A (5 x 3) * B^T (5 x 3) = \n" << cwise_prod << "\n\n";
}

void problem_with_auto()
{
	cout << "*** problem_with_auto() *** " << "\n\n";

	MatrixXd M
	{
		{1.0, 2.0, 3.0},
		{1.5, 2.5, 3.5},
		{4.0, 5.0, 6.0}
	};

	MatrixXd N
	{
		{10.0, 20.0, 30.0},
		{10.5, 20.5, 30.5},
		{40.0, 50.0, 60.0}
	};

	auto P = M * N;		// An Eigen::Product, a mutating
						// *view* of the product, and will be
						// modified as shown next.

	cout << "Matrix product type P: " << typeid(P).name() << "\n";
	cout << P << "\n\n";

	N.row(0) *= 0.1;
	cout << "Now P = \n";
	cout << P << "\n\n";		// P is modified

	// Change to M2, N2, and P2 so that this will
	// compile and run (in book, used the same M, N, and P naming)
	MatrixXd M2
	{
		{1.0, 2.0, 3.0},
		{1.5, 2.5, 3.5},
		{4.0, 5.0, 6.0}
	};

	MatrixXd N2
	{
		{10.0, 20.0, 30.0},
		{10.5, 20.5, 30.5},
		{40.0, 50.0, 60.0}
	};

	MatrixXd P2 = M2 * N2;
	cout << "Matrix product type P2: " << typeid(P2).name() << "\n";
	cout << P2 << "\n\n";

	N2.row(0) *= 0.1;
	cout << "Now P2 = \n";
	cout << P2 << "\n\n";		// No change
}

void portfolio_example()
{
	cout << "*** portfolio_example() *** " << "\n\n";

	// Simple portfolio example:
	MatrixXd corr_mtx
	{
		{1.0, 0.5, 0.25},
		{0.5, 1.0, -0.7},
		{0.25, -0.7, 1.0}
	};

	VectorXd vols{{0.2, 0.1, 0.4}};

	MatrixXd vol_mtx{vols.asDiagonal()};
	cout << "Type of vols.asDiagonal(): " << typeid(vols.asDiagonal()).name() << "\n\n";

	// Output of the covariance matrix calculation:

	MatrixXd cov_mtx = vols.asDiagonal() * corr_mtx * vols.asDiagonal();
	cout << "Covariance matrix type and contents: " << typeid(cov_mtx).name()
		<< " \n" << cov_mtx << "\n\n";

	//cout << vol_mtx << " * " << corr_mtx << " * " << vol_mtx
	//	<< " = " << cov_mtx << "\n\n";

	VectorXd fund_weights{{0.6, -0.3, 0.7}};
	double port_vol = std::sqrt(fund_weights.transpose() * cov_mtx * fund_weights);
	cout << "Portfolio volatility = " << port_vol << "\n\n";

}

void dot_products()
{
	cout << "*** dot_products() *** " << "\n\n";

	VectorXd u{{1.0, 2.0, 3.0}};
	VectorXd v{{0.5, -0.5, 1.0}};

	// Need to be careful here:
	double dp = u.transpose() * v;			// Returns 'double'
	MatrixXd op = u * v.transpose();		// Returns a matrix

	cout << "u' * v = " << dp << "\n\n";
	cout << "u * v' = \n" << op << "\n\n";

	// If you know you want the dot product, using
	// the dot(.) member function is safer:
	double dot_prod = u.dot(v);
	double dot_prod_rev = v.dot(u);
	cout << std::format("u dot v = {}, v dot u = {}", dot_prod, dot_prod_rev) << "\n\n";

}

void addition_and_subtraction()
{
	cout << "*** addition_and_subtraction() *** " << "\n\n";

	MatrixXd A
	{
		{1.0, 2.0, 3.0},
		{1.5, 2.5, 3.5},
		{4.0, 5.0, 6.0},
		{4.5, 5.5, 6.5},
		{7.0, 8.0, 9.0}
	};

	MatrixXd C
	{
		{10.0, 20.0, 30.0},
		{10.5, 20.5, 30.5},
		{40.0, 50.0, 60.0},
		{40.5, 50.5, 60.5},
		{70.0, 80.0, 90.0}
	};

	MatrixXd mtx_sum = A + C;
	cout << "Sum of matrices:\n";
	cout << mtx_sum << "\n\n";

	VectorXd u{{1.0, 2.0, 3.0}};
	VectorXd v{{0.5, -0.5, 1.0}};

	VectorXd vec_diff = v - u;
	cout << "Vector difference:\n";
	cout << vec_diff << "\n\n";
}