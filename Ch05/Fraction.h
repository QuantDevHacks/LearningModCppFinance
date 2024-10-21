/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <compare>

class Fraction
{
public:
	Fraction(int n, int d);
	//Fraction() = default;		// NEED FOR APPENDIX D!!!

	bool operator == (const Fraction& rhs) const = default;
	std::strong_ordering operator <=> (const Fraction& rhs) const /*= default*/;

	int numer() const;
	int denom() const;

	// Added for Ch 4:
	Fraction operator *(const Fraction& rhs) const;


private:
	// int n_{0}, d_{1};	// MOVE TO APPENDIX D!!!
	int n_, d_;

	//void simplify_();
	
};

// Remove these:
//void fraction_tests();
//void test_equality(unsigned n1, unsigned d1, unsigned n2, unsigned d2);
//void test_comparison(unsigned n1, unsigned d1, unsigned n2, unsigned d2);
//void test_product(unsigned n1, unsigned d1, unsigned n2, unsigned d2);