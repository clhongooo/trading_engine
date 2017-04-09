#include "Rsi.h"

Rsi::Rsi(int iP, bool bC, Rsi::Method meth)
{
  Reset(iP,bC,meth);
}

void Rsi::Reset(int iP, bool bC, Rsi::Method meth)
{
  _iPeriod = iP;
  _dCloseNow = 0;
  _dClosePrev = 0;
  _RSI = 0;
  _method = meth;
  _emaU.Reset((double)1/_iPeriod,false); // not checked as price, because can be zero
  _emaD.Reset((double)1/_iPeriod,false); // not checked as price, because can be zero
  _smaU.Reset(_iPeriod,false); // not checked as price, because can be zero
  _smaD.Reset(_iPeriod,false); // not checked as price, because can be zero
  this->_bCheckDataAsPrice = bC;
}

void Rsi::AddChild(double dClose)
{
  _dClosePrev = _dCloseNow; 
  _dCloseNow  = dClose;

  if (_dCloseNow == 0 || _dClosePrev == 0) return;

  double dU, dD;

  if (_dCloseNow > _dClosePrev)
  {
    dU = (_dCloseNow - _dClosePrev);
    dD = 0;
  }
  else if (_dCloseNow < _dClosePrev)
  {
    dU = 0;
    dD = (_dClosePrev - _dCloseNow);
  }
  else
  {
    dU = 0;
    dD = 0;
  }

  double dRS = 0;
  if (_method == EMA)
  {
    _emaU.Add(dU);
    _emaD.Add(dD);
    if (!_emaU.Ready() || !_emaD.Ready()) return;
    dRS = _emaU.Value() / _emaD.Value();
  }
  else if (_method == SMA)
  {
    _smaU.Add(dU);
    _smaD.Add(dD);
    if (!_smaU.Ready() || !_smaD.Ready()) return;
    dRS = _smaU.Value() / _smaD.Value();
  }
  else return;

  _RSI = 100 - (100 / (1 + dRS));
}

bool Rsi::Ready() const { return (_RSI != 0); }

double Rsi::Value() const
{
  return _RSI;
}
