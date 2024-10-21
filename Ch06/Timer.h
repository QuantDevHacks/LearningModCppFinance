/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <chrono>

// Timer class
//template <typename T = >
class Timer
{
	using clock = std::chrono::steady_clock;
	using millisec = std::chrono::duration<double, std::milli >;
	using sec = std::chrono::duration<double>;		// seconds

public:
	Timer() {}
	void start() { start_ = clock::now(); }
	void stop() { stop_ = clock::now(); }
	double milliseconds() const
	{
		return std::chrono::duration_cast<millisec>(stop_ - start_).count();
	}

	double seconds() const
	{
		return std::chrono::duration_cast<sec>(stop_ - start_).count();
	}

private:
	std::chrono::time_point<clock> start_, stop_;

};
