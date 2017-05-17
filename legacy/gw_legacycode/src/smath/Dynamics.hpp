#ifndef DYNAMICS_HPP
#define DYNAMICS_HPP

#include "../corelib/STool.hpp"
#include "gsl/gsl_randist.h"
#include "gsl/gsl_rng.h"
#include <cmath>
#include <ctime>

using namespace std;

class Dynamics
{
	private:
		char _cDyn;
		double _dMaxUpPctgChg;
		double _dMaxDwPctgChg;
		const gsl_rng_type * T;
		gsl_rng * r;

		int _iTime;
		double _InitS;
		double _d;
		int _m;
		double _S;
		double _v;
		double _r;

	public:
		Dynamics();
		// d = destination at maturity, S = initial S, m = maturity time, v = volatility, x1 = max up percentage change, x2 = max down percentage change
		void SetDynamicsA(double,double,int,double,double,double); // dS/S = (d - S)/(m - iTime)dt + v dBt
		double NextS();
};

#endif

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
}

