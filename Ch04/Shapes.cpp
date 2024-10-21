// This file is licensed under the Mozilla Public License, v. 2.0.
// You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.

#include "Shapes.h"
#include <numbers>

// For test function; not used in classes:
#include <iostream>
#include <format>

// Shape implementation
std::string Shape::what_am_i() const
{
	return "Shape";
}

// Circle implementation
Circle::Circle(double radius) :radius_{radius} {}

std::string Circle::what_am_i() const		// override
{
	return "Circle";
}

// Note: this will compile if `override` keyword not included in its declaration,
// and the return value of what_am_i() would be "Shape" if not overridden on Circle class
std::string Circle::what_i_am() const{return "Circle";}

double Circle::area() const				// override
{
	return std::numbers::pi * radius_ * radius_;
}

// Rectangle implementation
Rectangle::Rectangle(double length, double width) :
		length_{length}, width_{width} {}

std::string Rectangle::what_am_i() const	// override
{
	return "Rectangle";
}

double Rectangle::area() const				// override
{
	return length_ * width_;
}

void shapes_examples()
{
	using std::cout;
	using std::format;

	cout << "\n*** shapes_examples() ***\n";

	double radius = 1.0;
	double length = 3.0, width = 2.0;

	Circle circle{radius};
	Rectangle rect{length, width};

	cout << format("Area of {} with radius = {}: {}\n", circle.what_am_i(), radius, circle.area());
	cout << format("Area of {} with length = {} and width = {}: {}\n\n", rect.what_am_i(), 
		length, width, rect.area());
}