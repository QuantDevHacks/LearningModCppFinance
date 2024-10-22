#include "ExampleDeclarations.h"

// Note:  Might not need all of these declarations (and same for EigenStlExamples.cpp).
// Used to be in same file.  TO DO:  Tidy up!

#include <Eigen/Dense>
#include <cmath>
#include <complex>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <functional>
#include <random>
#include <vector>
#include <utility>		// std::move
#include <format>
#include <iostream>		
#include <iomanip>		// std::setprecision

using std::vector;
using std::cout;
using std::setprecision;

using Eigen::MatrixXd;
using Eigen::VectorXd;

void eigen_decomposition_examples()
{
	qr_linear_regression();
	cholesky_correlated_random_equity_paths();
	principal_components_mtx();

	// Extra examples not in book:
	solve_linear_system();
	jacobi_svd_linear_regression();
	linear_regression_with_intercept();
}

void qr_linear_regression()
{
	cout << "\n" << "*** qr_linear_regression() ***" << "\n";
	// To solve the least squares problem, Eigen says give me the X matrix and the
	// y vector, and I’ll calculate the beta(hat) coefficients:

	MatrixXd X
	{
		{-0.044700, -0.007888,  0.042980,  0.016417, -0.017797, -0.016714, 
		  0.019472,  0.029853,  0.023126, -0.033879, -0.003384, -0.018474, 
		 -0.012510, -0.018348,  0.010627,  0.036669,  0.010811, -0.035572, 
		  0.027474,  0.005406, -0.010159, -0.006146, -0.010327, -0.010435, 
		  0.011127, -0.023794, -0.028009,  0.002182,  0.008683,  0.001440},

		{-0.019003,  0.026037,  0.037827,  0.010629, -0.008382,  0.001122, 
		 -0.004494,  0.017305, -0.006106,  0.012175, -0.003305,  0.027220, 
		 -0.036089, -0.002230, -0.015748, -0.020619, -0.011641,  0.023149, 
		 -0.002291,  0.006288, -0.012038, -0.029259,  0.011219, -0.008847, 
		 -0.033738,  0.020619, -0.012078,  0.015673,  0.041013,  0.052195},

		{-0.030629,  0.024919, -0.001716,  0.008562,  0.003407, -0.010824, 
		 -0.010361, -0.009302,  0.008142, -0.004064,  0.000584,  0.004640, 
		  0.031893, -0.013544, -0.023574, -0.004665, -0.006446, -0.005311, 
		  0.045096, -0.007375, -0.005142, -0.001716, -0.005176, -0.002885, 
		  0.002309, -0.014522, -0.017712,  0.001192, -0.002382, -0.004396}
	};

	X.transposeInPlace();

	VectorXd Y
	{
		{-0.039891,  0.001787, -0.016202,  0.056452,  0.003425, -0.012038, 
		 -0.009998,  0.013452,  0.013486, -0.007898,  0.008111, -0.015425, 
		 -0.002161, -0.028752,  0.011293, -0.007958, -0.004002, -0.031691, 
		  0.026777,  0.009804,  0.000887,  0.014952, -0.004156, -0.001535, 
		  0.013517, -0.021229,  0.001989, -0.020518,  0.005841,  0.011249}
	};

	VectorXd beta = X.householderQr().solve(Y);

	cout << setprecision(6) 
		<< "The QR least-squares estimate of the regression coefficients (beta) is (solution of beta * X = Y):"
		<< "\n" << beta << "\n\n";
}

