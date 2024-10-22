/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

module;
#include <cmath>

export module SingleFileModule;

// Standard Library Header Units:
import <vector>;
import <algorithm>;
import <ranges>;
import <iterator>;          // std::back_inserter(.)

// SimpleClass declaration and implementation together
export class SimpleClass
{
public:
    SimpleClass(int k) :k_{k} {}

    int get_val() const
    {
        return k_;
    }

    void reset_val(int k)
    {
        k_ = k;
    }

private:
    int k_;
};

std::vector<double> vector_fcn_helper(const std::vector<double>& x)
{
    // transform to new vector y and return to export std::vector<double> vector_fcn(const std::vector<double>& x)

    std::vector<double> y;
    y.reserve(x.size());
    std::ranges::transform(x, std::back_inserter(y), [](double q) {return 3 * q;});

    return y;
}

export std::vector<double> vector_fcn(const std::vector<double>& x)
{
    return vector_fcn_helper(x);
}

export std::vector<double> math_fcn(const std::vector<double>& x)
{    
    std::vector<double> y;
    y.reserve(x.size());
    std::ranges::transform(x, std::back_inserter(y), [](double q) {return std::sin(q) + std::cos(q);});

    return y;
}