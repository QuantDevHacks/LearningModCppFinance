#include "Declarations.h"
#include "EnumsAndEnumClasses.h"

#include <format>
#include <iostream>

using std::format;
using std::cout;

void enum_examples()
{
	cout << "\n***** enum_examples() ***** (top calling function)\n";

	enum_const_examples();
	enum_class_examples();
	switch_statement_scoped_enum(Bond_Type::Corporate);
}

void enum_const_examples()
{
	using std::cout, std::format;
	cout << "\n" << "*** enum_const_examples() ***" << "\n\n";

	// Simple introductory example:

	cout << " European = " << European << "\n";
	cout << " American = " << American << "\n";
	cout << " Bermudan = " << Bermudan << "\n";
	cout << " Asian = " << Asian << "\n";
	cout << " American + Asian = " << American + Asian << "\n\n";

	if (Defensive_End == First_Baseman)
	{
		cout << " Defensive_End == First_Baseman" << "\n";
	}
	else
	{
		cout << " Defensive_End != First_Baseman" << "\n";
	}

}

void enum_class_examples()
{
	cout << "\n" << "*** enum_class_examples() (scoped enums) ***" << "\n";

	// This will be caught by the compiler (a good thing!)
	//if (Bond_Type::Corporate == Futures_Contract::Silver)
	//{
	//	// . . .
	//}

	cout << format("Corporate Bond index: {}", static_cast<int>(Bond_Type::Corporate)) << "\n";				// Num 1
	cout << format("Natural Gas Futures index: {}", 
		static_cast<int>(Futures_Contract::Silver)) << "\n";
}

void switch_statement_scoped_enum(Bond_Type bnd)
{
	std::cout << "\n" << "*** switch_statement_scoped_enum ***" << "\n";

	switch (bnd)
	{
	case Bond_Type::Government:
		std::cout << "Government Bond..." << "\n";
		// Do stuff...
		break;
	case Bond_Type::Corporate:
		cout << "Corporate Bond..." << "\n";
		// Do stuff...
		break;
	case Bond_Type::Municipal:
		cout << "Municipal Bond..." << "\n";
		// Do stuff...
		break;
	case Bond_Type::Convertible:
		cout << "Convertible Bond..." << "\n";
		// Do stuff...
		break;
	default:
		cout << "Unknown Bond..." << "\n";
		// Check the bond type...
		break;
	}
}
