/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */ 

#pragma once

#include "ChronoDate.h"
#include "DayCounts.h"
#include <chrono>

// std::year_month_day and related examples
void std_year_month_day_tests();		// Top level calling function
										// See StdYearMonthDayExamples.cpp
void date_construction();
void serial_representation();
void date_accessor_functions();
void date_validation();
void leap_years_and_last_day_of_month();
unsigned last_day_of_the_month(std::chrono::year_month_day& ymd);
void weekdays_and_weekends();
bool is_weekend(const std::chrono::year_month_day& ymd);
void add_days_months_years();

// ChronoDate Examples
// Most of these examples are not in the book, but they
// are provided to demonstrate how you can use the ChronoDate class.

void chrono_date_tests();		// Top level calling function
								// See ChronoDataExamples.cpp
void ctor_ymd();
void ctor_default();
void test_add_fcns();
void test_date_diff();
void test_spaceship();
void test_days_in_month();
void test_vector_of_dates();

// Day Count Basis examples
void day_count_basis_tests();		// Top calling function
									// See DayCountBasisTests.cpp
void book_examples();

double treasury_bill(const ChronoDate& sett_date,
	const ChronoDate& maturity_date, double mkt_yield,
	double face_value, const DayCount& dc);

void treasury_bill_examples();

// Bond example (this also includes an example of
// constructing a yield curve.
void valuation_20_yr_bond();		// See BondExamples.cpp