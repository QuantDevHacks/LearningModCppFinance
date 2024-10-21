/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
*/

#include "Ch03_example_functions.h"

#include <format>
#include <memory>	// unique_ptr, shared_ptr
#include <utility>	// move
#include <string>
#include <iostream>
using std::cout;

void smart_pointers()		// Top level calling function
{
	unique_pointers();
	shared_pointers();
}

void unique_pointers()
{
	using std::cout;
	using std::format;
	using std::string;

	cout << "\n" << "*** unique_pointers() ***" << "\n\n";
	std::unique_ptr<string> ptr_01;
	// . . .
	ptr_01 = std::make_unique<string>("To err is human");

	// Alternatively, on one line:
	auto ptr_02 =
		std::make_unique<string>("To moo, bovine");

	cout << format("Contents of ptr_01: {}\n", *ptr_01);
	cout << format("Contents of ptr_02: {}\n\n", *ptr_02);

	auto ptr_03 = std::move(ptr_02);

	if (ptr_02)
	{
		cout << format("ptr_02 is not null\n");
	}
	else
	{
		cout << format("ptr_02 is null\n\n");
	}

	*ptr_01 = "To bark is canine";
	*ptr_03 = "To purr, feline";

	cout << format("Contents of ptr_01: {}\n", *ptr_01);
	cout << format("Contents of ptr_03: {}\n\n", *ptr_03);

	ptr_01.reset();
	if (!ptr_01)
	{
		cout << "ptr_01 is null" << "\n";		// Will be displayed
	}

	// An equivalent result can be obtained by setting a unique pointer 
	// equal to `nullptr`, another keyword introduced in {cpp}11. 

	ptr_03 = nullptr;
	if (!ptr_03)
	{
		cout << "ptr_03 is null" << "\n";		// Will be displayed
	}
}


void shared_pointers()
{
	using std::cout;
	using std::format;
	using std::string;

	cout << "\n" << "*** shared_pointers() ***" << "\n\n";

	std::shared_ptr<std::string> sp_one;
	// . . .
	sp_one = std::make_shared<string>("To err is expected");

	// Single line
	auto sp_two =
		std::make_shared<string>("To nap is supine");

	cout << format("Contents of sp_one: {}\n", *sp_one);
	cout << format("Contents of sp_two: {}\n\n", *sp_two);

	// Reference counting:
	cout << format("How many pointers are there to sp_one? {}, {}\n",
		sp_one.use_count(), typeid(sp_one.use_count()).name());		// count = 1

	auto sp_one_2 = sp_one;
	auto sp_one_3 = sp_one_2;

	cout << format("How many pointers are there to sp_one? {}\n",
		sp_one.use_count());		// count = 3

	sp_one_2.reset();

	cout << format("How many pointers are there now (sp_one_2.reset() called) to sp_one? {}\n\n",
		sp_one.use_count());		// count = 2
}
