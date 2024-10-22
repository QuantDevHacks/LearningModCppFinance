/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

export module Templates;
import <iostream>;

export template<typename T>
T factored_polynomial(T a, T b, T c, T d)
{
	return (a + b) * (c + d);
}

export template<typename T>
void print_this(T t)
{
	std::cout << t << " ";
}

