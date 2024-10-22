/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

export module Concepts;

import <concepts>;
import <type_traits>;


// This concept tests whether 't + u' is a valid expression
// See: https://devblogs.microsoft.com/cppblog/c20-concepts-are-here-in-visual-studio-2019-version-16-3/
export template<typename T>
concept can_add = requires(const T& t1, const T& t2) {t1 + t2;};

// The function is only a viable candidate if 't + u' is a valid expression
export template<typename T> requires can_add<T>
T add_them(const T& t1, const T& t2)  // requires can_add<T>
{
	return t1 + t2;
}

export template<typename T>
concept can_square = requires(const T& t) {t* t;};

// Can use this form...
#if 0
export template <typename T> requires can_square<T>
T square_it(const T& t)
{
	return t * t;
}
#endif

// Or the more compact form...
export template <can_square T> 
T square_it(const T& t)
{
	return t * t;
}

// We can also combine constraints based on
// multiple concepts; note the && operator 
// to enforce two (or more) constraints:
export template<typename T>
concept can_double = requires(const T & t) {t + t;};

//export template<typename T>
//concept both = can_square<T> && can_double<T>;


// Finally, some concepts are pre-defined in the Standard.
// For example, we might want to restrict a function to 
// floating-point types.  See: https://en.cppreference.com/w/cpp/concepts/floating_point
// Note this is essentially a pre-defined concept wrapping the type_trait is_floating_point.
export template <typename T> requires std::floating_point<T>
T floatingSquare(const T& t)
{
	return t * t;
}


export template<typename T>
concept Number =
std::is_integral<T>::value ||
std::is_floating_point<T>::value;

// Alternatively:
export template<typename T>
concept NumberVal =
std::is_integral_v<T> ||
std::is_floating_point_v<T>;

export template<Number N> auto algo(const N& a, const N& b)
{
	// ...
	return a + b;
}

// Or:
export template<typename T>
concept Arithmetic =
std::is_arithmetic_v<T>;

export template<Arithmetic X> 
X add_arithmetic_types(const X& a, const X& b)
{
	return a + b;
}