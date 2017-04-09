#include "Dynamics.h"

Dynamics::Dynamics()
{
	gsl_rng_env_setup();
	T = gsl_rng_default;
	r = gsl_rng_alloc(T);
	gsl_rng_set(r, time(NULL));
}

void Dynamics::SetDynamicsA(double d, double S, int m, double v, double x1, double x2)
{
	_cDyn = 'A';

	_d = d;
	_m = m;
	_S = S;
	_InitS = S;
	_v = v;
	_dMaxUpPctgChg = x1;
	_dMaxDwPctgChg = x2;

	//_r = r;

	_iTime = 0;
}

double Dynamics::NextS()
{
	if (_cDyn == 'A')
	{
		if (_iTime >= _m) return _S;

		double dZ = gsl_ran_gaussian(r,1);
		double dS = (_d - _S)/(double)(_m - _iTime) + _v * dZ * _S;

		dS = STool::Max(STool::Min(dS,_dMaxUpPctgChg * _S),-_dMaxDwPctgChg * _S);

		_S += dS;
		++_iTime;
		return _S;
	}
  return 0;
}

