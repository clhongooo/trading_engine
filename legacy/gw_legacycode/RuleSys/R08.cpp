#include "R08.h"

const char * R08::Name() { return "R08"; }
R08::R08(const double *dParamVector)
{
	_iRegrLag = (int) dParamVector[0];
	_dRegrSlopeMax = dParamVector[1];
	_dRegrSlopeMin = dParamVector[2];
	_iNBarHigh = (int) dParamVector[3];
	_dTkPftFraction = dParamVector[4];
	_dSLFraction = dParamVector[5];

	Reset();
}

void R08::DailyInit()
{
  Reset();
}
void R08::ResetChild()
{
	_iCnt = 0;
	_dSlopeLast = 0;
	_dSlopeCurrent = 0;
	_dDayHigh = 0;
	_dEntryPrice = 0;
	_hlShortTermHigh.Reset(_iNBarHigh,true);
	_hlProfitTarget.Reset(_iNBarHigh*3,true);
	_lr.Reset(_iRegrLag, true);
}

void R08::Add(const BarProvider &bA)
{
	// Update indicators 1st part
	_lr.AddYX(bA.Close(),_iCnt++);


	// Get Latest Slope once
	if (_lr.Ready())
	{
		_dSlopeLast = _dSlopeCurrent;
		_dSlopeCurrent = _lr.Value();
	}


	// Generate Signals
	bool bGoShort = false;
	bool bCloseShort = false;

	if (this->_iPos >= 0)
	{
		bGoShort = (_dSlopeCurrent < 0);
		bGoShort = bGoShort && (!STool::IsNaN(_dSlopeCurrent));
		bGoShort = bGoShort && (bA.Close() > _hlShortTermHigh.HighestHigh());
		bGoShort = bGoShort && (_dSlopeCurrent < _dRegrSlopeMax);
		bGoShort = bGoShort && (_dSlopeCurrent > _dRegrSlopeMin);
		bGoShort = bGoShort && (bA.Close() < _dDayHigh);
	}
	else
	{
		bCloseShort = (bA.Close() < _dEntryPrice - _dTkPftFraction * (_dEntryPrice - _hlProfitTarget.LowestLow()) ) ||
									(bA.Close() > _dEntryPrice * _dSLFraction    * (_dEntryPrice - _hlProfitTarget.LowestLow()) );
	}

	// Update indicators 2nd part
	if (bA.High() > _dDayHigh) _dDayHigh = bA.High();
	_hlShortTermHigh.Add(bA.High(),bA.Low());
	_hlProfitTarget.Add(bA.High(),bA.Low());


	// Execute orders
	if (bGoShort)
	{
		this->_iPos = -1;
		_dEntryPrice = bA.Close();
	}
	else if (bCloseShort)
	{
		this->_iPos = 0;
		_dEntryPrice = 0;
	}

}
