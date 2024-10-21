// This file is licensed under the Mozilla Public License, v. 2.0.
// You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.

#include "Ch04_example_functions.h"
#include "BankAccount.h"
#include "BlackScholes.h"
#include "Payoffs.h"
#include "UserDefinedTemplates.h"
#include "Fraction.h"

#include <vector>
#include <utility>
#include <functional>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <format>

void std_vectors()			// Top calling function
{
	push_back_example();
	emplace_back_example();
	polymorphic_elements();
	reserve_and_capacity();
	clear_elements();
	front_back_and_pop();
	out_of_range();
	round_brackets_and_braces();
}

// Used in clear_elements() example:
class MakeItClear
{
public:

	~MakeItClear()
	{
		std::cout << "MakeItClear destructor called" << "\n";
	}
};


void push_back_example()
{
	using std::vector, std::cout, std::format;

	cout << "\n*** push_back_example()  ***\n";

	vector<BankAccount> ba_push;
	ba_push.reserve(5);

	// Use this example in later function to demonstrate capacity:
	cout << format("Bank account vector contains {} elements, and capacity = {}.", 
		ba_push.size(), ba_push.capacity()) << "\n\n";		// size = 0, capacity = 5

	// Match book example:
	cout << format("ba_push contains {} elements.\n\n", ba_push.size());		// size = 0	

	BankAccount ba_01{1000.00, 0.021};
	BankAccount ba_02{2000.00, 0.022};
	BankAccount ba_03{3000.00, 0.023};

	ba_push.push_back(ba_01);
	ba_push.push_back(ba_02);
	ba_push.push_back(ba_03);

	cout << format("After insertions, bank account vector contains {} elements.\n\n",
		ba_push.size());		// size = 0

	for (const auto& ba : ba_push)
	{
		// Format to two decimal places:
		cout  << "Accumulated amount after 1 year = " 
			<< std::setprecision(2) << std::fixed << ba.value(1.0) << "\n";
	}

	cout << "\n\n";
}

void emplace_back_example()
{
	using std::vector, std::cout, std::format;

	cout << "\n*** emplace_back_example()  ***\n";

	vector<BankAccount> ba_emplace;

	ba_emplace.emplace_back(1000.00, 0.021);
	ba_emplace.emplace_back(2000.00, 0.022);
	ba_emplace.emplace_back(3000.00, 0.023);

	for (const auto& ba : ba_emplace)
	{
		// Format to two decimal places:
		cout << "Accumulated amount after 1 year = "
			<< std::setprecision(2) << std::fixed << ba.value(1.0) << "\n";
	}

	cout << "\n\n";
}

void polymorphic_elements()
{
	using std::vector, std::cout, std::format;

	cout << "\n*** polymorphic_elements()  ***\n";

	vector<std::unique_ptr<Payoff>> payoffs;
	payoffs.reserve(10);

	payoffs.push_back(std::make_unique<CallPayoff>(90.0));
	payoffs.push_back(std::make_unique<CallPayoff>(95.0));
	payoffs.push_back(std::make_unique<CallPayoff>(100.0));
	payoffs.push_back(std::make_unique<PutPayoff>(100.0));
	payoffs.push_back(std::make_unique<PutPayoff>(105.0));
	payoffs.push_back(std::make_unique<PutPayoff>(110.0));


	double spot = 88.25;
	for (auto&& p : payoffs)
	{
		cout << format("Payoff = {}", p->payoff(spot)) << "\n";
	}

	cout << "\n\n";

	double time_to_exp = 0.25;

	cout << "\n\n";

	// As opposed to raw pointers, unique pointers clean up after themselves,
	// so we don't have to iterate through the vector and call `delete` 
	// on each element.  Safer, and more convenient.

}

