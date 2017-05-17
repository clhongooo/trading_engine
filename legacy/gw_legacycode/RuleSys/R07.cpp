#include "R07.h"

const char * R07::Name() { return "R07"; }
R07::R07(const double *dParamVector)
{
  _iPeriod = (int) dParamVector[0];
  _dBandDist = dParamVector[1];
  _dPctgStopLoss = dParamVector[2];
  _iFromHHMM = (int) dParamVector[3];
  _iToHHMM = (int) dParamVector[4];

  _hmsFromTime.Set(_iFromHHMM/100,_iFromHHMM%100,0);
  _hmsToTime.Set(_iToHHMM/100,_iToHHMM%100,0);

  Reset();
}

void R07::DailyInit()
{
  Reset();
}
void R07::ResetChild()
{
  vector<int> v;
  v.push_back(0);
  _snyStat.Reset(1, v, _iPeriod, _iPeriod * 10, true);
  _iPos = 0;
}

void R07::Add(const BarProvider &bA)
{

  _snyStat.Add(0,bA.Close());

  if (!_snyStat.Ready()) return;

  double dUpperBand = _snyStat.AMean(0) + _dBandDist * _snyStat.SD(0);
  double dLowerBand = _snyStat.AMean(0) - _dBandDist * _snyStat.SD(0);

  //--------------------------------------------------
  // Close trade
  //--------------------------------------------------
  if (_iPos != 0 && (bA.GetHHMMSS() < _hmsFromTime || bA.GetHHMMSS() >= _hmsToTime))
  {
    _iPos = 0;
    Reset();
  }
  else if (_iPos == 1 && (bA.Close() < _dStopLossPrice || bA.Close() > _snyStat.AMean(0)))
  {
    _iPos = 0;
    Reset();
  }
  else if (_iPos == -1 && (bA.Close() > _dStopLossPrice || bA.Close() < _snyStat.AMean(0)))
  {
    _iPos = 0;
    Reset();
  }
  //--------------------------------------------------
  // Open trade
  //--------------------------------------------------
  else if (_iPos == 0 && bA.GetHHMMSS() >= _hmsFromTime && bA.GetHHMMSS() < _hmsToTime)
  {
    if (bA.Close() < dLowerBand)
    {
      _iPos = 1;
      _dStopLossPrice = bA.Close() * (1-_dPctgStopLoss);
    }
    else if (bA.Close() > dUpperBand)
    {
      _iPos = -1;
      _dStopLossPrice = bA.Close() * (1+_dPctgStopLoss);
    }
  }

}
