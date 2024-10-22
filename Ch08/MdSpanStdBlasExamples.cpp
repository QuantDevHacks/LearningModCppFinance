#define MDSPAN_USE_PAREN_OPERATOR 1		
// For stdBLAS, to use the (i, j) operator.  
// stdBLAS will also support (by default?) the new [i, j] operator when
// released in C++26, but (i, j) is used for the following examples.
// Note also, in Visual Studio, you will need to use the
// Clang compiler option for these examples to compile and run.


#include "ExampleDeclarations.h"
#include <experimental/mdspan>
#include <experimental/linalg>
#include <vector>


#include  <cstdlib>
//using std::size_t;

#include <iostream>
using std::cout;

#include <format>
using std::format;

namespace stdex = std::experimental;

void md_span_and_std_blas_examples()
{
	using std::vector, std::size_t;

	fixed_dim_symm_mtx();

	vector<double> w{10.1, 10.2, 10.3, 10.4, 10.5, 10.6};
	size_t m = 3;
	size_t n = 2;
	print_dynamic_mdspan(m, n, w);				// Row major (default)

	submdspan_examples();
	std_blas_mtx_vector_prod();
}

void fixed_dim_symm_mtx()
{
	cout << "\n*** fixed_dim_symm_mtx() ***\n\n";
	using std::vector, std::size_t;

	vector v{101, 102, 103, 104, 105, 106};

	stdex::mdspan mds_01 {v.data(), 3, 2};
	size_t n_rows = mds_01.extent(0);		// = 3 rows
	size_t n_cols = mds_01.extent(1);		// = 2 columns
	size_t n_extents = mds_01.rank();		// = 2 dimensions

	cout << format("num rows = {}, num cols = {}, num extents = {}", n_rows, n_cols, n_extents) << "\n\n";

	for (size_t i = 0; i < n_rows; ++i)
	{
		for (size_t j = 0; j < n_cols; ++j)
			cout << mds_01(i, j) << "\t";			// Will be able to use mds1[i, j] in C++23

		cout << "\n";
	}

	cout << "\n\n";

	const auto mds_02 = stdex::mdspan{v.data(), 2, 3};

	for (size_t i = 0; i < mds_02.extent(0); ++i)
	{
		for (size_t j = 0; j < mds_02.extent(1); ++j)
			cout << mds_02(i, j) << "\t";

		cout << "\n";
	}

	cout << "\n\n";

	// Note that changing an element in the original vector is
	// also reflected in the associated mdspan objects:
	v[5] = 419;

	for (size_t i = 0; i < mds_01.extent(0); ++i)
	{
		for (size_t j = 0; j < mds_01.extent(1); ++j)
			cout << mds_01(i, j) << "\t";

		cout << "\n";
	}

	cout << "\n\n";

	for (size_t i = 0; i < mds_02.extent(0); ++i)
	{
		for (size_t j = 0; j < mds_02.extent(1); ++j)
			cout << mds_02(i, j) << "\t";

		cout << "\n";
	}

	cout << "\n\n";
}

void print_dynamic_mdspan(size_t m, size_t n, const std::vector<double>& v)
{
	cout << "\n" << "*** dynamic_mdspan(" << m << ", " << n << ")*** " << "\n";
	using std::vector, std::size_t;

	stdex::mdspan md{v.data(), stdex::extents{m, n}};		// Much easier with CTAD
	
	// Generalized -- note that without CTAD, we would have to
	//  include all of these specifications:
	//stdex::mdspan<const double, stdex::dextents<size_t, 2>> 
		//md{v.data(), stdex::dextents<size_t, 2>{m, n}};

	for (size_t i = 0; i < md.extent(0); ++i)
	{
		for (size_t j = 0; j < md.extent(1); ++j)
		{
			cout << md(i, j) << "\t";
		}

		cout << "\n";
	}

	cout << "\n\n";
}

void submdspan_examples()
{
	cout << "\n" << "*** sub_mdspan() *** " << "\n";
	using std::vector, std::size_t;

	// Borrowed from earlier example
	vector<int> v{101, 102, 103, 104, 105, 106};

	auto mds1 = stdex::mdspan{v.data(), 3, 2};
	size_t n_rows{mds1.extent(0)};		// 3
	size_t n_cols{mds1.extent(1)};		// 2
	size_t n_extents{mds1.rank()};		// 2

	cout << format("num rows = {}, num cols = {}, num extents = {}", n_rows, n_cols, n_extents) << "\n" << "\n";

	for (size_t i = 0; i < mds1.extent(0); ++i)
	{
		for (size_t j = 0; j < mds1.extent(1); ++j)
			cout << mds1(i, j) << "\t";

		cout << "\n";
	}

	auto row_1 = stdex::submdspan(mds1, 0, stdex::full_extent);
	auto col_2 = stdex::submdspan(mds1, stdex::full_extent, mds1.extent(1) - 1);

	//stdex::submdspan(mds1, stdex::full_extent, 1);

	cout << "\n" << typeid(row_1).name() << "\n";
	cout << row_1[0] << "\n" << "\n";


	cout << "1st row (index = 0): " << "\n";
	for (size_t k = 0; k < row_1.extent(0); ++k)
	{
		cout << format("row_1[{}] = {}", k, row_1[k]) << "\t";
	}

	cout << "\n" << "\n";

	cout << "2nd column (index = 1): " << "\n";
	for (size_t k = 0; k < col_2.extent(0); ++k)
	{
		cout << format("col_2[{}] = {}", k, col_2[k]) << "\t";
	}

	cout << "\n" << "\n";

	cout << "Modify last element of 2nd column submdspan; what happens to original mdspan?: " << "\n";
	col_2[2] = 3333;

	for (size_t i = 0; i < n_rows; ++i)
	{
		for (size_t j = 0; j < n_cols; ++j)
			cout << mds1(i, j) << "\t";

		cout << "\n";
	}

	cout << "\n\n";

	cout << "The original std::vector v is also modified:\n";
	for (int x : v)
	{
		cout << x << " ";
	}

	cout << "\n\n";
}

void std_blas_mtx_vector_prod()
{	
	cout << "\n" << "*** std_blas_mtx_vector_prod() *** " << "\n";

	using std::vector, std::size_t;
	namespace stdex = std::experimental;

	constexpr size_t m = 2;
	constexpr size_t n = 3;
	
	std::vector<double> A_vec(m * n);
	std::vector<double> x_vec(n);

	A_vec = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
	x_vec = {1.0, 2.0, 3.0};	

	print_dynamic_mdspan(m, n, A_vec);

	std::vector<double> y_vec(n);

	stdex::mdspan A{A_vec.data(), m, n};
	stdex::mdspan x{x_vec.data(), n};
	stdex::mdspan y{y_vec.data(), m};

	stdex::linalg::matrix_vector_product(A, x, y);

	cout << "Mtx-vector prod Ax = \n";
	for (size_t j = 0; j < m; ++j)
	{
		cout << y(j) << " ";		
	}

	cout << "\n\n";
}