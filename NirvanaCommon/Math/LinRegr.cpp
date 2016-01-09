#include "LinRegr.h"

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
	_lXCnt = 0;
	_bCheckDataAsPrice = bC;
	_cdqY.Reset(iS,bC);
	_cdqX.Reset(iS,bC);
}


bool LinRegr::Ready() const { return (_cdqY.Ready() && _cdqX.Ready()); }


void LinRegr::AddY(const double dY)
{
	_cdqY.Add(dY);
	_cdqX.Add((double)(++_lXCnt));
}

void LinRegr::AddYX(const double dY, const double dX)
{
	_cdqY.Add(dY);
	_cdqX.Add(dX);
}

double LinRegr::Value() const
{
	if (!Ready())
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

