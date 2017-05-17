#include "AMA.h"

const char * AMA::Name() { return "AMA"; }
AMA::AMA(const double *dParamVector) : _lAll1State(0)
{
  for (unsigned int i = 0; i < AMANUMOFMA; ++i)
  {
    _viSmaN.push_back((int) dParamVector[i]);
    _vSma.push_back(Sma<double>((int) dParamVector[i],true));
    _dDistSma.push_back(deque<double>());
  }

  //--------------------------------------------------
  // Find the all 1 state
  //--------------------------------------------------
  for (unsigned int i = 0; i < 2+(AMANUMOFMA*2); ++i)
    _lAll1State += pow(2, i);

  cout << "All1State = " << _lAll1State << endl;

  Reset();
}

void AMA::DailyInit()
{
  _iPos = 0;
}

void AMA::ResetChild()
{
  _oblma.Reset();

  for (unsigned int i = 0; i < AMANUMOFMA; ++i)
    _vSma[i].Reset(_viSmaN[i],true);

  _dqState.clear();

  if (!_dDistSma.empty())
  {
    for (unsigned int i = 0; i < _dDistSma.size(); ++i)
    {
      _dDistSma[i].clear();
    }
  }
  _oblma.Reset();
  _dDistOblma.clear();
}

void AMA::Add(const BarProvider &bA)
{
  //--------------------------------------------------
  // Update indicators
  //--------------------------------------------------
  _oblma.Add(bA.High(),bA.Low(),bA.Close());
  for (unsigned int i = 0; i < AMANUMOFMA; ++i)
    _vSma[i].Add(bA.Close());

  //--------------------------------------------------
  // See if indicators are Ready
  //--------------------------------------------------
  bool bAllReady = true;
  if (!_oblma.Ready()) bAllReady = false;
  for (unsigned int i = 0; i < AMANUMOFMA; ++i)
    if (!_vSma[i].Ready()) bAllReady = false;
  if (!bAllReady) return;

  //--------------------------------------------------
  // Dist from indicators
  //--------------------------------------------------
  _dDistOblma.push_back(bA.Close()-_oblma.Value());
  for (unsigned int i = 0; i < AMANUMOFMA; ++i)
    _dDistSma[i].push_back(bA.Close()-_vSma[i].Value());


  //--------------------------------------------------
  // State
  //--------------------------------------------------
  if (_dDistOblma.size() < 2) return;
  int iState = 0;
  int iTmp = 0;

  if (bA.Close() > _oblma.Value()) iState += pow(2, iTmp++);
  for (unsigned int i = 0; i < AMANUMOFMA; ++i)
    if (bA.Close() > _vSma[i].Value()) iState += pow(2, iTmp++);


  if (_dDistOblma.back()-*(_dDistOblma.end()-2) > 0) iState += pow(2, iTmp++);
  for (unsigned int i = 0; i < AMANUMOFMA; ++i)
  {
    if (_dDistSma[i].back()-*(_dDistSma[i].end()-2) > 0) iState += pow(2, iTmp++);
  }

  _dqState.push_back(iState);

  //--------------------------------------------------
  // Generate Signals
  //--------------------------------------------------
  if (HKFE::IsTrdgHour5m(bA.GetYYYYMMDD(),bA.GetHHMMSS()))
  {
    if (!_dqState.empty())
    {
      if      (_dqState.back() ==           0) _iPos = -1;
      else if (_dqState.back() == _lAll1State) _iPos = +1;
      else                                     _iPos =  0;
    }
    else
    {
      _iPos = 0;
    }
  }

}