void cholesky_correlated_random_equity_paths()
{
	cout << "\n" << std::format("\n*** cholesky_correlated_random_equity_paths() ***") << "\n";

	MatrixXd cov_basket
	{
		{ 0.01263, 0.00025, -0.00017, 0.00503 },
		{ 0.00025, 0.00138,  0.00280, 0.00027 },
		{-0.00017, 0.00280,  0.03775, 0.00480 },
		{ 0.00503, 0.00027,  0.00480, 0.02900 }
	};

	Eigen::LLT<Eigen::MatrixXd> chol{cov_basket};
	MatrixXd chol_mtx = chol.matrixL();

	cout << "Cholesky Decomposition of cov_basket mtx: " << "\n";
	cout << setprecision(7) << chol_mtx << "\n\n";

	MatrixXd corr_scens{4, 7};
	corr_scens.col(0) << 100.0, 150.0, 25.0, 50.0;		// Input the spot prices.

	std::mt19937_64 mt_norm{100};	// Seed is arbitrary, just set to 100 for this example
	std::normal_distribution<> std_nd;

	auto std_norm = [&mt_norm, &std_nd](double x)
	{
		return std_nd(mt_norm);
	};

	// proj_vals_block: "projected values block"
	Eigen::Block<MatrixXd> proj_vals_block =
		corr_scens(Eigen::seq(0, Eigen::last), Eigen::seq(1, Eigen::last));
	cout << "Type of proj_vals_block = " << typeid(proj_vals_block).name() << "\n\n";

	for (auto col : proj_vals_block.colwise())
	{
		std::ranges::transform(col, col.begin(), std_norm);
	}

	cout << "\nMatrix of N(0, 1) Random Draws (integ_scens): " << "\n";
	cout << std::fixed << setprecision(6) << corr_scens << "\n\n";

	proj_vals_block = chol_mtx * proj_vals_block;

	cout << "\nMatrix of N(0, 1) Random Draws (integ_scens) with spots in 1st col: " << "\n";
	cout << std::fixed << setprecision(6) << corr_scens << "\n\n";

	double rf_rate = 0.01;
	double time_to_maturity = 1.0;
	unsigned num_time_steps = 6;
	double dt = time_to_maturity / num_time_steps;

	auto gen_price = [dt, rf_rate](double price, double vol, double corr_norm) 
	{
		double expArg1 = (rf_rate - ((vol * vol) / 2.0)) * dt;
		double expArg2 = corr_norm * std::sqrt(dt);
		double next_price = price * std::exp(expArg1 + expArg2);
		return next_price;
	};

	for (unsigned j = 1; j < corr_scens.cols(); ++j)
	{
		for (unsigned i = 0; i < corr_scens.rows(); ++i)
		{
			corr_scens(i, j) = gen_price(corr_scens(i, j - 1), 
				cov_basket(i, i), corr_scens(i, j));
		}
	}

	cout << "Generated Random Price Paths (row --> path): " << "\n";
	cout << std::fixed << setprecision(2) << corr_scens << "\n\n";
}