void reserve_and_capacity()
{
	using std::vector, std::cout, std::format;

	cout << "\n*** reserve_and_capacity()  ***\n";

	// Before applying push_back(.):
	vector<BankAccount> ba_push;
	ba_push.reserve(5);				// Space is reserved for 5 elements in memory,
	// but the size at this point is still zero.

// Same as before:
	BankAccount ba_01{1000.00, 0.021};
	BankAccount ba_02{2000.00, 0.022};
	BankAccount ba_03{3000.00, 0.023};

	ba_push.push_back(ba_01);
	ba_push.push_back(ba_02);
	ba_push.push_back(ba_03);		// ba_push.size() now = 3, 
									// but ba_push.capacity() still = 5.
	cout << format("Size of vector (push) = {}, capacity = {}, remaining capacity = {}\n",
		ba_push.size(), ba_push.capacity(), ba_push.capacity() - ba_push.size());

// Similarly for emplace_back():
	vector<BankAccount> ba_emplace;
	ba_emplace.reserve(5);

	ba_emplace.emplace_back(1000.00, 0.021);
	ba_emplace.emplace_back(2000.00, 0.022);
	ba_emplace.emplace_back(3000.00, 0.023);
	cout << format("Size of vector (emplace) = {}, capacity = {}, remaining capacity = {}\n\n",
		ba_emplace.size(), ba_emplace.capacity(), ba_emplace.capacity() - ba_emplace.size());

}

void clear_elements()
{
	using std::vector, std::cout, std::format;

	cout << "\n*** clear_elements()  ***\n";

	vector<MakeItClear> mic;
	mic.reserve(3);
	mic.emplace_back();	// Emplace MakeItClear object 
	mic.emplace_back();	// using default constructor – no argument.
	mic.emplace_back();

	cout << format("Size of vector<MakeItClear> = {}, capacity = {}\n\n",
		mic.size(), mic.capacity());

	// Now, clear the elements:
	mic.clear();
	cout << format("\nSize of vector<MakeItClear> now = {}, capacity = {}\n\n", 
		mic.size(), mic.capacity());

	cout << "\n\n";
}

void front_back_and_pop()
{
	using std::vector, std::cout, std::format;

	cout << "\n*** front_back_and_pop()  ***\n";

	cout << "Test front() on a vector:\n";
	vector<int> ints{1, 2, 3, 4, 5};
	for (int i : ints) cout << i << " ";
	cout << "\n\n";

	cout << "Test front() on a vector after reassigning saved front value:\n";
	int &the_front = ints.front();
	the_front = 10;
	for (int i : ints) cout << i << " ";
	cout << "\n\n";


	vector<MyPair<int>> pairs;
	pairs.reserve(4);

	pairs.emplace_back(1, 2);
	pairs.emplace_back(1, -2);
	pairs.emplace_back(3, 4);
	pairs.emplace_back(3, -4);

	int front_min = pairs.front().get_min();
	int back_min = pairs.back().get_min();

	cout << format("Min elem of front pair = {}\n", front_min);
	cout << format("Min elem of back pair = {}\n", back_min);

	pairs.pop_back();
	back_min = pairs.back().get_min();
	cout << format("Min elem of back pair now = {}\n", back_min);

	pairs.front() = {10, 6};
	front_min = pairs.front().get_min();
	cout << format("Min elem of front pair now = {}\n\n", front_min);

}

void out_of_range()
{
	using std::vector, std::cout;

	cout << "\n*** out_of_range()  ***\n";

	vector<int> v = {0, 1, 2, 3, 4, 5};	// v.size() == 5

	try
	{
		int n = v.at(10);	// at(.) throws out_of_range exception
	}
	catch (const std::out_of_range& e)
	{
		std::cout << e.what() << "\n\n";
	}
}

void round_brackets_and_braces()
{
	using std::vector, std::cout, std::format;

	cout << "\n*** round_brackets_and_braces()  ***\n";

	// Either results in the same fraction (1/2):
	Fraction braces{1, 2};
	Fraction round_brackets(1, 2);
	cout << format("Fraction braces = {}/{}, Fraction round_brackets = {}/{}\n\n",
		braces.numer(), braces.denom(), round_brackets.numer(), round_brackets.denom());

	std::vector<int> v{10};				// size() == 1
	vector<int> v_round_brackets(10);	// size() == 10 

	for (int& i : v_round_brackets)
	{
		int k = 0;
		i = k;
		++k;
	}

	cout << "Contents of vector `v` (one element = 10): \n";
	for (int i : v)
	{
		cout << i << " ";
	}

	cout << "\n\n";

	cout << "Contents of vector `v_round_brackets` (10 elements): \n";
	for (int i : v_round_brackets)
	{
		cout << i << " ";
	}

	cout << "\n\n";
}


