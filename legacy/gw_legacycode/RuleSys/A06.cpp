#include "A06.h"

const char * A06::Name() { return "A06"; }
A06::A06(const double *dParamVector)
{
	_iEntryWindow = (int) dParamVector[0];
	_iExitWindow = (int) dParamVector[1];
	_dTkPftFrac = dParamVector[2];
	_dSLFrac = dParamVector[3];
	_dMaxChnlHeight = dParamVector[4];
	_dAtrAlpha = dParamVector[5];
	_iPermittedTrdDir = (int) dParamVector[6];
	_iReverse = (int) dParamVector[7];

	Reset();
}


void A06::DailyInit()
{
  Reset();
}

void A06::ResetChild()
{
	_dEntryPrice = 0;
	_atr.Reset(_dAtrAlpha,true);
	_hlChnlEntry.Reset(_iEntryWindow,true);
	_hlChnlExit.Reset(_iExitWindow,true);
}

void A06::Add(const BarProvider &bA)
{
	if (_hlChnlEntry.Ready() && _hlChnlExit.Ready() && _atr.Ready())
	{
				// Take profit
		if (((_iReverse * this->_iPos) > 0 && bA.Close() > _dEntryPrice + _hlChnlEntry.Height() * _dTkPftFrac) ||
				((_iReverse * this->_iPos) < 0 && bA.Close() < _dEntryPrice - _hlChnlEntry.Height() * _dTkPftFrac) ||
				// Stop loss
				((_iReverse * this->_iPos) > 0 && bA.Close() < _dEntryPrice - _hlChnlEntry.Height() * _dSLFrac) ||
				((_iReverse * this->_iPos) < 0 && bA.Close() > _dEntryPrice + _hlChnlEntry.Height() * _dSLFrac) ||
				// Exit Trade?
				((_iReverse * this->_iPos) > 0 && (bA.Close() < _hlChnlEntry.LowestLow()   || bA.Close() < _hlChnlExit.LowestLow()))   ||
				((_iReverse * this->_iPos) < 0 && (bA.Close() > _hlChnlEntry.HighestHigh() || bA.Close() > _hlChnlExit.HighestHigh())) ||
				(!HKFE::IsTrdgHour(bA.GetYYYYMMDD(),bA.GetHHMMSS())))
		{
			this->_iPos = 0;
		}

		// Enter Trade?
		if (((_iReverse * this->_iPos) != 1) &&
				(_iPermittedTrdDir != -1) &&
				(_hlChnlEntry.Height() < bA.Close() * _dMaxChnlHeight) &&
				(bA.Close() > _hlChnlEntry.HighestHigh()) &&
				(HKFE::IsTrdgHour(bA.GetYYYYMMDD(),bA.GetHHMMSS())))
		{
			this->_iPos = 1 * _iReverse;
			_dEntryPrice = bA.Close();
		}
		else if (((_iReverse * this->_iPos) != -1) &&
				(_iPermittedTrdDir != 1) &&
				(_hlChnlEntry.Height() < bA.Close() * _dMaxChnlHeight) &&
				(bA.Close() < _hlChnlEntry.LowestLow()) &&
				(HKFE::IsTrdgHour(bA.GetYYYYMMDD(),bA.GetHHMMSS())))
		{
			this->_iPos = -1 * _iReverse;
			_dEntryPrice = bA.Close();
		}
	}

	// Update indicators
	_atr.Add(bA.High(),bA.Low(),bA.Close());
	_hlChnlEntry.Add(bA.High(),bA.Low());
	_hlChnlExit.Add(bA.High(),bA.Low());

}
