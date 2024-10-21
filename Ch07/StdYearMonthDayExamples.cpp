/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "ExampleDeclarations.h"
#include "Timer.h"
#include <chrono>
#include <array>
#include <iostream>
#include <iomanip>
#include <format>

using std::cout;
using std::format;
namespace date = std::chrono;
using Date = std::chrono::year_month_day;

void std_year_month_day_tests()
{
	using namespace std::chrono;

	date_construction();
	serial_representation();
	date_accessor_functions();
	date_validation();
	leap_years_and_last_day_of_month();
	weekdays_and_weekends();
	add_days_months_years();	
}

void date_construction()
{
	cout << "\n*** date_construction() ***\n";

	std::chrono::year_month_day ymd{std::chrono::year{2022}, 
		std::chrono::month{11}, std::chrono::day{14}};
	cout << "year_month_day object ymd: " << ymd << "\n";		// << is overloaded for year_month_day

	// The "/" operator is overloaded to define a date, which we can then
	// use with copy initialization:
	std::chrono::year_month_day ymd_slash =
		std::chrono::year{2022} / std::chrono::month{11} / std::chrono::day{15};

	std::chrono::year_month_day alt_slash{std::chrono::year{2022} / std::chrono::November / 16};

	std::chrono::year_month_day ymd_alt_numerical{std::chrono::year{2022} / 11 / 17};
	auto ymd_eql_init = std::chrono::year{2022} / 11 / 18;

	auto the_year = static_cast<int>(ymd.year());	// Cannot do static_cast<unsigned>(year)
	auto the_month = static_cast<unsigned>(ymd.month());
	auto the_day = static_cast<unsigned>(ymd.day());

	// mm/dd/yyyy format can also be used:
	auto mdy = std::chrono::November / 19 / 2022;

	cout << "ymd_slash  = " << ymd_slash << "\n";
	cout << "alt_slash  = " << alt_slash << "\n";
	cout << "ymd_alt_numerical  = " << ymd_alt_numerical << "\n";
	cout << "mdy  = " << mdy << "\n\n";

	// Chrono Literals
	using namespace std::chrono_literals;

	std::chrono::year_month_day ymd_lit_1{2024y / 9 / 16d};  // yyyy/mm/dd
	std::chrono::year_month_day ymd_lit_2{10d / 10 / 2025y}; // dd/mm/yyyy
	std::chrono::year_month_day ymd_lit_3 = 7 / 16d / 2024y; // mm/dd/yyyy
	// Can also combine with month contstants:
	std::chrono::year_month_day ymd_lit_4 = std::chrono::October / 26d / 1973y;

	cout << "Chrono Literals:\n";
	cout << ymd_lit_1 << "\n";
	cout << ymd_lit_2 << "\n";
	cout << ymd_lit_3 << "\n";
	cout << ymd_lit_4 << "\n\n";

	// Can drop 'd' and 'y' suffixes (trivial reassignment):
	ymd_lit_1 = 2024y / 9 / 16;
	ymd_lit_2 = 10d / 10 / 2025;
	ymd_lit_3 = 7 / 16d / 2024;
	ymd_lit_4 = std::chrono::October / 26 / 1973;

	cout << "Chrono Literals reassigned, no 'd' or 'y' suffixes:\n";
	cout << ymd_lit_1 << "\n";
	cout << ymd_lit_2 << "\n";
	cout << ymd_lit_3 << "\n";
	cout << ymd_lit_4 << "\n\n";

	// Bring entire std::chrono namespace into local scope:
	using namespace std::chrono;
	year_month_day ymd_lit_commas{year{1999}, month{11}, day{11}};
	year_month_day ymd_in_october = October / 26 / 1973;
	cout << ymd_lit_commas << ", " << ymd_in_october << "\n\n";

	cout << "\n\n";
}

void serial_representation()
{
	cout << "\n*** serial_representation() ***\n";

	using namespace std::chrono;

	year_month_day epoch{year{1970}, month{1}, day{1}};
	year_month_day epoch_plus_1{year{1970}, month{1}, day{2}};
	year_month_day epoch_minus_1{year{1969}, month{12}, day{31}};

	int first_days_test =
		sys_days(epoch).time_since_epoch().count();		// 0
	cout << epoch << ", serial date = " << first_days_test << "\n";

	first_days_test =
		sys_days(epoch_plus_1).time_since_epoch().count();	// 1
	cout << epoch_plus_1 << ", serial date = " << first_days_test << "\n";

	first_days_test =
		sys_days(epoch_minus_1).time_since_epoch().count();	// -1 
	cout << epoch_minus_1 << ", serial date = " << first_days_test << "\n\n";

	year_month_day ymd{year{2022}, month{11}, day{14}};
	year_month_day ymd_later{year{2023}, month{5}, day{14}};
	int diff = ((sys_days(ymd_later) - sys_days(ymd))).count();		// = 181
	cout << "Difference in days between 2023-5-14 and 2022-11-14 = " << diff << "\n";
}