void principal_components_mtx()
{
	cout << "\n" << std::format("\n*** principal_components_mtx() ***") << "\n";

	MatrixXd term_struct_cov_mtx
	{
		// 1 month   
		{0.018920,	0.009889, 0.005820,	0.005103, 0.003813,	0.003626,
			0.003136, 0.002646, 0.002015, 0.001438, 0.001303},

		// 3 months   
		{0.0, 0.010107, 0.006123, 0.004796, 0.003532, 0.003414,
			0.002893, 0.002404, 0.001815, 0.001217, 0.001109},

		// 6 months
		{0.0, 0.0, 0.005665, 0.004677, 0.003808, 0.003790,
			0.003255, 0.002771, 0.002179, 0.001567, 0.001400},

		// 1 year   
		{0.0, 0.0, 0.0, 0.004830, 0.004695, 0.004672,
			0.004126, 0.003606, 0.002952, 0.002238, 0.002007},

		// 2 years   
		{0.0, 0.0, 0.0, 0.0, 0.006431, 0.006338,
			0.005789, 0.005162, 0.004337, 0.003343, 0.003004},

		// 3 years   
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.006524,
			0.005947, 0.005356, 0.004540, 0.003568, 0.003231},

		// 5 years   
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
			0.005800, 0.005291, 0.004552, 0.003669, 0.003352},

		// 7 years   
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
			0.0, 0.004985, 0.004346, 0.003572, 0.003288},

		// 10 years  
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
			0.0, 0.0, 0.003958, 0.003319, 0.003085},

		// 20 years 
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
			0.0, 0.0, 0.0, 0.003062, 0.002858},

		// 30 years 
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
			0.0, 0.0, 0.0, 0.0, 0.002814}
	};

	cout << "Original upper triangular covariance mtx :" << "\n";
	cout << std::fixed << setprecision(5) << term_struct_cov_mtx << "\n\n";

	// Eigenvalues are in general complex, but in this case only have real parts
	// (technically since covariance matrix is positive semidefinite):
	Eigen::VectorXcd cmplx_eigenvals = 
		term_struct_cov_mtx.selfadjointView<Eigen::Upper>().eigenvalues();
	cout << "VectorXcd: " << typeid(cmplx_eigenvals).name() << "\n";
	cout << "The (complex) eigenvalues of the term structure covariance matrix are:\n "
		/*<< std::fixed << setprecision(6)*/ << cmplx_eigenvals << "\n\n";

	VectorXd eigenvals = cmplx_eigenvals.real();

	// Alternatively, creating a temporary VectorXcd object could be 
	// eliminated by placing everything on one line:
	//VectorXd eigenvals = term_struct_cov_mtx.selfadjointView<Eigen::Upper>().eigenvalues().real();

	cout << "type of elem in eigenvals (real values): " << typeid(eigenvals).name() << "\n";
	cout << "Principal Components (Eigenvalues):" << "\n";
	for (auto ev : eigenvals)
	{
		cout << ev << " ";
	}
	cout << "\n\n";

	// Alternatively, try in one fell swoop and avoid the temporary MatrixXcd:
	VectorXd eigenvals_alt = term_struct_cov_mtx.selfadjointView<Eigen::Upper>().eigenvalues().real();

	cout << "Principal Components (Eigenvalues) -- Alt method:" << "\n";
	for (auto ev : eigenvals_alt)
	{
		cout << ev << " ";
	}
	cout << "\n\n";

	eigenvals = eigenvals_alt;		// Either results in the same PC values.

	eigenvals = eigenvals / eigenvals.sum();
	auto wgt_eigenvals = eigenvals;

	std::ranges::sort(eigenvals, std::greater{});

	cout << "Weighted Principal Components (now stored in sorted eigenvals):" << "\n";
	for (auto wv : eigenvals)
	{
		cout << std::fixed << setprecision(5) << wv << " ";
	}
	cout << "\n\n";

	cout << "Cumulative Principal Component Weightings:" << "\n";
	double cum_value{0.0};

	for (auto ev : eigenvals)
	{
		cum_value += ev;
		cout << cum_value << " ";
	}
	cout << "\n\n";
}

// The following are extra examples that are not in the book:
void solve_linear_system()
{
	cout << "*** solve_linear_system() (LU Decompositon) *** " << "\n\n";
	/*
		Basic linear solving

		The problem : You have a system of equations, that you have written as a single matrix equation

			Ax = b

		Where A and b are matrices(b could be a vector, as a special case).You want to find a solution x.

		The solution : You can choose between various decompositions, depending on what your matrix A looks like,
		and depending on whether you favor speed or accuracy.  However, let's use an example that
		is covered in most numerical analysis courses:  the LU decomposition method.
	*/

	MatrixXd A
	{
		{3.0,  -5.0, 1.0},
		{-1.0, -1.0, 1.0},
		{2.0,  -5.0, 1.0}
	};

	VectorXd b
	{
		{0.0, -4.0, 2.0}
	};

	cout << "Here is the matrix A:\n" << A << "\n\n";
	cout << "Here is the vector b:\n" << b << "\n\n";

	//Eigen::PartialPivLU<MatrixXd> lu(A);
	VectorXd x = Eigen::PartialPivLU<MatrixXd>{A}.solve(b);

	cout << "The solution using partial pivot LU is:\n" << x << "\n\n";

	Eigen::FullPivLU<MatrixXd> full_lu(A);
	x = full_lu.solve(b);

	cout << "The solution using full pivot LU is:\n" << x << "\n\n";

	// 

	// For more information, see: http://eigen.tuxfamily.org/dox/group__TutorialLinearAlgebra.html
	// but note that we need to put in the dimensions of the A matrix and b vector.  This is
	// not noted in this documentation.
}

