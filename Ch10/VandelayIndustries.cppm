/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

export module VandelayIndustries;

export class Latex
{
public:
	Latex(int quantity, double unit_price) :
		quantity_{quantity}, unit_price_{unit_price} {}

	double total_sale_value() const
	{
		return quantity_ * unit_price_;
	}

private:
	int quantity_;
	double unit_price_;
};