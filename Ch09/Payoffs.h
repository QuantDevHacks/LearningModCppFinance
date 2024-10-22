#pragma once
#include <memory>

// Payoff (C++11/C++14) -- from Ch 3

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
	std::unique_ptr<Payoff> clone() const override;		// clone() now returns a unique_ptr<Payoff>,
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
	std::unique_ptr<Payoff> clone() const override;		// clone() now returns a unique_ptr<Payoff>,
														// not unique_ptr<PutPayoff>

private:
	double strike_;
};