void jacobi_svd_linear_regression()
{
	cout << "\n" << "*** jacobi_svd_linear_regression() ***" << "\n";
	// To solve the least squares problem, Eigen says give me the X matrix and the
	// y vector, and I’ll use SVD to calculate the beta(hat) coefficients:

	MatrixXd X{3, 30};	// 3 sector funds, 30 observations
	X <<
		-0.044700388, -0.007888394, 0.042980064, 0.016416586, -0.01779658, -0.016714149,
		0.019472031, 0.029853293, 0.023126097, -0.033879088, -0.00338369, -0.018474493,
		-0.012509815, -0.01834808, 0.010626754, 0.036669407, 0.010811115, -0.035571742,
		0.027474007, 0.005406069, -0.010159427, -0.006145632, -0.0103273, -0.010435171,
		0.011127197, -0.023793709, -0.028009362, 0.00218235, 0.008683152, 0.001440032,

		-0.019002703, 0.026036835, 0.03782709, 0.010629292, -0.008382267, 0.001121697,
		-0.004494407, 0.017304537, -0.006106293, 0.012174645, -0.003305029, 0.027219671,
		-0.036089287, -0.00222959, -0.015748493, -0.02061919, -0.011641386, 0.023148757,
		-0.002290732, 0.006288094, -0.012038397, -0.029258743, 0.011219297, -0.008846992,
		-0.033738048, 0.02061908, -0.012077677, 0.015672887, 0.041012907, 0.052195282,

		-0.030629136, 0.024918984, -0.001715798, 0.008561614, 0.003406931, -0.010823864,
		-0.010361097, -0.009302434, 0.008142014, -0.004064208, 0.000584335, 0.004640294,
		0.031893332, -0.013544321, -0.023573641, -0.004665085, -0.006446259, -0.005311412,
		0.045096308, -0.007374697, -0.00514201, -0.001715798, -0.005176363, -0.002884991,
		0.002309361, -0.014521608, -0.017711709, 0.001192088, -0.00238233, -0.004395918;

	VectorXd Y{30};	// 30 observations 
	Y <<
		-0.039891316, 0.00178709, -0.0162018, 0.056452057, 0.00342504, -0.012038314,
		-0.009997657, 0.013452043, 0.013485674, -0.007898137, 0.008111428, -0.015424523,
		-0.002161451, -0.028752191, 0.011292655, -0.007958389, -0.004002386, -0.031690771,
		0.026776892, 0.009803957, 0.000886608, 0.01495181, -0.004155781, -0.001535225,
		0.013517306, -0.021228542, 0.001988701, -0.02051788, 0.005841347, 0.011248933;

	X.transposeInPlace();

	// JacobiSVD<MatrixXd> defines the svd object.  From this, we can get the
	// U and V matrices, with the matrixU() and matrixV() member functions.
	Eigen::JacobiSVD<MatrixXd> svd{X, Eigen::ComputeThinU | Eigen::ComputeThinV};
	//Eigen::JacobiSVD<MatrixXd> svd(X, Eigen::ComputeFullU | Eigen::ComputeFullV);		// Optional, but not required

	// To solve the least squares problem, Eigen says give me the X matrix and the
	// y vector, and I’ll use SVD to calculate the beta(hat) coefficients:
	cout << "Y (response - S&P 500 rtns) vector:" << "\n" << Y << "\n" << "\n";
	VectorXd beta = svd.solve(Y);
	cout << "A least-squares estimate of the regression coefficients (beta) is (solution of beta * X = Y):"
		<< "\n" << beta << "\n\n";

	cout << "Supplemental results from the SVD: \n";
	cout << "The singular values are:" << "\n" << svd.singularValues() << "\n\n";

	cout << "The Sigma Mtx is the diagonal matrix containing the singular values: " << "\n";
	MatrixXd sigma = svd.singularValues().asDiagonal();
	cout << sigma << "\n\n";

	// n x p = 30 x 3
	cout << std::format("U: num cols = {}, num rows = {}", svd.matrixU().cols(), svd.matrixU().rows()) << "\n\n";
	cout << std::format("V: num cols = {}, num rows = {}", svd.matrixV().cols(), svd.matrixV().rows()) << "\n\n";

	cout << "U and V matrices, respectively:" << "\n";
	cout << svd.matrixU() << "\n\n";			// U: n x p = 30 x 3
	cout << svd.matrixV() << "\n\n";			// V: p x p = 3 x 3
}

