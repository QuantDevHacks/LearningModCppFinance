/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "BaseAndDerived.h"

int main()
{
	// Appendix A:
	Derived* d = new Derived{};
	Base* b = new Base{};			
	Base* bd = new Derived{};

	delete d;
	delete b;
	delete bd;

	// Will have an issue with smart pointers as well without 
	// a virtual destructor on the base class:
	std::unique_ptr<Base> ubd;
	ubd = std::make_unique<Derived>();
	ubd.reset();

}