#include "S04.h"

// Dual Thrust &amp; Centre of Gravity
// 
// Entry:
// Go Long if
// Price &gt; Open Price + K1 * Range AND MACD line is above signal line AND
// Fast SMA is above slow SMA
// Go Short if
// Price &lt; Open Price - K1 * Range AND MACD line is below signal line AND
// Fast SMA is below slow SMA
// 
// Where
// K1 is a chosen constant, and
// Range = Max(Highest High - Lowest Close, Highest Close - Lowest Low) over N days
// 
// Exit:
// Exit Long if
// Fast SMA crosses below the slow SMA OR MACD line crosses below signal line
// Exit Short if
// Fast SMA crosses above the slow SMA OR MACD line crosses above signal line
// 
// 
// <b>Dual Thrust</b>
// Range = Max(HH-LC,HC-LL)
// O+K1*Range
// O-K2*Range
// Filter: SMAN1 XO SMAN2
// 

const char * S04::Name() { return "S04"; }
S04::S04(const double *dParamVector)
{
  _dKBuy = dParamVector[0];
  _dKSell = dParamVector[1];
  _iHLPeriod = dParamVector[2];
  _iSmaPeriodBuyEntry = (int) dParamVector[3];
  _iSmaPeriodSellEntry = (int) dParamVector[4];
  _iSmaPeriodBuyExit = (int) dParamVector[5];
  _iSmaPeriodSellExit = (int) dParamVector[6];
  _iMacdParam1 = (int) dParamVector[7];
  _iMacdParam2 = (int) dParamVector[8];
  _iMacdParam3 = (int) dParamVector[9];

  Reset();
}

void S04::DailyInit()
{
  _smaBuyEntry.Reset(_iSmaPeriodBuyEntry, true);
  _smaSellEntry.Reset(_iSmaPeriodSellEntry, true);
  _smaBuyExit.Reset(_iSmaPeriodBuyExit, true);
  _smaSellExit.Reset(_iSmaPeriodSellExit, true);
  _macd.Reset(_iMacdParam1,_iMacdParam2,_iMacdParam3,true);
  // _highlow.Reset(_iHLPeriod, true);
  _ymdCurrentDay.Set(1900,1,1);
  _dOpenPrice = 0;
}
void S04::ResetChild()
{
  _smaBuyEntry.Reset(_iSmaPeriodBuyEntry, true);
  _smaSellEntry.Reset(_iSmaPeriodSellEntry, true);
  _smaBuyExit.Reset(_iSmaPeriodBuyExit, true);
  _smaSellExit.Reset(_iSmaPeriodSellExit, true);
  _macd.Reset(_iMacdParam1,_iMacdParam2,_iMacdParam3,true);
  _highlow.Reset(_iHLPeriod, true);
  _ymdCurrentDay.Set(1900,1,1);
  _dOpenPrice = 0;
}

void S04::Add(const BarProvider &bA)
{
  // Update indicators
  _smaBuyEntry.Add(bA.Close());
  _smaSellEntry.Add(bA.Close());
  _smaBuyExit.Add(bA.Close());
  _smaSellExit.Add(bA.Close());
  _highlow.Add(bA.High(),bA.Low());
  _macd.Add(bA.Close());

  if (!_smaBuyEntry.Ready() ||
      !_smaSellEntry.Ready() ||
      !_smaBuyExit.Ready() ||
      !_smaSellExit.Ready() ||
      !_highlow.Ready() ||
      !_macd.Ready()) return;

  if (bA.GetYYYYMMDD() > _ymdCurrentDay)
  {
    _dOpenPrice = bA.Open();
    _ymdCurrentDay = bA.GetYYYYMMDD();
  }

  // Generate Signals
  if      (_iPos == 0  && bA.Close() > _dOpenPrice + _dKBuy  * _highlow.Height() && bA.Close() > _smaBuyEntry.Value()  && _macd.MACDDiv() > 0)  _iPos = 1;
  else if (_iPos == 0  && bA.Close() < _dOpenPrice - _dKSell * _highlow.Height() && bA.Close() < _smaSellEntry.Value() && _macd.MACDDiv() < 0)  _iPos = -1;
  else if (_iPos == 1  && (bA.Close() < _smaBuyExit.Value()  ||  _macd.MACDDiv() > 0))                                                          _iPos = 0;
  else if (_iPos == -1 && (bA.Close() > _smaSellExit.Value() ||  _macd.MACDDiv() < 0))                                                          _iPos = 0;

}
