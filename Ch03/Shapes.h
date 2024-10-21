/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>

class Shape
{
public:
	virtual double area() const = 0;
	virtual std::string what_am_i() const;	// Returns "Shape"
	virtual ~Shape() = default;		// Make sure to include a virtual default destructor 
};

class Circle final : public Shape
{
public:
	Circle(double radius);
	double area() const override;
	std::string what_am_i() const override;	// Returns "Circle"

	// Similar, but not same function name (might be a problem):
	std::string what_i_am() const;	// Will compile and run if `override` keyword not appended
									// and function is implemented in the .cpp file (See Ch 3)

private:
	double radius_;
};

class Rectangle final : public Shape
{
public:
	Rectangle(double length, double width);
	double area() const override;
	std::string what_am_i() const override;	// Returns "Rectangle"
	
private:
	double length_, width_;
};

// Just a sketch -- need to discuss clone() methods
// and unique pointers (see payoff and option examples)
class Enclose
{
public:
	Enclose(const Shape& shape)
	{
		// ....
	}

	double scale_area(double scale) const
	{
		return scale * shape_->area();
	}

private:
	Shape* shape_;
};



