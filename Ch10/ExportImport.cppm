/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

export module ExportImportBusiness;
export import VandelayIndustries;
import <string>;
import <iostream>;
import <format>;

export void order(const std::string& order_code, const Latex& latex)
{
	std::cout 
		<< std::format("Order: {}, Total Price: {}", order_code, latex.total_sale_value()) << "\n";
}