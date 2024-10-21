/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "Ch03_example_functions.h"

// SimpleClass implementation (declaration in Ch03_example_functions.h)
SimpleClass::SimpleClass(int k) :k_{k} {}

int SimpleClass::get_val() const
{
	return k_;
}

void SimpleClass::reset_val(int k)
{
	k_ = k;
}

