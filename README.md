# LearningModCppFinance
Example source code, notes, and errata for _Learning Modern C++ for Finance_, by Daniel Hanson (O'Reilly)

## Errata
The following are known typos and errors as of 2025.06.04 and have been submitted to O'Reilly for corrections:
1. From the acknowledgments section in the preface, "C++23" and "C++26" got changed to "C23" and "C26", plus both are in fixed-width font.  These should read as "C++23" and "C++26" and should be in regular font.

2. Pages 256 and 258 of the print edition:  The code line: 
```
   double t1 = act365_(settle_date(), d1);
```
   should actually be:  
```
   double t1 = act_365().year_fraction(settle_date(), date_01);`
```   

4. Page 269, in cue ball (2), need to remove the repeated word, "presented".

5. Page 79, the sample code line
```
   OptionInfo(OptionInfo& rhs) = delete;

```
   requires an additional ampersand: 

```    
    OptionInfo(OptionInfo&& rhs) = delete;

```
   as it is a move constructor.

5. Page 108, under the class declaration in line 2, the constructor should instead be written as follows (remove the `const` and `&` from the parameter list); viz, it should be:  `MyPair(T first, T second);`

6. Page 132, the implementation of the standard normal pdf $N'(x)$ is incorrect in the book but correct in the sample code on GitHub.  The lambda function at the bottom of the page should read:

```
	// N'(x): Standard Normal pdf:
	auto norm_pdf = [](double x) -> double
	{
		using namespace std::numbers;
		return (inv_sqrtpi / sqrt2) * exp(-x * x/2.0);
	};

```
7. Page 133, 

```
  auto corp = corp = PayoffType::Put;

```

should be changed to
     
```
  auto corp = PayoffType::Put;

```

8. Page 272, in the 1st full sentence,

    "In constructing a `Bond` object, we need to supply the face value, dated date, first coupon date, penultimate payment date, and maturity date of the bond, along with its face value."

    should be replaced by

    "In constructing a `Bond` object, we need to supply the face value, dated date, first coupon date, penultimate payment date, and the maturity date of the bond."

9. Page 368, the first instance of "Note that the export keyword is not used in the implementation unit" should be removed(duplicated).  The 2nd instance, followed by a clause, should remain:

    "Note that the export keyword is _not_ used in the implementation unit; it can be placed only in a module interface unit."

10. Page 342, 3rd line from bottom, the comment line

```
// change name calculate_discounted_expected_payoffs

```
   should be removed from the code example.

11. Page 341, "At a high level, the labor is then divided between two private member functions that project the underlying prices from the initial node out to expiration (`project_prices_()`)... "

    should instead read:
    "At a high level, the labor is then divided between two private member functions that project the underlying prices from the initial node out to expiration (`project_underlying_prices_()`)... ".

12. Page 310, The code block:

```
for (unsigned j = 1; j < integ_scens.cols(); ++j)
{
    for (unsigned i = 0; i < integ_scens.rows(); ++i)
    {
        integ_scens(i, j) = gen_price(integ_scens(i, j - 1), cov_basket(i, i),
            corr_norms(i, j - 1));
    }
}

```
should be replaced by:

```
for (unsigned j = 1; j < corr_scens.cols(); ++j)
{
	for (unsigned i = 0; i < corr_scens.rows(); ++i)
	{
		corr_scens(i, j) = gen_price(corr_scens(i, j - 1), 
			cov_basket(i, i), corr_scens(i, j));
	}
}

```
13. Page 302, The 2nd comment in the 2nd code example on p 302 should be changed from

```
  // and save the results back to the vals matrix object (MatrixXd):
```

to

```
// and save the results back to the vals_01 matrix object (MatrixXd):
```

## External Libraries
Zip files containing the Boost header files, the Eigen library, and the reference implementations of mdspan, submdspan, stdBLAS, and mdarray have been uploaded in respective folders in the repository.  The Boost version is 1.78.0, and the code will compile and run with this version, but there is a later version available on Boost.org.  You can also get the Eigen library from their website https://eigen.tuxfamily.org/index.php?title=Main_Page if you wish, which may contain subsequent updates.  Finally, mdspan, stdBLAS et al are in the stdBLAS folder.  This will work with the sample code in the book, but there may also be update in the Kokkos GitHub repository cited in the book.  These are included here for convenience, but if you want the more recent versions, please refer to the respective websites/repositories. 
