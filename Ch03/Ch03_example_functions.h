/*
 * This file is licensed under the Mozilla Public License, v. 2.0.
 * You can obtain a copy of the license at http://mozilla.org/MPL/2.0/.
 */

#pragma once

// SimpleClass 
class SimpleClass
{
public:
    SimpleClass(int k);
    int get_val() const;
    void reset_val(int k);

private:
    int k_;
};

void shapes_examples();	// This function is not in the book but supplied to show examples.

void smart_pointers();      // Top level calling function
void unique_pointers();
void shared_pointers();

void option_info_examples();	// Top level calling function
void cpp03_examples();
void cpp11_examples();