void date_accessor_functions()
{
	cout << "\n*** date_accessor_functions() ***\n";

	using namespace std::chrono;

	year_month_day ymd{year{2022}, month{11}, day{14}};
	year_month_day ymd_later{year{2023}, month{5}, day{14}};

	auto ymd_year = ymd.year();		// returns std::chrono::year
	auto ymd_month = ymd.month();	// returns std::chrono::month
	auto ymd_day = ymd.day();		// returns std::chrono::day

	// Can get differences in years, months, and days:
	auto year_diff = (ymd_later.year() - ymd.year()).count();	// returns 2023 - 2022 = 1
	auto month_diff = (ymd_later.month() - ymd.month()).count();	// returns 11 - 5 = 6
	auto day_diff = (ymd_later.day() - ymd.day()).count();		// returns returns 14 - 14 = 0

	cout << format("type(year_diff): {}, year_diff = {}\n", typeid(year_diff).name(), year_diff);
	cout << format("type(month_diff): {}, month_diff = {}\n", typeid(month_diff).name(), month_diff);
	cout << format("type(day_diff): {}, day_diff = {}\n\n", typeid(day_diff).name(), day_diff);

	int the_year = static_cast<int>(ymd.year());					// 2022	(int)	
	unsigned the_month = static_cast<unsigned>(ymd.month());		// 11 	(unsigned)
	unsigned the_day = static_cast<unsigned>(ymd.day());			// 14	(unsigned)

	cout << format("ymd cast to int, unsigned, unsigned: {}, {}, {}\n\n", 
		the_year, the_month, the_day);
}

void date_validation()
{
	cout << "*** date_validation() ***\n";

	using namespace std::chrono;

	year_month_day ymd{year{2022}, month{11}, day{14}};
	bool torf = ymd.ok();			// true
	cout << std::boolalpha << ymd << ": " << torf << "\n";

	year_month_day negative_year{year{-1000}, October, day{10}};
	torf = negative_year.ok();		// true – negative year is valid
	cout << negative_year << ": " << torf << "\n";

	year_month_day ymd_invalid{year{2018}, month{2}, day{31}};
	torf = ymd_invalid.ok();			// false
	cout << ymd_invalid << ": " << torf << "\n";

	year_month_day ymd_completely_bogus{year{-2004}, month{19}, day{58}};
	torf = ymd_completely_bogus.ok();	// false
	cout << ymd_completely_bogus << ": " << torf << "\n\n";
}

void leap_years_and_last_day_of_month()
{
	cout << "\n" << "*** leap_years_and_last_day_of_month() ***" << "\n";
	using namespace std::chrono;

	year_month_day ymd_leap{year{2024}, month{10}, day{26}};
	bool torf = ymd_leap.year().is_leap();		// true
	cout << std::boolalpha << ymd_leap << " is a leap year? " << torf << "\n\n";

	// `year_month_day_last` represents the last day of a month:
	year_month_day_last eom_apr{year{2009} / April / last};	// std::chrono::last
	auto last_day = static_cast<unsigned>(eom_apr.day());	// result = 30
	cout << format("Last day in April = {}", last_day) << "\n";

	// Can also use `year_month_day_last` to check if a given date is eom:
	year_month_day ymd_eom{year{2009}, month{4}, day{30}};
	torf = ymd_eom == eom_apr;		// Returns true 
	cout << eom_apr << " == " << ymd_eom << "? " << torf << "\n\n";

	// Can also get the last day of the month of an arbitrary date
	year_month_day ymd = year{2024} / 2 / 21;
	year_month_day_last eom{year{ymd.year()} / month{ymd.month()} / last};
	last_day = static_cast<unsigned>(eom.day());	// result = 29
	cout << "Last day of the month in which " << ymd << " falls: " << last_day << "\n";

	// Can check if a given date falls on eom using a function (see implementation below):
	cout << "Last day of the month in which " << ymd << " falls (fcn call): "
		<< last_day_of_the_month(ymd) << "\n\n";

	// a year_month_day_last type is implicitly convertible to a
	// year_month_day via reassignment :
	ymd = eom_apr; // ymd is now 2009-04-30
	cout << "ymd reassigned to eom: " << ymd << "\n\n";
}

unsigned last_day_of_the_month(std::chrono::year_month_day& ymd)
{
	using namespace std::chrono;
	year_month_day_last eom{ymd.year() / ymd.month() / last};
	return static_cast<unsigned>(eom.day());
}


