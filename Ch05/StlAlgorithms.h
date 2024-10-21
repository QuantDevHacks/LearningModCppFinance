/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once

// Intro/Other STL Algorithms (IntroAndOtherStlAlgos.cpp)
void intro_and_other_stl_algos();		// Top calling function

// Used as predicate in count_if examples:
bool is_odd(int n);

void first_example_count_if();
void first_ex_with_other_containers();
void first_ex_with_ranges();

// Commonly Used Algorithms (CommonlyUsedAlgorithms.cpp)
void commonly_used_algorithms();		// Top calling function
void for_each_examples();
void transform_examples_same_container();
void transform_examples_different_container();
void std_begin_and_std_end();

// The Quadratic class is used within the next 
// two functions example functions below:
class Quadratic
{
public:
	Quadratic(double a, double b, double c) :
		a_{a}, b_{b}, c_{c} {}

	// Functor
	double operator()(double x) const
	{
		return (a_ * x + b_) * x + c_;
	}

	// Member function does the same thing:
	double value(double x) const
	{
		return (a_ * x + b_) * x + c_;
	}

private:

	double a_, b_, c_;

};

void class_functor_as_aux_fcn();
void member_fcn_as_aux_fcn();

void algo_examples_in_chptr_summary();	// Containers (mostly) interchangeable

// Locate, Sort, Search, Copy, Move (LocateSortSearchCopyMove.cpp)
void locate_sort_search_copy_move();		// Top calling function
void count_and_count_if();
void max_min_elems();
void sorting_values();
void searching_containers();		
void binary_search_examples();
void copy_and_move();

// Numeric Algorithms (NumericAlgorithms.cpp)
void numerical_algorithms();		// Top calling function
void iota_examples();
void accumulate_and_inner_product_examples();
void partial_sum_examples();
void adj_diff_examples();
void fold_left_examples();

void vwap_example();
void logs_of_adj_diff_divisions();
void adj_diffs_of_logs();



// Range views (RangeViews.cpp)
void range_view_examples();			// Top calling function
void indiv_views();
void chain_views();
void range_based_views();
void views_containers_loops();