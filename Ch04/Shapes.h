// This file is licensed under the Mozilla Public License, v. 2.0.
// You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.

#pragma once

#include <string>

class Shape
{
public:
	virtual std::string what_am_i() const;	// Returns "Shape"
	virtual double area() const = 0;
	virtual ~Shape() = default;		// Make sure to include a virtual default destructor 
};

class Circle final : public Shape
{
public:
	Circle(double radius);

	std::string what_am_i() const override;	// Returns "Circle"

	std::string what_i_am() const;	// Will compile and run if `override` keyword not appended
									// and function is implemented in the .cpp file (See Ch 3)

	double area() const override;	

private:
	double radius_;
};

class Rectangle final : public Shape
{
public:
	Rectangle(double length, double width);
	std::string what_am_i() const override;	// Returns "Rectangle"
	double area() const override;
	
private:
	double length_, width_;
};

void shapes_examples();

