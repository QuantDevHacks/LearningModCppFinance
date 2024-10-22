// This file is licensed under the Mozilla Public License, v. 2.0.
// You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.

#include "Fraction.h"

#include <vector>
#include <format>
#include <iostream>

void round_brackets_and_braces();

// Appendix D is supplemental to Ch 4 
// (initializing a std::vector)

int main()
{
	round_brackets_and_braces();
}

void round_brackets_and_braces()
{
	using std::vector, std::cout, std::format;

	cout << "\n*** round_brackets_and_braces()  ***\n";

	// These results will be the same:
	Fraction braces{1, 2};
	Fraction round_brackets(1, 2);

	cout << format("Fraction in braces case: {}/{}\n\n", braces.numer(), braces.denom());
	cout << format("Fraction in round brackets case: {}/{}\n\n", round_brackets.numer(), round_brackets.denom());

	// In contrast, there is a difference in this case:
	vector<int> v_braces{10};				// size() == 1
	vector<int> v_round_brackets(10);		// size() == 10 
	auto size_rb = v_round_brackets.size();
	auto size_braces = v_braces.size();

	cout << format("Size of each vector<int> -- braces: {}, round brackets: {}\n\n",
		size_rb, size_braces);

	// Note the difference in results here if the Fraction constructor
	// taking a single int argument is added to the class (see Fraction.h)
	vector<Fraction> v_frac_braces{10};		// size() = 10 if single int ctor commented out
											// but size() = 1 if it is added to the class.
	vector<Fraction> v_frac_rb(10);
	size_rb = v_frac_braces.size();		
	size_braces = v_frac_rb.size();

	cout << format("Size of each vector<Fraction> -- braces: {}, round brackets: {}\n\n",
		size_rb, size_braces);

	// We also get different sizes here:
	vector<int> v1{10, -1};
	vector<int> v2(10, -1);
	cout << format("Size of each v1 (braces): {}, size of v2 (round brackets): {}\n\n",
		v1.size(), v2.size());

	cout << "\n";

	// For these reasons, if you wish to initialize a vector of, say, size 10,
	// use the C++03 style with the round bracket constructor form.  
	// This way, there is no ambiguity:

	vector<int> v_int_10(10);
	vector<Fraction> v_frac_10(10);

	cout << format("Size of each using round brackets: v_int_10: {}, v_frac_10: {}\n\n",
		v_int_10.size(), v_frac_10.size());

	cout << "\n";
}
