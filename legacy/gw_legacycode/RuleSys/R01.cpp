#include "R01.h"

const char * R01::Name() { return "R01"; }
R01::R01(const double *dParamVector)
{
  _iMinB4Close = (int) dParamVector[0];
  _dThresPctg = dParamVector[1];
  _dSLossPctg = dParamVector[2];
  _iReverse = (int) dParamVector[3];
  _iMaxNoTrial = (int) dParamVector[4];
  _iPermittedTrdDir = (int) dParamVector[5];

  Reset();
}

void R01::DailyInit()
{
  _iPos = 0;
  Reset();
}
void R01::ResetChild()
{
  _iTrialTaken = 0;
  _dEntryPrice = 0;
  _hlDay.Reset(9999,true); // Adequately large for the day; more than 60x24=1440
}

void R01::Add(const BarProvider &bA)
{
  _hlDay.Add(bA.High(),bA.Low());

  HMS hmsStartTimeR01;
  HKFE::GetAftnClose(bA.GetYYYYMMDD(),hmsStartTimeR01);

  hmsStartTimeR01.SubtractMinute(_iMinB4Close);

  // R01's trading time
  if (hmsStartTimeR01 <= bA.GetHHMMSS() && HKFE::IsTrdgHour5m(bA.GetYYYYMMDD(),bA.GetHHMMSS()))
  {
    if (_iTrialTaken < _iMaxNoTrial && this->_iPos == 0)
    {
      // Do nothing for conflicting situation
      if (bA.Close() > _hlDay.HighestHigh() * (1-_dThresPctg) &&
          bA.Close() < _hlDay.LowestLow()   * (1+_dThresPctg)    )
        return;

      // Long
      if (bA.Close() > _hlDay.HighestHigh() * (1-_dThresPctg) && _iPermittedTrdDir != -1)
      {
        this->_iPos = _iReverse * 1;
        _dEntryPrice = bA.Close();
        ++_iTrialTaken;
        _dEntryPrice = bA.Close();
      }
      // Short
      else if (bA.Close() < _hlDay.LowestLow() * (1+_dThresPctg) && _iPermittedTrdDir != 1)
      {
        this->_iPos = _iReverse * -1;
        _dEntryPrice = bA.Close();
        ++_iTrialTaken;
        _dEntryPrice = bA.Close();
      }
      return;
    }
    else if (this->_iPos != 0) // Already have a position? Then look for the exit
    {
      // POE stop
      if (_dSLossPctg > 0)
      {
        if ((_iReverse * this->_iPos) > 0 && (_dEntryPrice - bA.Close()) > _dSLossPctg * _dEntryPrice)
        {
          this->_iPos = 0;
          _dEntryPrice = 0;
        }
      }
      else return;
    }
    else // Not R01 trading time
    {
      Reset();
      return;
    }
  }
}
