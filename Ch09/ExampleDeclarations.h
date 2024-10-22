/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once

// BoostMathConstExamples.cpp
void boost_math_const_examples();

// BoostMathConstExamples.cpp
void statistical_distributions();		// Top level calling function
void density_and_distribution_fcns();
void drawdown_revisited();
void non_central_t();

// BoostMultiArray.cpp
void multi_array();						// Top level calling function
void simple_multi_array();
void amer_itm_put();	// No dividend
void lattice_pricing_convergence();

// Accumulators.cpp
void accumulator_examples();		// Top level calling function
void min_max_accumulator();
void mean_and_var_accumulator();
void intro_rolling_windows();

// n = num of std dev's, obs = num observations, win_size = num obs in rolling window
void bollinger_bands(double n, unsigned obs, unsigned win_size);
void ma_cross(unsigned fast_ma_win, unsigned slow_ma_win, unsigned obs);