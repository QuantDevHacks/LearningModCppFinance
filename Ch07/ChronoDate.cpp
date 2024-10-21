/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "ChronoDate.h"
#include <stdexcept>		// std::invalid_argument
#include <utility>			// std::move

ChronoDate::ChronoDate(int year, unsigned month, unsigned day) :
	date_{std::chrono::year{year} / std::chrono::month{month} / std::chrono::day{day}}
{
	validate_();			
}

ChronoDate::ChronoDate(std::chrono::year_month_day ymd) : date_{std::move(ymd)}
{
	validate_();			// Throws exception if date is not valid
}

void ChronoDate::validate_() const
{
	if (!date_.ok())		// std::chrono member function to check if valid date
	{
		throw std::invalid_argument{"ChronoDate constructor: Invalid date."};
	}
}

// Accessors...
int ChronoDate::serial_date() const
{
	return std::chrono::sys_days(date_).time_since_epoch().count();
}

std::chrono::year_month_day ChronoDate::ymd() const
{
	return date_;
}

// chrono::year can be cast to int
// See https://en.cppreference.com/w/cpp/chrono/year/operator_int
int ChronoDate::year() const
{
	return static_cast<int>(date_.year());
}

// chrono::month can be cast to unsigned (not int)
// See https://en.cppreference.com/w/cpp/chrono/month/operator_unsigned
unsigned ChronoDate::month() const
{
	return static_cast<unsigned>(date_.month());
}

// chrono::day can be cast to unsigned (not int)
// See https://en.cppreference.com/w/cpp/chrono/day/operator_unsigned
unsigned ChronoDate::day() const
{
	return static_cast<unsigned>(date_.day());
}

// Properties...
unsigned ChronoDate::days_in_month() const
{
	using namespace std::chrono;
	year_month_day_last eom{date_.year() / date_.month() / last};
	return static_cast<unsigned>(eom.day());
}

bool ChronoDate::is_end_of_month() const
{
	return date_ == date_.year() / date_.month() / std::chrono::last;
}

bool ChronoDate::is_leap_year() const
{
	return date_.year().is_leap();
}

// Operators...
int ChronoDate::operator - (const ChronoDate& rhs) const
{
	return this->serial_date() - rhs.serial_date();
}

bool ChronoDate::operator == (const ChronoDate& rhs) const
{
	return this->serial_date() == rhs.serial_date();

	// Alternatively, we could also just wrap == already
	// defined on std::chrono::year_month_day:
	 //return this->ymd() == rhs.ymd();
}

std::strong_ordering ChronoDate::operator <=> (const ChronoDate& rhs) const
{
	if (this->serial_date() < rhs.serial_date())
	{
		return std::strong_ordering::less;
	}
	if (*this == rhs)
	{
		return std::strong_ordering::equivalent;
	}
	else
	{
		return std::strong_ordering::greater;
	}

	// Alternatively could also just wrap <=> already
	// defined on std::chrono::year_month_day:
	 //return this->ymd() <=> rhs.ymd();
}

ChronoDate& ChronoDate::add_years(int rhs_years)
{
	// Proceed naively: 
	date_ += std::chrono::years(rhs_years);

	// The only possible error case is if month is February
	// and the result is day = 29 in a non-leap year:
	if (!date_.ok())
	{
		date_ = date_.year() / date_.month() / 28;
	}

	return *this;
}

ChronoDate& ChronoDate::add_months(int rhs_months)
{
	date_ += std::chrono::months(rhs_months);    // Naively attempt the addition

	// If the date is invalid, it is because the
	// result is an invalid end-of-month:
	if (!date_.ok())
	{
		date_ = date_.year() / date_.month() / std::chrono::day{days_in_month()};
	}

	return *this;
}

ChronoDate& ChronoDate::add_days(int rhs_days)
{
	using namespace std::chrono;
	// Note that adding days is handled differently, per Howard Hinnant's Stack Overflow comments.
	// See https://stackoverflow.com/questions/62734974/how-do-i-add-a-number-of-days-to-a-date-in-c20-chrono

	date_ = sys_days(date_) + days(rhs_days);

	return *this;
}

ChronoDate& ChronoDate::weekend_roll()
{
	using namespace std::chrono;

	weekday wd{sys_days(date_)};		// std::chrono::weekday
	std::chrono::month orig_mth{date_.month()};

	unsigned wdn{wd.iso_encoding()}; // Mon = 1, ..., Sat = 6, Sun = 7

	// Sat: 8 - 6 = 2 days to roll forward, Sun: 8 - 7 = 1 day roll forward
	if (wdn > 5) date_ = sys_days(date_) + days(8 - wdn);

	// Case where date gets rolled into the 1st Monday of the next month --
	// Modified Following rule says to roll back three days to previous biz day:
	if (orig_mth != date_.month())
	{
		date_ = sys_days(date_) - days(3);
	}

	return *this;
}

// Non-member stream operator (for cout in particular):
std::ostream& operator << (std::ostream& os, const ChronoDate& rhs)
{
	os << rhs.ymd();
	return os;
}