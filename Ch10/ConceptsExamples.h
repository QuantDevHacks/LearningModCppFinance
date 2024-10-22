/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <concepts>
#include <utility>

// Example function headers:
void concepts_examples();		// Top level calling program
void add_stuff();
void square_stuff();
void quadratic_stuff();

// This file contains function and class templates used in 
// examples in the book, as well as concepts examples.

template<typename T>
concept can_add = requires(const T & t1, const T & t2) {t1 + t2;};

template<typename T>
T add_them(const T& t1, const T& t2) requires can_add<T>
{
	return t1 + t2;
}

// Add/Subtract:
template<typename T>
concept can_add_and_subtr = requires(const T & t1, const T & t2)
{
	t1 + t2;
	t1 - t2;
};

template<typename T>
concept can_subtract = requires(const T & t1, const T & t2) { t1 - t2; };

template<typename T>
concept can_add_and_subtr_combined = requires(const T & t)
{
	can_add<T>&& can_subtract<T>;
};

template<typename T>
concept can_square = requires(const T & t) {t* t;};
		
// Can replace this with more expressive and concise definition just below
//template <typename T> requires can_square<T>
//T square_it(const T& t)
//{
//	return t * t;
//}

template <can_square T>
T square_it(const T& t)
{
	return t * t;
}
// Pre-defined Standard Library Concepts

// Note:  A separate FPQuadratic class template is used in
// place of the Quadratic class with std::floating_point<T> requirement
// only, shown at the beginning of the section on Pre-defined 
// Standard Library Concepts.  This way, the sample code will compile together.
template<typename T> requires std::floating_point<T>
class FPQuadratic  
{
public:
	FPQuadratic(const T& a, const T& b, const T& c):a_{a}, b_{b}, c_{c}{}
	T operator ()(T x) const
	{
		return (a_ * x + b_) * x + c_;
	}

private:
	T a_, b_, c_;
};

template<typename T>
concept Number = requires(T t)
{
	std::integral<T> || std::floating_point<T>;
};

// Can just use the Number concept as the template parameter:
template<Number N> 
class Quadratic
{
public:
	Quadratic(N a, N b, N c) :a_{a}, b_{b}, c_{c} {}

	N operator ()(N x) const
	{
		return (a_ * x + b_) * x + c_;
	}

private:
	N a_, b_, c_;
};