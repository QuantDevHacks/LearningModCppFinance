/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once

// The content in Appendix A is supplemental to Chapter 3.

#include <cmath>
#include <iostream>

class Base
{
public:
    virtual double trig_fcn(double x) const
    {
        return std::cos(x);
    }

    // Appendix A:  virtual default destructors:
    // Comment out two of the three cases to observe each remaining behavior.
 
    // Case 1:  No virtual destructor on the Base class;
    // the Derived class has its own destructor:
    ~Base() { std::cout << "Base destructor called\n\n"; }

    // Case 2:  Implement a virtual destructor on the base class, and
    // keep the destructor on the Derived class:
    //virtual ~Base() { std::cout << "Base (virtual) destructor called\n\n"; }

    // Case 3: 
    // The simplest (and usually best) way is to use a virtual default destructor
    // on the base class, which will call the compiler-provided default
    // destructors on the derived classes (so you can comment out the user-defined
    // destructor examples in the derived classes):
    //virtual ~Base() = default;
};

class Derived final : public Base
{
public:
    double trig_fcn(double x) const override
    {
        return std::sin(x);
    }

    // Appendix A:  virtual default destructors:

    // Use this for Case 1 noted in the Base class comments:
    ~Derived() { std::cout << "Derived destructor called\n\n"; }

    // If you implement a destructor on the Derived class, use the override keyword.
    // Use this for Case 2 noted in the Base class comments:
    //~Derived() override { std::cout << "Derived destructor (override) called\n"; }

    // Case 3 (see Base class comments): It usually suffices to just use a virtual default
    // destructor on the base class, without implementing a destructor
    // on derived classes (In this case, comment out the all of the
    // destructor implementations and use 
    // virtual ~Base() = default;
    // in the Base class above.
};