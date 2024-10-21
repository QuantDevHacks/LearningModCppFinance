// This file is licensed under the Mozilla Public License, v. 2.0.
// You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.

#pragma once
#include <iostream>

template <typename T>
T tmpl_square(const T& t);

template <typename T>
T tmpl_square(const T& t)
{
	return t * t;
}

// Was originally template <typename T>
template <typename T = double>		// Can specify a default template parameter type
class MyPair
{
public:
	MyPair(T first, T second);
	T get_min() const;

private:
	T a_, b_;
};

template <typename T>
MyPair<T>::MyPair(T first, T second) :a_(first), b_(second) {}

template <typename T>
T MyPair<T>::get_min() const
{
	// See NOTE in text -- this is limiting to cases where 
	// a type T has a default constructor (so best not to do this):
	/*
	T retval;
	retval = a_ < b_ ? a_ : b_;
	return retval;
	*/

	// This is more general and preferred:
	return a_ < b_ ? a_ : b_;
}

template<typename T>
void print_this(T t)
{
	std::cout << t << " ";
}



