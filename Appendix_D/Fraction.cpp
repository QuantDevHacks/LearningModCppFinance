// This file is licensed under the Mozilla Public License, v. 2.0.
// You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.

#include "Fraction.h"

#include <cassert>
#include <numeric>		// std::gcd
#include <iostream>
#include <format>

Fraction::Fraction(int n, int d):n_{n}, d_{d} 
{
	assert(!(n_ < 0) && !(d_ <= 0));

	int frac_gcd = std::gcd(n_, d_);
	n_ /= frac_gcd;
	d_ /= frac_gcd;

	if (n_ == d_)
	{
		n_ = 1;
		d_ = 1;
	}
}

std::strong_ordering Fraction::operator <=> (const Fraction& rhs) const
{
	if (n_ * rhs.d_ < rhs.n_ * d_)
	{
		return std::strong_ordering::less;
	}
	else if (*this == rhs)
	{
		return std::strong_ordering::equivalent;
	}
	else
	{
		return std::strong_ordering::greater;
	}
}

Fraction Fraction::operator *(const Fraction& rhs) const
{
	return Fraction{n_ * rhs.n_, d_ * rhs.d_};
}

int Fraction::numer() const
{
	return n_;
}

int Fraction::denom() const
{
	return d_;
}