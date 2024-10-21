// This file is licensed under the Mozilla Public License, v. 2.0.
// You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.

#include "Ch04_example_functions.h"
#include "Shapes.h"

#include <vector>		// Move to iterators
#include <deque>
#include <list>
#include <array>
#include <map>			// Move to iterators
#include <set>		// Move to iterators
#include <unordered_map>		// Move to iterators

#include <iostream>
#include <format>

void other_sequential_stl_containers()
{
	deque_examples();
	list_examples();
	std_array_examples();
}

void deque_examples()
{
	using std::cout;
	cout << "\n*** deque_examples()  ***\n";

	std::deque<int> on_deque{0, 1, 2, 3};

	// Push new elements onto the front:
	on_deque.push_front(-1);
	on_deque.push_front(-2);

	// Can also push onto the back like a vector;
	on_deque.push_back(4);
	for (int k : on_deque)
	{
		cout << k << ' '; // on_deque now contains: -2 -1 0 1 2 3 4 
	}

	cout << "\n\n";

	// Remove both 1st and last element:
	on_deque.pop_front();
	on_deque.pop_back();
	for (int k : on_deque)
	{
		cout << k << ' '; // on_deque now contains: -1 0 1 2 3
	}

	cout << "\n\n";

	std::deque<Rectangle> recs;
	recs.emplace_front(3.0, 2.0);
	recs.emplace_front(4.0, 3.0);
	recs.emplace_front(5.0, 4.0);

	for (const auto& elem : recs)
	{
		cout << elem.area() << " ";		// Output: 20 12 6
	}

	cout << "\n\n";

}

void list_examples()
{
	using std::cout;
	cout << "\n*** list_examples()  ***\n";

	std::list<int> franz{0, 1, 2, 3};
	for (int elem : franz)
	{
		cout << elem << " ";		// 0 1 2 3
	}

	cout << "\n\n";

	franz.push_front(-1);
	franz.push_back(4);
	franz.push_back(5);
	franz.pop_back();

	for (int elem : franz)
	{
		cout << elem << " ";		// -1 0 1 2 3 4
	}

	//	int sum = franz.at(0) + franz[1];	// Compiler error!

	cout << "\n\n";
}

void std_array_examples()
{
	using std::cout, std::format;
	cout << "\n*** std_array_examples()  ***\n";

	int arr_01[]{0, 1, 2, 3};
	int sum = 0;
	for (int x : arr_01)
	{
		sum += x;
	}

	cout << format("sum (C array) = {}\n", sum);		// = 6

	// note : we could also have written std::array arr_02{0, 1, 2, 3} since C++17 (CTAD)
	std::array<int, 4> arr_02{0, 1, 2, 3};
	sum = 0;
	for (int elem : arr_02)
	{
		sum += elem;
	}

	cout << format("sum (std::array) = {}\n", sum);		// = 6

	cout << "\n\n";
}


