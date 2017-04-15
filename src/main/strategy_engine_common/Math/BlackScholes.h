#ifndef BLACKSCHOLES_H
#define BLACKSCHOLES_H

#include "PCH.h"
#include "Constants.h"
#include "ValidityChk.h"
#include "gsl/gsl_cdf.h"

using namespace std;

class BlackScholes
{
	public:
		static double d1(double,double,double,double,double,double);
		static double d2(double,double,double,double,double,double);
		static double EuropeanCallPrice(double,double,double,double,double,double);
		static double EuropeanPutPrice(double,double,double,double,double,double);
		static double EuropeanCallPriceFut(double,double,double,double,double);
		static double EuropeanPutPriceFut(double,double,double,double,double);
		static double ImpldVolatility(bool,double,double,double,double,double,double);
		static double ImpldVolatility(bool,double,double,double,double,double,double,double);
		static double ImpldVolatilityFut(bool,double,double,double,double,double);
		static double ImpldVolatilityFut(bool,double,double,double,double,double,double);

		static double CallDelta(double,double,double,double,double,double);
		static double PutDelta(double,double,double,double,double,double);
		static double Gamma(double,double,double,double,double,double);
};

#endif
