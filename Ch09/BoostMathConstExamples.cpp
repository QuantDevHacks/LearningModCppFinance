#include "ExampleDeclarations.h"

#include <boost/math/constants/constants.hpp>

#include <format>
#include <iostream>
using std::format;
using std::cout;

void boost_math_const_examples()
{
	cout << "*** boost_math_const_examples() ***\n";

	auto std_norm_pdf = [](double x) -> double
	{
		return boost::math::double_constants::one_div_root_two_pi
			* std::exp(-boost::math::double_constants::half * x * x);
	};

	double y = std_norm_pdf(0.0);

	cout << format("Std Norm pdf f({}) = {}", 0, y) << "\n\n";

}