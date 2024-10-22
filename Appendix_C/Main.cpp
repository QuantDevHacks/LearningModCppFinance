/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#include "OptionInfo.h"

#include <utility>
#include <format>
#include <iostream>

using std::cout, std::format;

// Called from main():
void std_exchange_examples();
void opt_info_move_examples();

// Appendix C is supplemental to Ch 3

int main()
{
	std_exchange_examples();
	opt_info_move_examples();
}

void std_exchange_examples()
{
	using std::cout;
	using std::format;

	cout << "\n*** std_exchange_examples() ***\n";

	int a = 1;
	cout << format("\nStart: a = {}\n", a);

	int b = std::exchange(a, 500);
	cout << format("\nNow: : a = {}, b = {}\n", a, b);

	auto put_payoff = std::make_unique<PutPayoff>(55.0);
	auto exch_payoff{std::exchange(put_payoff, nullptr)};
	if (!put_payoff)
	{
		cout << format("\nPayoff at spot = 51.5: {}\n\n", exch_payoff->payoff(51.5));
	}
}


void opt_info_move_examples()
{
	using std::cout;
	using std::format;

	cout << "\n" << "*** opt_info_move_examples() ***" << "\n";

	OptionInfo call_opt{std::make_unique<CallPayoff>(75.0), 0.5};

	cout << "\n" << format("Payoff for call option with {} year(s) to expiration = {}",
		call_opt.time_to_expiration(), call_opt.option_payoff(85.0)) << "\n";

	// Use the implemented move constructor and move assignment operator:
	OptionInfo call_move{std::move(call_opt)};
	cout << "\n" << format("Payoff for _moved_ (ctor) call option with {} year(s) to expiration = {}",
		call_move.time_to_expiration(), call_move.option_payoff(95.0)) << "\n";

	OptionInfo call_move_assgn{std::make_unique<CallPayoff>(75.0), 0.5};
	call_move_assgn = std::move(call_move);
	cout << "\n" << format("Payoff for _moved_ (=) call option with {} year(s) to expiration = {}",
		call_move_assgn.time_to_expiration(), call_move_assgn.option_payoff(106.0)) << "\n\n";
}