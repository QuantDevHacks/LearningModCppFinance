#include "ExampleDeclarations.h"
#include "ExpressionTemplates.h"

#include <vector>
#include <utility>      // std::move
#include <random>
#include <cstdlib>
#include <format>
#include <deque>
#include <set>


void expression_template_examples()
{ 
    add_four_real_vectors_exp_tmpl();
    vector_conversion_operator(); 
}

void add_four_real_vectors_exp_tmpl()
{
    using std::vector; 
    using std::cout;

    cout << "*** add_four_real_vectors() ***" << "\n";

    vector<double> v_01{1.0, 2.0, 3.0};
    vector<double> v_02{1.5, 2.5, 3.5};
    vector<double> v_03{4.0, 5.0, 6.0};
    vector<double> v_04{4.5, 5.5, 6.5};

    auto result = v_01 + v_02 + v_03 + v_04;
    cout << "typeof(result) = " << typeid(result).name() << "\n\n";

    vector<double> y{result[0], result[1], result[2]};
    for (double x : y)
    {
        cout << x << " ";
    }
    cout << "\n\n"; 

    // Note that this will work for an arbitrary
    // number of vector addends:
    auto result_add_three = v_02 + v_03 + v_04;

    vector<double> v_05{2.0, 3.0, 4.0};
    vector<double> v_06{3.5, 4.5, 5.5};

    auto result_add_six = v_01 + v_02 + v_03 + v_04 + v_05 + v_06;

    vector<double> y_add_three{result_add_three[0], result_add_three[1], result_add_three[2]};
    vector<double> y_add_six{result_add_six[0], result_add_six[1], result_add_six[2]};

    for (double x : y_add_three)
    {
        cout << x << " ";
    }
    cout << "\n\n";

    for (double x : y_add_six)
    {
        cout << x << " ";
    }
    cout << "\n\n";
}

void vector_conversion_operator()
{
    using std::vector;
    using std::cout;
    cout << "*** vector_conversion_operator() ***" << "\n";

    vector<double> v_01{1.0, 2.0, 3.0};
    vector<double> v_02{1.5, 2.5, 3.5};
    vector<double> v_03{4.0, 5.0, 6.0};
    vector<double> v_04{4.5, 5.5, 6.5};

    vector<double> vec_result = v_01 + v_02 + v_03 + v_04;

    cout << "typeof(vec_result) = " << typeid(vec_result).name() << "\n\n";
    for (double x : vec_result)
    {
        cout << x << " ";
    }
    cout << "\n\n";
}

