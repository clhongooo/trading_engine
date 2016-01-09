#include "MACD.h"

MACD::MACD()
{
  Reset(12,26,9,false);
}

MACD::MACD(unsigned int a, unsigned int b, unsigned int c, bool bC)
{
  Reset(a,b,c,bC);
}

void MACD::Reset(unsigned int a, unsigned int b, unsigned int c, bool bC)
{
  this->_iTargetSize = 0;
  this->_bCheckDataAsPrice = bC;
  _ema1.Reset((double)1/a,bC);
  _ema2.Reset((double)1/b,bC);
  _ema3.Reset((double)1/c,bC);
  _dLastMACD = MAGIC_NUM;
  _dLastMACDDiv = MAGIC_NUM;
}

bool MACD::Ready() const
{
  if (_dLastMACDDiv != MAGIC_NUM)
    return true;
  else return false;
}

void MACD::AddChild(double dP)
{
  _ema1.Add(dP);
  _ema2.Add(dP);
  if (_ema1.Ready() && _ema2.Ready())
  {
    _dLastMACD = _ema1.Value() - _ema2.Value();
    _ema3.Add(_dLastMACD);
  }
  _dLastMACDDiv = _dLastMACD - _ema3.Value();
}

double MACD::MACDVal() const
{
  if (_dLastMACD != MAGIC_NUM)
    return _dLastMACD;
  else return 0;
}

double MACD::MACDDiv() const
{
  if (_dLastMACDDiv != MAGIC_NUM)
    return _dLastMACDDiv;
  else return 0;
}

double MACD::Value() const
{
  return 0;
}