void weekdays_and_weekends()
{
	cout << "\n" << "*** weekdays_and_weekends() ***" << "\n";
	using namespace std::chrono;

	// Define a year_month_day date that falls on a business day (Wednesday):
	year_month_day ymd_biz_day{year{2022}, month{10}, day{26}};	// Wednesday

	// Its day of the week can be constructed as a weekday object:
	weekday dw{sys_days(ymd_biz_day)};
	unsigned iso_code = dw.iso_encoding();
	cout << ymd_biz_day << ", " << dw << ", " << iso_code << "\n";

	// Do the same thing for a weekend:
	year_month_day ymd_weekend{year{2024}, month{1}, day{7}};	// Sunday
	dw = sys_days(ymd_weekend);
	iso_code = dw.iso_encoding();
	cout << ymd_weekend << ", " << dw << ", " << iso_code << "\n\n";

	// Can subsume this into a function (is_weekend(.)) and call as follows:
	bool torf = is_weekend(ymd_biz_day);		// false (Wed)
	cout << std::boolalpha << ymd_biz_day << " is weekend? " << torf << "\n";

	torf = is_weekend(ymd_weekend);				// true (Sat) 
	cout << std::boolalpha << ymd_weekend << " is weekend? " << torf << "\n\n";
}

bool is_weekend(const std::chrono::year_month_day& ymd)
{
	using namespace std::chrono;
	weekday dw{sys_days(ymd)};
	return dw.iso_encoding() >= 6;
}

void add_days_months_years()
{
	cout << "\n" << "*** add_days_months_years() ***" << "\n";
	using namespace std::chrono;

	//year_month_day_last ymd_last{year{2022} / month{4} / last};
	//cout << ymd_last << "\n" << "\n";		// 30 April 2022

	cout << "Adding years :\n";
	year_month_day ymd_01{year{2002}, month{11}, day{14}};
	cout << "ymd_01 = " << ymd_01 << "\n";
	ymd_01 += years{2};
	cout << "ymd_01 + 2 years = " << ymd_01 << "\n";
	ymd_01 += years{18};
	cout << "ymd_01 + 18 more years = " << ymd_01 << "\n" << "\n";

	cout << "Adding months :\n";
	year_month_day ymd_02{year{2022}, month{2}, day{16}};
	cout << "ymd_02 = " << ymd_02 << "\n";
	ymd_02 += months{2};
	cout << "ymd_02 + 2 months = " << ymd_02 << "\n";
	ymd_02 += months{18};
	cout << "ymd_02 + 18 more months = " << ymd_02 << "\n";

	// Subtraction assignment is also available:
	ymd_02 -= months{2};		// Result: 2023-08-16
	cout << "ymd_02 - 2 months = " << ymd_02 << "\n\n";

	cout << "Add months to end - of - month: Invalid date problem\n";
	year_month_day ymd_eom_1{year{2015}, month{1}, day{31}};
	year_month_day ymd_eom_2{year{2014}, month{8}, day{31}};
	year_month_day ymd_eom_3{year{2016}, month{2}, day{29}};

	// Take copies to use momentarily:
	auto copy_ymd_eom_1 = ymd_eom_1;
	auto copy_ymd_eom_2 = ymd_eom_2;
	auto copy_ymd_eom_3 = ymd_eom_3;

	ymd_eom_1 += months{1};		// 2015-02-31 is not a valid date
	ymd_eom_2 += months{1};		// 2014-09-31 is not a valid date 
	ymd_eom_3 += months{12};	// 2017-02-29 is not a valid date

	cout << "Incorrect month addition (+=):\n";
	cout << ymd_eom_1 << ", " << ymd_eom_2 << ", " << ymd_eom_3 << "\n\n";


	auto add_months = [](year_month_day& ymd, unsigned mths) -> void
	{
		using namespace std::chrono;
		ymd += months(mths);    // Naively attempt the addition

		if (!ymd.ok())
		{
			ymd = ymd.year() / ymd.month() / day{last_day_of_the_month(ymd)};
		}
	};

	add_months(copy_ymd_eom_1, 1);
	add_months(copy_ymd_eom_2, 1);
	add_months(copy_ymd_eom_3, 12);

	// Trust, but verify:
	cout << "Corrected month addition:\n";
	cout << copy_ymd_eom_1 << ", " << copy_ymd_eom_2 << ", " << copy_ymd_eom_3 << "\n\n";

	cout << "Adding days:\n";
	year_month_day ymd_03{year{2022}, month{10}, day{7}};
	auto add_days = sys_days(ymd_03) + days(3);
	cout << "ymd_03 = " << ymd_03 << ", 2022.10.7 + 3 days = " << add_days << "\n";
	ymd_03 = add_days;	// Implicit conversion to `year_month_day`
	cout << "ymd_03 now = " << ymd_03 << "\n\n";
}