// This is also an extra example that is not in the book.  It shows
// how to run a regression with an intercept term using Eigen.
// Note that the columns of the X matrix can be populated using
// data stored in Eigen::VectorXd types.
void linear_regression_with_intercept()
{
	cout << "\n" << "*** linear_regression_with_intercept() ***" << "\n";

	MatrixXd X{30, 4};
	X.col(0) = Eigen::VectorXd::Ones(30);

	X.col(1) = VectorXd
	{
		{
			-0.044700388, -0.007888394, 0.042980064, 0.016416586, -0.01779658, -0.016714149,
			0.019472031, 0.029853293, 0.023126097, -0.033879088, -0.00338369, -0.018474493,
			-0.012509815, -0.01834808, 0.010626754, 0.036669407, 0.010811115, -0.035571742,
			0.027474007, 0.005406069, -0.010159427, -0.006145632, -0.0103273, -0.010435171,
			0.011127197, -0.023793709, -0.028009362, 0.00218235, 0.008683152, 0.001440032
		}
	};

	X.col(2) = VectorXd
	{
		{
			-0.019002703, 0.026036835, 0.03782709, 0.010629292, -0.008382267, 0.001121697,
			-0.004494407, 0.017304537, -0.006106293, 0.012174645, -0.003305029, 0.027219671,
			-0.036089287, -0.00222959, -0.015748493, -0.02061919, -0.011641386, 0.023148757,
			-0.002290732, 0.006288094, -0.012038397, -0.029258743, 0.011219297, -0.008846992,
			-0.033738048, 0.02061908, -0.012077677, 0.015672887, 0.041012907, 0.052195282
		}
	};

	X.col(3) = VectorXd
	{
		{
			-0.030629136, 0.024918984, -0.001715798, 0.008561614, 0.003406931, -0.010823864,
			-0.010361097, -0.009302434, 0.008142014, -0.004064208, 0.000584335, 0.004640294,
			0.031893332, -0.013544321, -0.023573641, -0.004665085, -0.006446259, -0.005311412,
			0.045096308, -0.007374697, -0.00514201, -0.001715798, -0.005176363, -0.002884991,
			0.002309361, -0.014521608, -0.017711709, 0.001192088, -0.00238233, -0.004395918
		}
	};

	std::cout << "Design Matrix is\n:";
	std::cout << X << "\n\n";

	VectorXd Y{30};	// 30 observations 
	Y <<
		-0.039891316, 0.00178709, -0.0162018, 0.056452057, 0.00342504, -0.012038314,
		-0.009997657, 0.013452043, 0.013485674, -0.007898137, 0.008111428, -0.015424523,
		-0.002161451, -0.028752191, 0.011292655, -0.007958389, -0.004002386, -0.031690771,
		0.026776892, 0.009803957, 0.000886608, 0.01495181, -0.004155781, -0.001535225,
		0.013517306, -0.021228542, 0.001988701, -0.02051788, 0.005841347, 0.011248933;

	cout << "Design matrix X:\n" << X << "\n\n";
	cout << "Response vector Y:\n" << Y << "\n\n";

	VectorXd beta = X.householderQr().solve(Y);

	cout << "The QR least-squares estimate of the regression coefficients (beta) is (solution of beta * X = Y):"
		<< "\n" << beta << "\n\n";
}

