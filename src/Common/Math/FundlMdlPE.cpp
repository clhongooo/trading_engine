#include "FundlMdlPE.h"

		const double FundlMdlPE::_EPS_k = 0.08333; // around 1 year
		const double FundlMdlPE::_EPS_SD = 0.11;

		const double FundlMdlPE::_PE_MEAN = 14.67;
		const double FundlMdlPE::_PE_k = 0.08333; // around 1 year
		const double FundlMdlPE::_PE_SD = 1.782538;

		const double FundlMdlPE::_E = 2.71828;

		const double FundlMdlPE::_EXP_EPS1 = 186.99666;
		const double FundlMdlPE::_EXP_EPS2 = 0.0050451;
		const double FundlMdlPE::_EXP_EPS3 = -206.05888;

		const int FundlMdlPE::_dInitTime = 480;
		const double FundlMdlPE::_dInitEPS = 2096.52;
		const double FundlMdlPE::_dInitNmlzPE = -3.67;
		const double FundlMdlPE::_dInitHSI = 23083;

		const double FundlMdlPE::_HSI_LIM_MTH = 0.05;
		const double FundlMdlPE::_HSI_LIM_DAY = 0.001;

FundlMdlPE::FundlMdlPE()
{
	gsl_rng_env_setup();
	T = gsl_rng_default;
	r = gsl_rng_alloc(T);
	gsl_rng_set(r, time(NULL));
	Reset();
}
void FundlMdlPE::Reset()
{
		_dTime = _dInitTime;
		_dEPS = _dInitEPS;
		_dNmlzPE = _dInitNmlzPE;
		_dHSI = _dInitHSI;
}

void FundlMdlPE::NextMonth()
{
	// Random
	double dZ1 = gsl_ran_gaussian(r,1);
	double dZ2 = gsl_ran_gaussian(r,1);

	++_dTime;

	// Exponential EPS
	double dExpEPS = _EXP_EPS1 * pow(_E, _EXP_EPS2 * _dTime) + _EXP_EPS3;

	// EPS
	_dEPS = _dEPS + (_EPS_k * (dExpEPS-_dEPS)) + _dEPS*_EPS_SD*dZ1;

	// Normalized PE
	_dNmlzPE = _dNmlzPE + (_PE_k * STool::Sign(-_dNmlzPE) * pow(abs(_dNmlzPE),1.2)) + _PE_SD*dZ2;

	// HSI
	_dHSI = STool::Min(STool::Max(_dEPS*(_dNmlzPE+_PE_MEAN), _dHSI*(1-_HSI_LIM_MTH)),_dHSI*(1+_HSI_LIM_MTH));
}

