/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */ 

#include "DayCounts.h"
#include <utility>

// *** Class Act365 ***
double Act365::year_fraction(const ChronoDate& date_01, const ChronoDate& date_02) const
{
	return (date_02 - date_01) / 365.0;
}

// *** Class Act360 ***
double Act360::year_fraction(const ChronoDate& date_01, const ChronoDate& date_02) const
{
	return (date_02 - date_01) / 360.0;
}

// *** Class Thirty360 ***
	// This is the **ISDA** 30/360  daycount.
	// See https://www.iso20022.org/15022/uhb/mt565-16-field-22f.htm and
	// http://kalotay.com/SupportingDocumentation/BondOAS/daycounts.html (30/360 ISDA)
double Thirty360::year_fraction(const ChronoDate& date_01, const ChronoDate& date_02) const
{
	return date_diff_(date_01, date_02) / 360.0;	
}

int Thirty360::date_diff_(const ChronoDate& date_01, const ChronoDate& date_02) const
{
	int d1, d2;
	d1 = date_01.day();
	d2 = date_02.day();

	if (d1 == 31) d1 = 30;

	// Actual rule is conditioned on d1 being 30 *or* 31,
	// but the case of 31 was already adjusted in the previous command
	if ((d2 == 31) && (d1 == 30)) d2 = 30;

	// For testing:
	return 360 * (date_02.year() - date_01.year()) + 30 * (date_02.month() - date_01.month()) + d2 - d1;
}
