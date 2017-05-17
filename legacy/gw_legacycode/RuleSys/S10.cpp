#include "S10.h"

const char * S10::Name() { return "S10"; }
S10::S10(const double *dParamVector)
{
	_iPermittedTrdDir = (int) dParamVector[0];
	_iReverse = (int) dParamVector[1];

	Reset();
}

void S10::DailyInit()
{
  _iPos = 0;
}

void S10::ResetChild()
{
  _oblma.Reset();
}

void S10::Add(const BarProvider &bA)
{
  _oblma.Add(bA.High(),bA.Low(),bA.Close());
  if (!_oblma.Ready()) return;
  _dqOBLMA.push_back(_oblma.Value());

  if (_dqOBLMA.size() > 2 && HKFE::IsTrdgHour5m(bA.GetYYYYMMDD(),bA.GetHHMMSS()))
  {
    if      (_iPermittedTrdDir != -1 &&_dqOBLMA.back() > *(_dqOBLMA.end()-2)) this->_iPos = _iReverse *  1;
    else if (_iPermittedTrdDir !=  1 &&_dqOBLMA.back() < *(_dqOBLMA.end()-2)) this->_iPos = _iReverse * -1;
  }
  else
  {
    this->_iPos = 0;
  }

}
