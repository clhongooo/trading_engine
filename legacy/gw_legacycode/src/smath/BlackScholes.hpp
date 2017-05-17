#ifndef BLACKSCHOLES_HPP
#define BLACKSCHOLES_HPP

#include "../corelib/ValidityChk.hpp"
#include "gsl/gsl_cdf.h"
#include <cmath>

using namespace std;

class BlackScholes
{
	public:
		static double d1(double,double,double,double,double,double);
		static double d2(double,double,double,double,double,double);
		static double EuropeanCallPrice(double,double,double,double,double,double);
		static double EuropeanPutPrice(double,double,double,double,double,double);
		static double ImpldVolatility(bool,double,double,double,double,double,double);

		static double CallDelta(double,double,double,double,double,double);
		static double PutDelta(double,double,double,double,double,double);
		static double Gamma(double,double,double,double,double,double);
};

#endif
