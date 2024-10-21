/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once

enum OptionType		
{
	European,     	// default integer value = 0
	American,     	// default integer value = 1
	Bermudan,     	// default integer value = 2
	Asian	      	// default integer value = 3
};

enum Baseball
{
	Pitcher,		// 0
	Catcher,		// 1
	First_Baseman,	// 2
	Second_Baseman,	// 3
	Third_Baseman,	// 4
	Shortstop,		// 5
	Left_Field,    	// 6
	Center_Field,	// 7
	Right_Field		// 8
};

enum Football
{
	Defensive_Tackle,	// 0
	Edge_Rusher,		// 1
	Defensive_End,		// 2
	Linebacker,			// 3
	Cornerback,			// 4
	Strong_Safety,		// 5
	Weak_Safety			// 6
};

enum class Bond_Type
{
	Government,
	Corporate,
	Municipal,
	Convertible
};

enum class Futures_Contract
{
	Gold,
	Silver,
	Oil,
	Natural_Gas,
	Wheat,
	Corn
};
