// This file is licensed under the Mozilla Public License, v. 2.0.
// You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.

#include <iostream>

// References for STL:
// https://www.fluentcpp.com/2016/12/15/respect-levels-of-abstraction/ (with maps)
// https://www.youtube.com/watch?v=fHNmRkzxHWs (2014 CppCon - Carruth)

// for_each vs range-based for loop arguments:
// https://www.fluentcpp.com/2018/03/30/is-stdfor_each-obsolete/
// https://www.fluentcpp.com/2019/02/07/why-you-should-use-stdfor_each-over-range-based-for-loops/ (Jon Kalb)
// https://stackoverflow.com/questions/9994421/preferred-standard-use-range-based-for-or-stdfor-each
// https://www.go4expert.com/articles/cpp-11-range-based-loop-stdforeach-t34604/

#include "Ch04_example_functions.h"
#include "Fraction.h"
#include "UserDefinedTemplates.h"

int main()
{
	user_defined_template_examples();
	std_vectors();
	other_sequential_stl_containers();
	assoc_stl_containers();
	stl_iterator_examples();
}