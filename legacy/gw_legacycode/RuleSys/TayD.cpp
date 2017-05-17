#include "TayD.h"

const char * TayD::Name() { return "TayD"; }

TayD::TayD()
{
  Reset();
}

void TayD::ResetChild()
{
  vector<int> vWmaParam;
  vWmaParam.push_back(4);
  vWmaParam.push_back(3);
  vWmaParam.push_back(1);
  _wma.Reset(vWmaParam,true);
  _ema10.Reset((double)1/10,true);
  _ema20.Reset((double)1/20,true);
  _ema19.Reset((double)1/19,true);
  _ema18.Reset((double)1/18,true);
  _ema17.Reset((double)1/17,true);
  _ema16.Reset((double)1/16,true);
  _dqema10.clear();
  _dqema20.clear();
  _dqema19.clear();
  _dqema18.clear();
  _dqema17.clear();
  _dqema16.clear();
}

void TayD::Add(const BarProvider &bA)
{
  double dMid = (bA.High() + bA.Low() + bA.Close()) / (double)3;

  _wma.Add(dMid);
  if (!_wma.Ready()) return;

  _ema10.Add(_wma.Value());
  if (!_ema10.Ready()) return;

  _dqema10.push_back(_ema10.Value());
  if (_dqema10.size() < 3) return;

  //--------------------------------------------------
  // Taylor expansion
  //--------------------------------------------------
  double dTay = _wma.Value() + (_dqema10.back() - *(_dqema10.end()-2)) + 0.5 * (_dqema10.back() - 2 * *(_dqema10.end()-2) + *(_dqema10.end()-3));
  _ema20.Add(dTay);
  _ema19.Add(dTay);
  _ema18.Add(dTay);
  _ema17.Add(dTay);
  _ema16.Add(dTay);

  if (!_ema20.Ready() || !_ema19.Ready() || !_ema18.Ready() || !_ema17.Ready() || !_ema16.Ready()) return;

  _dqema20.push_back(_ema20.Value());
  _dqema19.push_back(_ema19.Value());
  _dqema18.push_back(_ema18.Value());
  _dqema17.push_back(_ema17.Value());
  _dqema16.push_back(_ema16.Value());

  if (_dqema20.size() < 2 || _dqema19.size() < 2 || _dqema18.size() < 2 || _dqema17.size() < 2 || _dqema16.size() < 2) return;

  int iCntRisingEma = 0;
  if (_dqema10.back() > *(_dqema10.end()-2)) iCntRisingEma++; else iCntRisingEma--;
  if (_dqema20.back() > *(_dqema20.end()-2)) iCntRisingEma++; else iCntRisingEma--;
  if (_dqema19.back() > *(_dqema19.end()-2)) iCntRisingEma++; else iCntRisingEma--;
  if (_dqema18.back() > *(_dqema18.end()-2)) iCntRisingEma++; else iCntRisingEma--;
  if (_dqema17.back() > *(_dqema17.end()-2)) iCntRisingEma++; else iCntRisingEma--;
  if (_dqema16.back() > *(_dqema16.end()-2)) iCntRisingEma++; else iCntRisingEma--;

  if (iCntRisingEma > 0 && dTay > _ema20.Value()) _iPos = 1;
  else if (iCntRisingEma < 0 && dTay < _ema20.Value()) _iPos = -1;
  else _iPos = 0;

}
