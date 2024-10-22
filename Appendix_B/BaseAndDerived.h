/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cmath>
#include <iostream>

// Base and Derived classes for Appendix B

class Base
{
public:
    virtual double trig_fcn(double x) const
    {
        return std::cos(x);
    }

    // Uncomment this (and the Derived implementation), and 
    // remove the slice_function(.) definition and calls --
    // then, you will prevent object slicing.
    //virtual double pure_virtual_trig_fcn(double x) const = 0;

    virtual ~Base() = default;

};

class Derived final : public Base
{
public:
    inline double trig_fcn(double x) const override
    {
        return std::sin(x);
    }

    // Uncomment this as well after declaring the pure virtual function in Base:
    //double pure_virtual_trig_fcn(double x) const override
    //{
    //    return std::sin(x) + std::cos(x);
    //}
};
