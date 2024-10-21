/*
	A portion of this sample code is based on sample code provided on CD
	to accompany the book _C++ Design Patterns and Derivatives Pricing (2E)_,
	by Mark Joshi, 2008.

	Its accompanying license agreement is as follows:

	The code is released under an artistic license. This means that you
	can do what you like with it, provided that if you redistribute the
	source code you allow the receiver to do what they like with it too.

	Otherwise, this file is licensed under the Mozilla Public License, v. 2.0.
	You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once
#include <memory>

// The first Payoff/CallPayoff/PutPayoff versions use an '03' suffix 
// in the sample code to avoid conflict with the modernized versions
// that follow later.  '03' refers to C++03, before unique_ptr(s) 
// and move semantics were part of the C++ Standard.  These use RAII
// in a C++03 context, using "virtual constructors" similar to those 
// described in the Joshi (2008) text.  This "03" and Chapter "03"
// are coincidental.

class Payoff03		// Implemented as class Payoff03 in sample code.
{
public:
	virtual double payoff(double price) const = 0;
	virtual Payoff03* clone() const = 0;		// New (pure virtual) member function	
	virtual ~Payoff03() = default;
};

class CallPayoff03 : public Payoff03
{
public:
	CallPayoff03(double strike);
	double payoff(double price) const override;
	CallPayoff03* clone() const override;		// New overriding member function				


private:
	double strike_;
};


class PutPayoff03 : public Payoff03
{
public:
	PutPayoff03(double strike);
	double payoff(double price) const override;
	PutPayoff03* clone() const override;		// New overriding member function				


private:
	double strike_;
};



// Payoff (C++11/C++14) -- Similar to Joshi 2E (2008), but will replace 
// raw pointer resource on OptionInfo with unique pointer member.

class Payoff
{
public:
	virtual double payoff(double price) const = 0;
	virtual std::unique_ptr<Payoff> clone() const = 0;	
	virtual ~Payoff() = default;
};

class CallPayoff final : public Payoff
{
public:
	CallPayoff(double strike);
	double payoff(double price) const override;
	std::unique_ptr<Payoff> clone() const override;	// clone() here returns a 
                                                    // unique_ptr<Payoff>,
                                                    // not unique_ptr<CallPayoff>
													

private:
	double strike_;
};

class PutPayoff final : public Payoff
{
public:
	PutPayoff(double strike);
	//double operator()(double spot) const override;
	double payoff(double price) const override;
	std::unique_ptr<Payoff> clone() const override;	// clone() here returns a 
                                                    // unique_ptr<Payoff>,
													// not unique_ptr<PutPayoff>

private:
	double strike_;
};