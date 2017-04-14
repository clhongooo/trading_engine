#include "Bisection.h"

Bisection::Bisection()
	:
		_dLowerBound(-1),
		_dUpperBound(-1),
		_dMiddle(-1),
		_dMiddlePrev(-1),
		_bFirstTime(true),
		_dIntendedAccuracy(-1)
{}

// Arguments:
// d1, d2: Accepts arguments d1 and d2 in any order
// dIntendedAccuracy: (Intended Accuracy) e.g. 0.001 means abs(dMiddle(n)-dMiddle(n-1)) < 0.001
double Bisection::GetFirst(
		double d1,
		double d2,
		double dIntendedAccuracy)
{
	if (!_bFirstTime)
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}

	_dIntendedAccuracy = dIntendedAccuracy;
	_bFirstTime = false;

	if (d1 > d2)
	{
		_dLowerBound = d2;
		_dUpperBound = d1;
	}
	else
	{
		_dUpperBound = d2;
		_dLowerBound = d1;
	}
	_dMiddle = (_dUpperBound + _dLowerBound) / (double)2;

	return _dMiddle;
}

// Tells the class whether the last figure was too large
double Bisection::GetNext(bool bTooLarge)
{
	if (_dIntendedAccuracy < 0)
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}

	if (bTooLarge) _dUpperBound = _dMiddle;
	else _dLowerBound = _dMiddle;

	_dMiddlePrev = _dMiddle;
	_dMiddle = (_dUpperBound + _dLowerBound) / (double)2;

	return _dMiddle;
}

bool Bisection::IsEnough()
{
	if (_dMiddlePrev == -1) return false;
	else if (abs(_dMiddle - _dMiddlePrev) < _dIntendedAccuracy) return true;
	else return false;
}

