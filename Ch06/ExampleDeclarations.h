/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once
#include <iostream>

// Generic print function
template <typename T>
void print_this(const T& t)
{
	std::cout << t << " ";
}

// Engines and Distributions
void engines_and_distributions();		// Top calling function
										// See EnginesAndDistributions.cpp
void default_random_engine();
void mersenne_twister_and_normal_dist();
void other_distributions();
void shuffle_algo_example();
void max_drawdown_sim();

// Monte Carlo Simulation for Option Valuation
// (non-parallel version 1st, then followed by parallel STL algos,
// and then reprised MC Simulation with std::async).
void mc_option_examples();				// Top calling function
										// See MCOptionExamples.cpp
void euro_no_barrier_examples();		
void euro_with_barrier_examples();

// Parallel STL Algorithms
void parallel_stl_algorithms();			// Top calling function
										// See ParallelSTLAlgorithms.cpp
void parallel_min_max_mean();
void parallel_dot_product();
void exp_approx(unsigned num_elements, unsigned n);

// Concurrency
void concurrency_examples();			// Top calling function
										// See Concurrency.cpp
void squares();							

// MC Option pricing with async/future(s)
void mc_option_examples_parallel();			// Top calling function
void par_barrier_same_as_no_par();			// Quick test -- compare with earlier serial example
void perf_test_results_no_barrier();		// Sub-Top calling function
void perf_test_results_with_barrier();		// Sub-Top calling function
void euro_no_barrier_examples_par();		// See MCOptionExamplesParallel.cpp
void euro_with_barrier_examples_par();
void euro_no_barrier_examples_compare_async();
void euro_with_barrier_examples_compare_async();
void perf_tests_euro_no_barrier_examples(double time_to_exp, int time_steps, int num_scenarios);
void perf_tests_euro_with_barrier_examples(double time_to_exp, int time_steps, int num_scenarios);


// MC Option pricing with async/future(s), single seed
//void mc_option_examples_parallel_one_seed();		// Top calling function
//void perf_test_results_no_barrier_one_seed();		// Sub-Top calling function
//void perf_test_results_with_barrier_one_seed();		// Sub-Top calling function		
//void perf_tests_euro_no_barrier_examples_one_seed(double time_to_exp, int time_steps, int num_scenarios);
//void perf_tests_euro_with_barrier_examples_one_seed(double time_to_exp, int time_steps, int num_scenarios);
