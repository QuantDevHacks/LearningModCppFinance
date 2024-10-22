/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <vector>
#include <cstdlib>
#include <utility>
#include <cassert>
#include <iostream>
#include <format>

// Expression template class for vector addition:
template <typename U, typename V>
class VectorAddExpr
{
public:
    VectorAddExpr(const U& u, const V& v) : u_{u}, v_{v} 
    {
        assert(u_.size() == v_.size());
    }
 
    double operator[](size_t idx) const
    {
        return u_[idx] + v_[idx];
    }

    std::size_t size() const
    {
        return u_.size();
    }

    operator std::vector<double>() const
    {
        std::vector<double> result;
        result.reserve(this->size());

        for (size_t i = 0; i < this->size(); ++i) 
        {
            result.push_back(operator[](i));
        }

        return result;
    }

private:
    const U& u_;
    const V& v_;
};

// Helper function to create expression template for vector addition
template <typename U, typename V>
VectorAddExpr<U, V> operator +(const U& u, const V& v)
{
    return {u, v};
}