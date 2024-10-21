/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ChronoDate.h"

class DayCount
{
public:
	virtual double year_fraction(const ChronoDate& date_01, const ChronoDate& date2) const = 0;
	virtual ~DayCount() = default;
};

class Act365 : public DayCount
{
public:
	double year_fraction(const ChronoDate& date_01, const ChronoDate& date2) const override;
};

class Act360 : public DayCount
{
public:	
	double year_fraction(const ChronoDate& date_01, const ChronoDate& date2) const override;
};

class Thirty360 : public DayCount
{
public:
	double year_fraction(const ChronoDate& date_01, const ChronoDate& date2) const override;

private:
	int date_diff_(const ChronoDate& date_01, const ChronoDate& date2) const;
};
