/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "Bond.h"	
#include <iterator>

Bond::Bond(const std::string& bond_id, const ChronoDate& dated_date, 
	const ChronoDate& first_coupon_date, const ChronoDate& penultimate_coupon_date, 
	const ChronoDate& maturity_date, int coupon_frequency, double coupon_rate, 
	double face_value) : bond_id_{bond_id}
{
	// Generate the due and payment dates, and the payment amounts
	// Note that first_coupon_date, penultimate_coupon_date and the
	// maturity_date are due dates which fall on business days.
	// The first_coupon_date and penultimate_coupon_date are also dates
	// which are part of the regular schedule of due dates; the maturity
	// date may also be part of the regular schedule of due dates.
	
	// coupon_frequency is defined in the bond contract, and often stored in a bond database.
	// However, it is easier to use the length of the regular coupon period:
	// (1) Number of months in coupon period: 
	const int months_in_regular_coupon_period = 12 / coupon_frequency;
	//
	// Regular coupon periods are those which span two adjacent due dates.
	// All coupon periods, except the first and last are certain to be regular.
	// (2) Regular coupon payment:
	const double regular_coupon_payment = coupon_rate * face_value / coupon_frequency;

	calculate_pmt_schedule_(first_coupon_date, penultimate_coupon_date,
		months_in_regular_coupon_period, regular_coupon_payment);

	amend_initial_irregular_dates_and_pmts_(dated_date, first_coupon_date,
		months_in_regular_coupon_period, regular_coupon_payment);

	amend_final_irregular_dates_and_pmts_(penultimate_coupon_date, maturity_date,
		months_in_regular_coupon_period, regular_coupon_payment, face_value);

	// (10) Maturity date is a due date which falls on a business day
	due_dates_.push_back(maturity_date);
	payment_dates_.push_back(maturity_date);
}

void Bond::calculate_pmt_schedule_(const ChronoDate& first_coupon_date,
	const ChronoDate& penultimate_coupon_date, const int months_in_regular_coupon_period,
	const double regular_coupon_payment)
{
	// Bonds have many variations. Since this is not production code we simplify the code
	// by assuming the coupon day is less than 29; this avoids end of month calculations.
	// (3) Generate vectors containing due dates, payment dates, 
	// and regular coupon payment amounts:
	for (ChronoDate regular_due_date{first_coupon_date};
			regular_due_date <= penultimate_coupon_date;  
			regular_due_date.add_months(months_in_regular_coupon_period))
	{
		// The due and payment dates:
		due_dates_.push_back(regular_due_date);
		ChronoDate payment_date{regular_due_date};

		// (4) Roll any due dates falling on a weekend:
		payment_dates_.push_back(payment_date.weekend_roll());
		// Assume all coupons are regular; deal with short first period later.
		payment_amounts_.push_back(regular_coupon_payment);
	}
}

void Bond::amend_initial_irregular_dates_and_pmts_(const ChronoDate& dated_date, 
	const ChronoDate& first_coupon_date, const int months_in_regular_coupon_period, 
	const double regular_coupon_payment)
{
	// (5) If first coupon is irregular, amend the coupon payment.
	// Calculate the first_prior, the last regular date before first_coupon_date:
	ChronoDate first_prior{first_coupon_date};
	first_prior.add_months(-months_in_regular_coupon_period);
	if (first_prior != dated_date) // => irregular coupon
	{
		if (first_prior < dated_date) // => short coupon period
		{
			double coupon_fraction =
				static_cast<double>(first_coupon_date - dated_date) / 
					static_cast<double>(first_coupon_date - first_prior);

			payment_amounts_[0] *= coupon_fraction;
		}
		else // dated_date < first_prior => long coupon period
		{
			// long_first_coupon = regular_coupon + extra_interest.
			// Calculate the second_prior, the last regular date before the first_prior:
			ChronoDate second_prior{first_prior};
			second_prior.add_months(-months_in_regular_coupon_period);
			double coupon_fraction =
				static_cast<double>(first_prior - dated_date) / 
					static_cast<double>(first_prior - second_prior);
			payment_amounts_[0] += coupon_fraction * regular_coupon_payment;
		}
	}
}

void Bond::amend_final_irregular_dates_and_pmts_(const ChronoDate& penultimate_coupon_date, 
	const ChronoDate& maturity_date, const int months_in_regular_coupon_period, 
	const double regular_coupon_payment, const double face_value)
{
	// (6) If final coupon period is irregular amend the coupon payment
	// Calculate maturity_regular_date, the first regular date 
	// after penultimate_coupon_date:
	ChronoDate maturity_regular_date{penultimate_coupon_date};
	maturity_regular_date.add_months(months_in_regular_coupon_period);			// (7)
	double final_coupon{regular_coupon_payment};			

	if (maturity_regular_date != maturity_date) // => irregular coupon period	   (8)
	{
		if (maturity_date < maturity_regular_date) // => short coupon period
		{
			double coupon_fraction =
				static_cast<double>(maturity_date - penultimate_coupon_date) / 
					static_cast<double>(maturity_regular_date - penultimate_coupon_date);
			final_coupon *= coupon_fraction;
		}
		else // maturity_regular_date < maturity_date => long coupon period
		{
			// final_coupon = regular_coupon_amount + extra_interest.
			// Calculate the next_regular_date, the first regular date 
			// after the maturity_regular_date:
			ChronoDate next_regular_date{maturity_regular_date};
			next_regular_date.add_months(months_in_regular_coupon_period);
			double extra_coupon_fraction =
				static_cast<double>(maturity_date - maturity_regular_date) / 
					static_cast<double>(next_regular_date - maturity_regular_date);
			final_coupon += extra_coupon_fraction * regular_coupon_payment;
		}
	}

	// (9) Calculate final payment and store in the payment amounts vector:
	payment_amounts_.push_back(face_value + final_coupon);
}

double Bond::discounted_value(const ChronoDate& bond_settle_date, const YieldCurve& yield_curve)
{
	// The buyer receives the payments which fall due after the bond_settle_date
	// If the bond_settle_date falls on a due_date the seller receives the payment
	double pv = 0.0;
	for (size_t i = 0; i < due_dates_.size(); i++)
	{
		if (bond_settle_date < due_dates_[i])
		{
			pv += yield_curve.discount_factor(bond_settle_date, 
				payment_dates_[i]) * payment_amounts_[i];
		}
	}

	return yield_curve.discount_factor(yield_curve.settle_date(), bond_settle_date) * pv;
}

std::string Bond::bond_id() const
{
	return bond_id_;
}
