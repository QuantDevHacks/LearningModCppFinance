/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <chrono>
#include <compare>

class ChronoDate
{
public:
	ChronoDate(int year, unsigned month, unsigned day);

	// Maybe change this to year_month_day ymd and provide std::move option
	ChronoDate(std::chrono::year_month_day ymd);	// Can pass and/or initialize by move (see Ch 2)
	ChronoDate() = default;	

	// Accessors:
	int serial_date() const;
	std::chrono::year_month_day ymd() const;
	int year() const;
	unsigned month() const;
	unsigned day() const;

	// Properties (check state):
	unsigned days_in_month() const;
	bool is_end_of_month() const;
	bool is_leap_year() const;

	// Operators
	int operator - (const ChronoDate& rhs) const;
	bool operator == (const ChronoDate& rhs) const;
	std::strong_ordering operator <=> (const ChronoDate& rhs) const;

	// Modifying member functions:
	ChronoDate& add_years(int rhs_years);
	ChronoDate& add_months(int rhs_months);
	ChronoDate& add_days(int rhs_days);
	ChronoDate& weekend_roll();

private:
	// Default date used for default constructor (in-class member initialization):
	std::chrono::year_month_day date_
		{std::chrono::year{1970}, std::chrono::month{1}, std::chrono::day{1}};

	void validate_() const;
};

std::ostream& operator << (std::ostream& os, const ChronoDate& rhs);