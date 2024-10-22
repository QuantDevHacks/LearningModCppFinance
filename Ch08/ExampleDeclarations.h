#pragma once

#include "LazyEvaluation.h"
#include <cmath>
#include <vector>
#include <experimental/mdspan>
#include <iostream>
#include <cstdlib>      // std::size_t

// Generic print function (name clash with Eigen):
//template<typename T>
//void test_print(T t) { std::cout << t << " "; }

template<typename T>
void print_this(T t)
{
	std::cout << t << " ";
}

// LazyEvaluationDemonstrations.cpp
void lazy_evaluation_demonstration();		// Top calling function used in main()
void naive_vector_addition();
void sum_by_element();
void lazy_vector_addition();

// ExpressionTemplateExamples.cpp
void expression_template_examples();	// Top calling function used in main()
void add_four_real_vectors_exp_tmpl();
void vector_conversion_operator();

// EigenIntroExamples.cpp 
void eigen_intro_examples();			// Top calling function used in main()
void fixed_dim_matrices();
void construct_dynamic_matrix();
void vector_and_row_vector();
void products();
void portfolio_example();
void dot_products();
void addition_and_subtraction();
void problem_with_auto();

// EigenStlExamples.cpp
void eigen_stl_examples();				// Top calling function used in main()
void stl_and_eigen();
void rowwise_and_colwise();
void log_rtns_rowwise_colwise();
void eigen_mtx_unary_expr();
void eigen_math_array_fcns();

// EigenDecompositionExamples.cpp
void eigen_decomposition_examples();	// Top calling function used in main()
void qr_linear_regression();
void cholesky_correlated_random_equity_paths();
void principal_components_mtx();
// Extra examples not in the book:
void solve_linear_system();
void jacobi_svd_linear_regression();
void linear_regression_with_intercept();	// Extra example not in book


// MdSpanStdBlasExamples.cpp
void md_span_and_std_blas_examples();	// Top calling function used in main()
void fixed_dim_symm_mtx();
void print_dynamic_mdspan(size_t m, size_t n, const std::vector<double>& v);
void submdspan_examples();
void std_blas_mtx_vector_prod();