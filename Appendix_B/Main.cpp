/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "BaseAndDerived.h"
#include <cmath>
#include <numbers>
#include <format>
#include <iostream>

// Appendix B is supplemental to Ch 3

// Comment out this function (and calls to it) after adding in
// the pure virtual function and its Derived implementation:
void slice_function(Base b)
{
	using std::cout, std::format;
	using namespace std::numbers;

	cout << format("slice_function(.): trig_fcn(pi) = {}\n",
		b.trig_fcn(pi));
}

int main()
{
	using std::cout, std::format;
	using namespace std::numbers;

	// Suppose we create instances of each on the stack:
	Base b;			// Comment this out after adding in pure virtual function
	Derived d;

	// Then, call slice_function(.) with both `b` and `d`; `d` will get sliced:
	slice_function(b);	// Comment this out after adding in pure virtual function
	slice_function(d);	// Comment this out after adding in pure virtual function

	// Another example of object slicing is the following, where in a similar sense 
	//to the pointer `bd` in Appendix A, a `Base` object is constructed with a 
	// `Derived` copy constructor argument.  Will also get object slicing:
	Base bd{d};		// Comment this out after adding in the pure virtual function
	cout << format("trig_fcn(pi) = {}\n", bd.trig_fcn(pi));

	// By adding the pure virtual function pure_virtual_trig_fcn(.) to the
	// Base class and implementing it on the Derived class (and by disabling
	// slice_function(.), which will no longer compile), we can prevent 
	// object slicing (see the Base and Derived classes).

	// After adding in the pure virtual function, uncomment the following:
	//Derived dpv;
	//cout << format("trig_fcn(pi) = {}\n", dpv.trig_fcn(pi));
	//cout << format("pure_virtual_trig_fcn(pi) = {}\n", dpv.pure_virtual_trig_fcn(pi));

	// Now, object slicing is prevented by the compiler.
}

