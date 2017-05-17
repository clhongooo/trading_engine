#ifndef LINREGR_HPP
#define LINREGR_HPP

#include "../corelib/SException.hpp"
#include "gsl/gsl_statistics.h"
#include "CirDataQue.hpp"

using namespace std;

class LinRegr
{
	private:
		int _iTargetSize;
		bool _bCheckDataAsPrice;
		CirDataQue<double> _cdqY;
		CirDataQue<double> _cdqX;

	public:
		LinRegr();
		LinRegr(int,bool);
		void AddYX(double,double);
		void Reset(int,bool);
		bool WarmedUp() const;
		double Value() const;
};


LinRegr::LinRegr()
{
	Reset(0,false);
}

LinRegr::LinRegr(int iS, bool bC)
{
	Reset(iS,bC);
}

void LinRegr::Reset(int iS, bool bC)
{
	_iTargetSize = iS;
	_bCheckDataAsPrice = bC;
	_cdqY.Reset(iS,bC);
	_cdqX.Reset(iS,bC);
}


bool LinRegr::WarmedUp() const { return (_cdqY.WarmedUp() && _cdqX.WarmedUp()); }


void LinRegr::AddYX(double dY, double dX)
{
	_cdqY.Add(dY);
	_cdqX.Add(dX);

}

double LinRegr::Value() const
{
	if (!WarmedUp())
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}

	int iY, iX;
	const double *Y = _cdqY.GetArrayUnord(iY);
	const double *X = _cdqX.GetArrayUnord(iX);

	int iS = iY; if (iY > iX) iS = iX;

	double dCovXY = gsl_stats_covariance(Y,1,X,1,iS);
	double dVarX = gsl_stats_variance(X,1,iS);

	return dCovXY / dVarX;
}

#endif
