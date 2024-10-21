/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */ 

#include "ExampleDeclarations.h"
#include "DayCounts.h"

#include <vector>
#include <cassert>
#include <string>
#include <iostream>
#include <iomanip>
#include <format>

using std::string;
using std::cout, std::endl;
using std::format;

// Most of these examples are not in the book, but they
// are provided to demonstrate how you can use the ChronoDate class.

void chrono_date_tests()
{
	ctor_ymd();
	ctor_default();
	test_add_fcns();
	test_date_diff();
	test_spaceship();
	test_days_in_month();
	test_vector_of_dates();
}

void ctor_ymd()
{
	cout << endl << "*** ctor_ymd() ***" << endl;

	ChronoDate d4{2022, 11, 14};
	ChronoDate d2{1973, 10, 26};
	ChronoDate d3{1960, 10, 10};

	cout << "21st century date = " << d4
		<< ", days since 1970.01.01 = " << d4.serial_date()
		<< ", type = " << typeid(d4.serial_date()).name() << endl;
	cout << "20th century date after UNIX epoch = " << d2
		<< ", days since 1970.01.01 = " << d2.serial_date()
		<< endl;
	cout << "20th century date before UNIX epoch = " << d3
		<< ", days before 1970.01.01 = " << -d3.serial_date()
		<< endl;

	cout << endl << endl;
}

void ctor_default()
{
	cout << endl << "*** ctor_default() ***" << endl;
	ChronoDate def{};

	cout << "Default date = " << def
		<< ", days since 1970.01.01 = " << def.serial_date() << endl;

	cout << endl << endl;
}

void test_add_fcns()
{
	cout << endl << "*** test_add_fcns() ***" << endl;

	using namespace std::chrono;

	ChronoDate d1{};
	ChronoDate d2{1973, 10, 26};
	ChronoDate d4{2016, 7, 22};
	ChronoDate d5{1999,2,28};
	ChronoDate d6{2000, 12, 31};
	ChronoDate d7{2017,12,29};

	cout << "===== Test add_years() function =====" << endl;
	cout << "Original date = " << d1 << ", serial date = " << d1.serial_date() << ", add 1 year" << endl;
	d1.add_years(1);
	cout << d1 << endl;
	cout << std::boolalpha << "ChronoDate d1 year: " << (d1.year() == 1971)
		<< "; month: " << (d1.month() == 1)
		<< "; day: " << (d1.day() == 1) << endl;
	cout << "result = " << d1 << ", new serial date = " << d1.serial_date() << endl;

	d1.add_years(-2);
	cout << "Previous date - 2 years = " << d1 << endl;
	cout << "Serial date = " << d1.serial_date() << endl << endl;
	cout << "ChronoDate d1 year: " << (d1.year() == 1969) << "; month: "
		<< (d1.month() == 1) << "; day: "
		<< (d1.day() == 1) << endl << endl;
	//cout << "result = " << d1 << ", new serial date = " << d1.serial_date() << endl << endl;

	cout << "Start date (d5): " << d5 << endl;
	cout << "Serial date = " << d5.serial_date() << endl
		<< "Add one year: " << d5.add_years(1) << endl;
	cout << "New serial date = " << d5.serial_date() << endl << endl;

	cout << "Go back Jack, do it again - Start date (d5): " << d5 << endl;
	cout << "Serial date = " << d5.serial_date() << endl;
	cout << "Add one year: " << d5.add_years(1) << endl;
	cout << "New serial date = " << d5.serial_date() << endl << endl;

	d6.add_years(10);
	cout << "Expect 2010.12.31: " << d6 << endl << endl;

	cout << "===== Test add_months() function =====" << endl;
	cout << "d4 start date = " << d4 << endl;
	cout << "Serial date = " << d4.serial_date() << endl << endl;
	d4.add_months(7);
	cout << "Add 7 months -- ChronoDate d4 year: " << (d4.year() == 2017)
		<< "; month: " << (d4.month() == 2)
		<< "; day: " << (d4.day() == 22) << endl;
	cout << "result = " << d4 << endl << endl;
	cout << "New serial date = " << d4.serial_date() << endl << endl;

	// Reassign d6:
	d6 = year{1999} / month{12} / day{31};
	cout << "d6 start date = " << d6 << endl;
	cout << "Serial date = " << d6.serial_date() << endl << endl;

	d6.add_months(2);		// Verify eom to 29 Feb in leap year
	cout << "Add 2 months -- ChronoDate d6 year: " << (d6.year() == 2000)
		<< "; month: " << (d6.month() == 2)
		<< "; day: " << (d6.day() == 29) << endl;
	cout << "result = " << d6 << endl;
	cout << "New serial date = " << d6.serial_date() << endl << endl;

	// Test if start date is 29th (December) and end date is in February in non-leap year
	cout << "ChronoDate d7 = " << d7 << ".  Now roll to next February. " << endl;
	d7.add_months(2);
	cout << "result = " << d7 << endl << endl;

	cout << "===== Test add_days() function =====" << endl;
	cout << "d2 start date = " << d2 << endl;
	cout << "Serial date = " << d2.serial_date() << endl << endl;
	d2.add_days(365);
	cout << "Add 365 days -- ChronoDate d2 year: " << (d2.year() == 1974)
		<< "; month: " << (d2.month() == 10)
		<< "; day: " << (d2.day() == 26) << endl;
	cout << "result = " << d2 << endl;
	cout << "New serial date = " << d2.serial_date() << endl << endl;
	d2.add_days(-365);
	cout << "Add -365 days -- ChronoDate d2 year: " << (d2.year() == 1973)
		<< "; month: " << (d2.month() == 10)
		<< "; day: " << (d2.day() == 26) << endl;
	cout << "result = " << d2 << endl;
	cout << "New serial date = " << d2.serial_date() << endl << endl;
}

