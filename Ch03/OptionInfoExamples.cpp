/*
	This file is licensed under the Mozilla Public License, v. 2.0.
	You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
*/

#include "Ch03_example_functions.h"
#include "OptionInfo.h"
#include "Ch03_example_functions.h"
#include <utility>		// std::move, std::swap
#include <stdexcept>
#include <iostream>
#include <format>

#include <string>

void option_info_examples()
{
	cpp03_examples();
	cpp11_examples();
}

void cpp03_examples()
{
	using std::cout;
	using std::format;

	cout << "\n" << "*** cpp03_examples() (RAII) ***" << "\n";

	CallPayoff03 call_payoff(75.0);
	OptionInfo03 call_opt(call_payoff, 0.5);
	cout << "\n" << format("Payoff for call option with {} year(s) to expiration = {}",
		call_opt.time_to_expiration(), call_opt.option_payoff(85.0)) << "\n";

//#if 0
	// Copy examples (comment out the following `#if 0` block 
	// if copy operations declared private):
	OptionInfo03 copy{call_opt};
	OptionInfo03 copy_assgn = call_opt;

	cout << "\n" << format("Payoff for call option (copy ctor) with {} year(s) to expiration = {}",
		copy.time_to_expiration(), copy.option_payoff(90.0)) << "\n";

	cout << "\n" << format("Payoff for call option (copy assgn) with {} year(s) to expiration = {}",
		copy_assgn.time_to_expiration(), copy_assgn.option_payoff(95.0)) << "\n\n";
//#endif
}


void cpp11_examples()
{
	using std::cout;
	using std::format;

	cout << "\n" << "*** cpp11_examples() (RAII) ***" << "\n";

	auto call_payoff = std::make_unique<CallPayoff>(75.0);
	OptionInfo call_opt{std::move(call_payoff), 0.5};

	// Alternatively, this could be written in one line with an anonymous temporary unique pointer:
	//OptionInfo call_opt{std::make_unique<CallPayoff>(75.0), 0.5};

	cout << "\n" << format("Payoff for call option with {} year(s) to expiration = {}",
		call_opt.time_to_expiration(), call_opt.option_payoff(85.0)) << "\n";

	// Need copy operations implemented on OptionInfo to construct
	// call_copy and call_copy_assgn; otherwise, comment out the following `#if 0` block.
//#if 0
	OptionInfo call_copy{call_opt};		// Need copy operations implemented on OptionInfo
	cout << "\n" << format("Payoff for _copied_ call option with {} year(s) to expiration = {}",
		call_copy.time_to_expiration(), call_copy.option_payoff(90.0)) << "\n";

	OptionInfo call_copy_assgn{call_copy};
	call_copy_assgn = call_opt;
//#endif

	// Moves, however, are provided by default (but be sure to heed the
	// Rule of Zero or Rule of Five):
	OptionInfo call_move{std::move(call_opt)};
	cout << "\n" << format("Payoff for _moved_ (ctor) call option with {} year(s) to expiration = {}",
		call_move.time_to_expiration(), call_move.option_payoff(100.0)) << "\n";

	OptionInfo call_move_assgn{std::make_unique<CallPayoff>(75.0), 0.5};
	call_move_assgn = std::move(call_move);
	cout << "\n" << format("Payoff for _moved_ (=) call option with {} year(s) to expiration = {}",
		call_move_assgn.time_to_expiration(), call_move_assgn.option_payoff(100.0)) << "\n\n";
}