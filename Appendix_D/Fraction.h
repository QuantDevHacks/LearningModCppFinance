// This file is licensed under the Mozilla Public License, v. 2.0.
// You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.

#pragma once

#include <compare>

class Fraction
{
public:
	Fraction(int n, int d);
	Fraction() = default;

	// Remove this comment, and then observe that the vector size 
	// changes to 1 in the round_brackets_and_braces() function.
	//Fraction(int n) :n_{n} {}

	bool operator == (const Fraction& rhs) const = default;
	std::strong_ordering operator <=> (const Fraction& rhs) const /*= default*/;

	int numer() const;
	int denom() const;

	// Added for Ch 4:
	Fraction operator *(const Fraction& rhs) const;


private:
	int n_{0}, d_{1};	
};
