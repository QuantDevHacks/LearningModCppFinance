/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <iostream>

template<typename T>
void print_this(T t)
{
	std::cout << t << " ";
}

// ValarrayDemos.cpp
void val_array_demos();					// Top calling function used in main()

void valarray_operators();
void valarray_slices();
void valarray_slice_products();