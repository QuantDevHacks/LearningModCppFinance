/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "Payoffs.h"
#include <memory>

class OptionInfo
{
public:
	OptionInfo(std::unique_ptr<Payoff> payoff, double time_to_exp);
	double option_payoff(double spot) const;
	double time_to_expiration() const;
	void swap(OptionInfo& rhs) noexcept;

	OptionInfo(const OptionInfo& rhs);
	OptionInfo& operator =(const OptionInfo& rhs) noexcept;

	// Remaining Rule of Five:
	OptionInfo(OptionInfo&& rhs) = default;					// Default move constructor
	OptionInfo& operator =(OptionInfo&& rhs) = default;		// Default move assignment

	~OptionInfo() = default;								// Default destructor

private:
	std::unique_ptr<Payoff> payoff_ptr_;
	double time_to_exp_;
};