#ifndef DYNAMICS_H
#define DYNAMICS_H

#include "PCH.h"
#include "Constants.h"
#include "STool.h"
#include "gsl/gsl_randist.h"
#include "gsl/gsl_rng.h"
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


