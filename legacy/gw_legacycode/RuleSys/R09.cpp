#include "R09.h"

const char * R09::Name() { return "R09"; }
R09::R09(const double *dParamVector)
{
  _dPctgStopLoss = dParamVector[0];
  _dPctgTkPft = dParamVector[1];
  _iFromHHMM = (int) dParamVector[2];
  _iToHHMM = (int) dParamVector[3];

  _hmsFromTime.Set(_iFromHHMM/100,_iFromHHMM%100,0);
  _hmsToTime.Set(_iToHHMM/100,_iToHHMM%100,0);

  Reset();
}

void R09::DailyInit()
{
  Reset();
}
void R09::ResetChild()
{
  _iPos = 0;
  _dStopLossPrice = 0;
  _dTakeProfitPrice = 0;
}

void R09::Add(const BarProvider &bA)
{

  if (_iPos != 0 && (bA.GetHHMMSS() < _hmsFromTime || bA.GetHHMMSS() >= _hmsToTime))
  {
    _iPos = 0;
    Reset();
  }
  else if (_iPos == 0 && bA.GetHHMMSS() >= _hmsFromTime && bA.GetHHMMSS() < _hmsToTime)
  {
    _iPos = -1;
    _dStopLossPrice = bA.Close() * (1+_dPctgStopLoss);
    _dTakeProfitPrice = bA.Close() * (1-_dPctgTkPft);
  }
  else if (_iPos == 1 && bA.Close() < _dStopLossPrice) // stop loss
  {
    _iPos = -1;
    _dStopLossPrice = bA.Close() * (1+_dPctgStopLoss);
    _dTakeProfitPrice = bA.Close() * (1-_dPctgTkPft);
  }
  else if (_iPos == -1 && bA.Close() > _dStopLossPrice) // stop loss
  {
    _iPos = 1;
    _dStopLossPrice = bA.Close() * (1-_dPctgStopLoss);
    _dTakeProfitPrice = bA.Close() * (1+_dPctgTkPft);
  }
  else if (_iPos == 1 && bA.Close() > _dTakeProfitPrice) // take profit
  {
    _iPos = 1;
    _dStopLossPrice = bA.Close() * (1-_dPctgStopLoss);
    _dTakeProfitPrice = bA.Close() * (1+_dPctgTkPft);
  }
  else if (_iPos == -1 && bA.Close() < _dTakeProfitPrice) // take profit
  {
    _iPos = -1;
    _dStopLossPrice = bA.Close() * (1+_dPctgStopLoss);
    _dTakeProfitPrice = bA.Close() * (1-_dPctgTkPft);
  }

}