void test_date_diff()
{
	cout << endl << "*** test_date_diff() ***" << endl;
	ChronoDate d1{1973, 10, 26};
	ChronoDate d2{2016, 02, 22};
	ChronoDate d3{2016, 02, 29};
	ChronoDate d4{2016, 05, 31};

	auto diff1 = d2 - d1;
	auto diff2 = d3 - d2;
	auto eom_diff = d4 - d3;

	cout << d2 << " - " << d1 << " = " << diff1 << endl;
	cout << d3 << " - " << d2 << " = " << diff2 << endl;
	cout << d4 << " - " << d3 << " = " << eom_diff << endl;
	cout << ChronoDate{2023, 5, 14} << " - " << ChronoDate{2022,11,14} << " = "
		<< ChronoDate{2023, 5, 14} - ChronoDate{2022,11,14} << "\n";

	cout << endl << endl;
}

void test_spaceship()
{
	cout << endl << "*** test_spaceship() ***" << endl;

	ChronoDate d1{1973, 10, 26};
	ChronoDate d2{2016, 02, 22};
	ChronoDate d3{2016, 02, 29};
	ChronoDate d4{2016, 4, 30};
	ChronoDate d5{2016, 4, 30};

	cout << std::boolalpha;
	cout << d1 << " < " << d2 << "? " << (d1 < d2) << endl;
	cout << d1 << " > " << d2 << "? " << (d1 > d2) << endl;
	cout << d1 << " <= " << d2 << "? " << (d1 <= d2) << endl;
	cout << d1 << " >= " << d2 << "? " << (d1 >= d2) << endl;
	cout << d1 << " == " << d2 << "? " << (d1 == d2) << endl;
	cout << d1 << " != " << d2 << "? " << (d1 != d2) << endl << endl;

	cout << d2 << " < " << d3 << "? " << (d2 < d3) << endl;
	cout << d2 << " > " << d3 << "? " << (d2 > d3) << endl;
	cout << d2 << " <= " << d3 << "? " << (d2 <= d3) << endl;
	cout << d2 << " >= " << d3 << "? " << (d2 >= d3) << endl;
	cout << d2 << " == " << d3 << "? " << (d2 == d3) << endl;
	cout << d2 << " != " << d3 << "? " << (d2 != d3) << endl << endl;

	cout << d3 << " < " << d4 << "? " << (d3 < d4) << endl;
	cout << d3 << " > " << d4 << "? " << (d3 > d4) << endl;
	cout << d3 << " <= " << d4 << "? " << (d3 <= d4) << endl;
	cout << d3 << " >= " << d4 << "? " << (d3 >= d4) << endl;
	cout << d3 << " == " << d4 << "? " << (d3 == d4) << endl;
	cout << d3 << " != " << d4 << "? " << (d3 != d4) << endl << endl;

	cout << d4 << " < " << d5 << "? " << (d4 < d5) << endl;
	cout << d4 << " > " << d5 << "? " << (d4 > d5) << endl;
	cout << d4 << " <= " << d5 << "? " << (d4 <= d5) << endl;
	cout << d4 << " >= " << d5 << "? " << (d4 >= d5) << endl;
	cout << d4 << " == " << d5 << "? " << (d4 == d5) << endl;
	cout << d4 << " != " << d5 << "? " << (d4 != d5) << endl << endl;
}

void test_days_in_month()
{
	cout << endl << "*** test_days_in_month() ***" << endl;
	ChronoDate d1{};
	ChronoDate d2{1973, 10, 26};
	ChronoDate d3{2016, 2, 22};
	ChronoDate d4{2000, 4, 30};

	cout << "Days in 1970.01: " << d1.days_in_month() << endl;
	cout << "Days in 1973.10: " << d2.days_in_month() << endl;
	cout << "Days in 2016.02: " << d3.days_in_month() << endl;
	cout << "Days in 2000.04: " << d4.days_in_month() << endl << endl;
}

void test_vector_of_dates()
{
	cout << "\nVector of Dates:\n";
	std::vector<ChronoDate> v{{1973,10,26}, {1993,7,16}, {1997,2,21}, {1998,5,30}, {2023,5,30}};
	for (const ChronoDate& d : v)
	{
		cout << d << " ";
	}
	cout << "\n\n";
}


