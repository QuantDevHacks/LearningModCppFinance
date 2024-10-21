/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "StlAlgorithms.h"

// References for Ranges/Views:
// https://hannes.hauswedell.net/post/2019/11/30/range_intro/
// https://www.modernescpp.com/index.php/c-20-the-ranges-library
// https://mariusbancila.ro/blog/2019/01/20/cpp-code-samples-before-and-after-ranges/ (std::as_const for cbegin)
// https://en.cppreference.com/w/cpp/utility/as_const (replaces cbegin (for now))

int main()
{
	intro_and_other_stl_algos();
	commonly_used_algorithms();
	locate_sort_search_copy_move();
	numerical_algorithms();
	range_view_examples();
	algo_examples_in_chptr_summary();
}