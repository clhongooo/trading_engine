// Class name: Bisection
// 
// Author: Sunny Yan
// 
// Description: This class implements the bisection numerical method for solving (non-linear) functions.
// GetFirst() is supposed to be called at the outset, then GetNext() is used in all subsequent calls.

#ifndef BISECTION_H
#define BISECTION_H

#include "PCH.h"
#include "Constants.h"
#include "SException.h"

using namespace std;

class Bisection
{
	private:
		double _dLowerBound;
		double _dUpperBound;
		double _dMiddle;
		double _dMiddlePrev;
		bool _bFirstTime;
		double _dIntendedAccuracy;

	public:
		Bisection();
		double GetFirst(double,double,double);
		double GetNext(bool);
		bool IsEnough();
};

#endif
