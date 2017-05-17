#ifndef MORTGAGE_HPP
#define MORTGAGE_HPP

#include "Bisection.hpp"
#include "../corelib/STool.hpp"
#include "../corelib/SException.hpp"

using namespace std;

class Mortgage
{
	private:
		static const double _ACCURACY; // Hardcoded accuracy parameter for the Bisection class
		static const int _DAYSINYEAR; // Hardcoded number of days in year for interest calculation

		double _dInitLoanAmt; // Initial Loan Amount
		double _dAnlIntRate; // Annual Interest Rate
		int _iStartYMD; // Start Date of Loan in YYYYMMDD
		int _iNumPeriods;

		double _dOSLoanAmt; // Outstanding Loan Amount
		int _iYMD; // From _iStartYMD to the end

	public:
		Mortgage(double,double,int,int);
		double CalcMonthlyPayment();
};

const double Mortgage::_ACCURACY = 0.001;
const int Mortgage::_DAYSINYEAR = 365;

Mortgage::Mortgage(
		double dInitLoanAmt,
		double dAnlIntRate,
		int iStartYMD,
		int iNumPeriods) :
	_dInitLoanAmt(dInitLoanAmt),
	_dAnlIntRate(dAnlIntRate),
	_iStartYMD(iStartYMD),
	_iNumPeriods(iNumPeriods)
{}

double Mortgage::CalcMonthlyPayment()
{
	Bisection bisec;
	double dMthPay = bisec.GetFirst(0,_dInitLoanAmt,_ACCURACY);

	while (!bisec.IsEnough())
	{
		// Initialization
		_iYMD = _iStartYMD;
		int iStartD = STool::GetDay(_iStartYMD);
		_dOSLoanAmt = _dInitLoanAmt;

		// Amortize using this dMthPay for # of periods (_iNumPeriods)
		for (int i = 0; i < _iNumPeriods; ++i)
		{
			// It's inefficient to calculate the array of dates in every run... but leave as is because of laziness
			int iNextYMD = STool::NextMonthStrict(_iYMD,iStartD);
			int iCalDayDiff = STool::CalendarDayDiff(iNextYMD,_iYMD);
			double dMthIntPymt = _dOSLoanAmt * _dAnlIntRate * (double)(iCalDayDiff)/_DAYSINYEAR;
			double dMthPrinPymt = dMthPay - dMthIntPymt;
			_dOSLoanAmt -= dMthPrinPymt;
			_iYMD = iNextYMD;
		}

		dMthPay = bisec.GetNext(_dOSLoanAmt < 0 ? true : false);
	}

	return dMthPay;
}

#endif
