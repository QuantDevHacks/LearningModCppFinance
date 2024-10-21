/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once
#include <iostream>

// Subset of user-defined templates from Ch 4, these are
// used in Ch 5 also.

template <typename T>
T tmpl_square(const T& t);

template <typename T>
T tmpl_square(const T& t)
{
	return t * t;
}

template<typename T>
void print_this(T t)
{
	std::cout << t << " ";
}



