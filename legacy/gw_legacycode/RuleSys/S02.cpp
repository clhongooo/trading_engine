#include "S02.h"


const char * S02::Name() { return "S02"; }
S02::S02(const string & s1, const string & s2, const double *dParamVector) :
  iCondition(0),
  dLeg1chgAtMrngRef(0),
  dLeg2chgAtMrngRef(0),
  dLeg1chgAtAftnRef(0),
  dLeg2chgAtAftnRef(0),
  dLeg1MrngClosePrice(0),
  dLeg2MrngClosePrice(0),
  dLeg1AftnClosePrice(0),
  dLeg2AftnClosePrice(0),
  dLeg1CurrentPrice(0),
  dLeg2CurrentPrice(0),

  _sLeg1Sym(s1),
  _sLeg2Sym(s2), 
  _dLeg1_1st2ndLevBoundary(0),
  _dLeg2_1st2ndLevBoundary(0),
  _hmsMrngRefTime(0),
  _hmsAftnRefTime(0),
  _dMrngLeg2ConfPctg(0),
  _dAftnLeg2ConfPctg(0),
  _iSmaPeriod(10)
{
  _dLeg1_1st2ndLevBoundary = dParamVector[0];
  _dLeg2_1st2ndLevBoundary = dParamVector[1];
  _hmsMrngRefTime          = HMS((int)dParamVector[2]/100,(int)dParamVector[2]%100,0);
  _hmsAftnRefTime          = HMS((int)dParamVector[3]/100,(int)dParamVector[3]%100,0);
  _dMrngLeg2ConfPctg       = dParamVector[4];
  _dAftnLeg2ConfPctg       = dParamVector[5];
  _iSmaPeriod              = (int)dParamVector[6];

  Reset();
}

void S02::ResetChild()
{
  DailyInit();
  dLeg1MrngClosePrice = 0;
  dLeg2MrngClosePrice = 0;
  dLeg1AftnClosePrice = 0;
  dLeg2AftnClosePrice = 0;
}
void S02::DailyInit()
{
  _iPos = 0;
  iTradeDir = 0;
  iCondition = 0;
  dLeg1chgAtMrngRef = 0;
  dLeg2chgAtMrngRef = 0;
  dLeg1chgAtAftnRef = 0;
  dLeg2chgAtAftnRef = 0;
  smaLeg1.Reset(_iSmaPeriod,true);
  smaLeg2.Reset(_iSmaPeriod,true);
}

void S02::Add(const string & sSym, const BarProvider &bA)
{
  //--------------------------------------------------
  // UNTESTED
  //--------------------------------------------------
  HKFE::TRDGSESS ts = HKFE::GetTrdgSess(bA.GetYYYYMMDD(),bA.GetHHMMSS());

  if (sSym.substr(0,3) == _sLeg1Sym.substr(0,3))
  {
    dLeg1CurrentPrice = bA.Close();
    if (ts == HKFE::MRNG_SESS) dLeg1MrngClosePrice = bA.Close();
    else if (ts == HKFE::AFTN_SESS) dLeg1AftnClosePrice = bA.Close();
    smaLeg1.Add(bA.Close());
  }
  else if (sSym.substr(0,3) == _sLeg2Sym.substr(0,3))
  {
    dLeg2CurrentPrice = bA.Close();
    if (ts == HKFE::MRNG_SESS) dLeg2MrngClosePrice = bA.Close();
    else if (ts == HKFE::AFTN_SESS) dLeg2AftnClosePrice = bA.Close();
    smaLeg2.Add(bA.Close());
  }

  if (sSym.substr(0,3) == _sLeg2Sym.substr(0,3)) return;

  if (ts == HKFE::MRNG_SESS)
    DetermineMrngAction(sSym, bA);
  else if (ts == HKFE::AFTN_SESS)
    DetermineAftnAction(sSym, bA);

  if (!smaLeg1.Ready() || !smaLeg2.Ready()) return;

  if      (iTradeDir > 0 && _iPos <= 0 && bA.Close() > smaLeg1.Value())  _iPos = 1;
  else if (iTradeDir < 0 && _iPos >= 0 && bA.Close() < smaLeg1.Value())  _iPos = -1;
  else if (iTradeDir > 0 && _iPos > 0 && bA.Close() < smaLeg1.Value())   _iPos = 0;
  else if (iTradeDir < 0 && _iPos < 0 && bA.Close() > smaLeg1.Value())   _iPos = 0;
}

//returns -2 -2nd level
//returns -1 -1st level
//returns +1 +1st level
//returns +2 +2nd level
int S02::DetLeg1_1st2ndLevel(double dP, double dChg)
{
  if (dChg/dP*100 <= -_dLeg1_1st2ndLevBoundary) return -2;
  else if (dChg/dP*100 < 0) return -1;
  else if (dChg/dP*100 == 0) return 0;
  else if (dChg/dP*100 < _dLeg1_1st2ndLevBoundary) return 1;
  else return 2;
}

//returns -2 -2nd level
//returns -1 -1st level
//returns +1 +1st level
//returns +2 +2nd level
int S02::DetLeg2_1st2ndLevel(double dP, double dChg)
{
  if (dChg/dP*100 <= -_dLeg2_1st2ndLevBoundary) return -2;
  else if (dChg/dP*100 < 0) return -1;
  else if (dChg/dP*100 == 0) return 0;
  else if (dChg/dP*100 < _dLeg2_1st2ndLevBoundary) return 1;
  else return 2;
}

void S02::DetermineMrngAction(const string & sSymbol, const BarProvider & bA)
{
  if (dLeg1AftnClosePrice == 0 || dLeg2AftnClosePrice == 0) return; //can't calculate price difference

  double dLeg1chg = dLeg1CurrentPrice - dLeg1AftnClosePrice;
  double dLeg2chg = dLeg2CurrentPrice - dLeg2AftnClosePrice;

  if (bA.GetHHMMSS() >= _hmsMrngRefTime && iTradeDir == 0)
  {
    if      (DetLeg1_1st2ndLevel(dLeg1AftnClosePrice,dLeg1chg) == 1  && DetLeg2_1st2ndLevel(dLeg2AftnClosePrice,dLeg2chg) == 1 && dLeg2chg >= dLeg1chg) iCondition = 1;
    else if (DetLeg1_1st2ndLevel(dLeg1AftnClosePrice,dLeg1chg) == 1  && DetLeg2_1st2ndLevel(dLeg2AftnClosePrice,dLeg2chg) == 1 && dLeg1chg >= dLeg2chg) iCondition = 2;
    else if (DetLeg1_1st2ndLevel(dLeg1AftnClosePrice,dLeg1chg) == -1 && DetLeg2_1st2ndLevel(dLeg2AftnClosePrice,dLeg2chg) == -1 && dLeg2chg <= dLeg1chg) iCondition = 3;
    else if (DetLeg1_1st2ndLevel(dLeg1AftnClosePrice,dLeg1chg) == -1 && DetLeg2_1st2ndLevel(dLeg2AftnClosePrice,dLeg2chg) == -1 && dLeg1chg < dLeg2chg) iCondition = 4;
    else if (DetLeg1_1st2ndLevel(dLeg1AftnClosePrice,dLeg1chg) == 1  && DetLeg2_1st2ndLevel(dLeg2AftnClosePrice,dLeg2chg) == -1) iCondition = 5;
    else if (DetLeg1_1st2ndLevel(dLeg1AftnClosePrice,dLeg1chg) == -1 && DetLeg2_1st2ndLevel(dLeg2AftnClosePrice,dLeg2chg) == 1) iCondition = 6;
    else if (DetLeg1_1st2ndLevel(dLeg1AftnClosePrice,dLeg1chg) == -1 && DetLeg2_1st2ndLevel(dLeg2AftnClosePrice,dLeg2chg) == -2) iCondition = 7;
    else if (DetLeg1_1st2ndLevel(dLeg1AftnClosePrice,dLeg1chg) == 1  && DetLeg2_1st2ndLevel(dLeg2AftnClosePrice,dLeg2chg) == 2) iCondition = 8;
    else if (DetLeg1_1st2ndLevel(dLeg1AftnClosePrice,dLeg1chg) == -2 && DetLeg2_1st2ndLevel(dLeg2AftnClosePrice,dLeg2chg) == -2) iCondition = 9;
    else if (DetLeg1_1st2ndLevel(dLeg1AftnClosePrice,dLeg1chg) == 2  && DetLeg2_1st2ndLevel(dLeg2AftnClosePrice,dLeg2chg) == 2) iCondition = 10;

    dLeg1chgAtMrngRef = dLeg1chg;
    dLeg2chgAtMrngRef = dLeg2chg;
  }
  else if (bA.GetHHMMSS() > _hmsMrngRefTime && HKFE::GetTrdgSess(bA.GetYYYYMMDD(),bA.GetHHMMSS()) == HKFE::MRNG_SESS && iTradeDir == 0)
  {
    if      (iCondition == 1 && DetLeg2_1st2ndLevel(dLeg2AftnClosePrice,dLeg2chg) <= -1) iTradeDir = -1;
    else if (iCondition == 3 && DetLeg2_1st2ndLevel(dLeg2AftnClosePrice,dLeg2chg) <= -1) iTradeDir = -1;
    else if (iCondition == 4) iTradeDir = -1;
    else if (iCondition == 5 && dLeg1chg >= 2 && DetLeg2_1st2ndLevel(dLeg2AftnClosePrice,dLeg2chg) >= 1) iTradeDir = 1;
    else if (iCondition == 6 && dLeg1chg <= -2 && DetLeg2_1st2ndLevel(dLeg2AftnClosePrice,dLeg2chg) <= -1) iTradeDir = -1;
    else if (iCondition == 8 && dLeg2chg/dLeg2AftnClosePrice >= dLeg2chgAtMrngRef/dLeg2AftnClosePrice+_dMrngLeg2ConfPctg/100) iTradeDir = 1;
    else if (iCondition == 9 && dLeg2chg/dLeg2AftnClosePrice <= dLeg2chgAtMrngRef/dLeg2AftnClosePrice-_dMrngLeg2ConfPctg/100) iTradeDir = -1;
    else if (iCondition == 10 && dLeg2chg/dLeg2AftnClosePrice >= dLeg2chgAtMrngRef/dLeg2AftnClosePrice+_dMrngLeg2ConfPctg/100) iTradeDir = 1;
  }
  else if (HKFE::GetTrdgSess(bA.GetYYYYMMDD(),bA.GetHHMMSS()) != HKFE::MRNG_SESS)
  {
    iTradeDir = 0;
  }

  return;
}

void S02::DetermineAftnAction(const string & sSymbol, const BarProvider & bA)
{
  if (dLeg1MrngClosePrice == 0 || dLeg2MrngClosePrice == 0) return; //can't calculate price difference

  double dLeg1chg = dLeg1CurrentPrice - dLeg1MrngClosePrice;
  double dLeg2chg = dLeg2CurrentPrice - dLeg2MrngClosePrice;

  if (bA.GetHHMMSS() >= _hmsAftnRefTime && iTradeDir == 0)
  {
    if      (DetLeg1_1st2ndLevel(dLeg1MrngClosePrice,dLeg1chg) == 1  && DetLeg2_1st2ndLevel(dLeg2MrngClosePrice,dLeg2chg) == 1 && dLeg2chg >= dLeg1chg) iCondition = 1;
    else if (DetLeg1_1st2ndLevel(dLeg1MrngClosePrice,dLeg1chg) == 1  && DetLeg2_1st2ndLevel(dLeg2MrngClosePrice,dLeg2chg) == 1 && dLeg1chg >= dLeg2chg) iCondition = 2;
    else if (DetLeg1_1st2ndLevel(dLeg1MrngClosePrice,dLeg1chg) == -1 && DetLeg2_1st2ndLevel(dLeg2MrngClosePrice,dLeg2chg) == -1 && dLeg2chg <= dLeg1chg) iCondition = 3;
    else if (DetLeg1_1st2ndLevel(dLeg1MrngClosePrice,dLeg1chg) == -1 && DetLeg2_1st2ndLevel(dLeg2MrngClosePrice,dLeg2chg) == -1 && dLeg1chg < dLeg2chg) iCondition = 4;
    else if (DetLeg1_1st2ndLevel(dLeg1MrngClosePrice,dLeg1chg) == 1  && DetLeg2_1st2ndLevel(dLeg2MrngClosePrice,dLeg2chg) == -1) iCondition = 5;
    else if (DetLeg1_1st2ndLevel(dLeg1MrngClosePrice,dLeg1chg) == -1 && DetLeg2_1st2ndLevel(dLeg2MrngClosePrice,dLeg2chg) == 1) iCondition = 6;
    else if (DetLeg1_1st2ndLevel(dLeg1MrngClosePrice,dLeg1chg) == -1 && DetLeg2_1st2ndLevel(dLeg2MrngClosePrice,dLeg2chg) == -2) iCondition = 7;
    else if (DetLeg1_1st2ndLevel(dLeg1MrngClosePrice,dLeg1chg) == 1  && DetLeg2_1st2ndLevel(dLeg2MrngClosePrice,dLeg2chg) == 2) iCondition = 8;
    else if (DetLeg1_1st2ndLevel(dLeg1MrngClosePrice,dLeg1chg) == -2 && DetLeg2_1st2ndLevel(dLeg2MrngClosePrice,dLeg2chg) == -2) iCondition = 9;
    else if (DetLeg1_1st2ndLevel(dLeg1MrngClosePrice,dLeg1chg) == 2  && DetLeg2_1st2ndLevel(dLeg2MrngClosePrice,dLeg2chg) == 2) iCondition = 10;

    dLeg1chgAtAftnRef = dLeg1chg;
    dLeg2chgAtAftnRef = dLeg2chg;
  }
  else if (bA.GetHHMMSS() > _hmsAftnRefTime && HKFE::GetTrdgSess(bA.GetYYYYMMDD(),bA.GetHHMMSS()) == HKFE::AFTN_SESS && iTradeDir == 0)
  {
    if      (iCondition == 1 && DetLeg2_1st2ndLevel(dLeg2MrngClosePrice,dLeg2chg) <= -1) iTradeDir = -1;
    else if (iCondition == 3 && DetLeg2_1st2ndLevel(dLeg2MrngClosePrice,dLeg2chg) <= -1) iTradeDir = -1;
    else if (iCondition == 4) iTradeDir = -1;
    else if (iCondition == 5 && dLeg1chg >= 2 && DetLeg2_1st2ndLevel(dLeg2MrngClosePrice,dLeg2chg) >= 1) iTradeDir = 1;
    else if (iCondition == 6 && dLeg1chg <= -2 && DetLeg2_1st2ndLevel(dLeg2MrngClosePrice,dLeg2chg) <= -1) iTradeDir = -1;
    else if (iCondition == 8 && dLeg2chg/dLeg2MrngClosePrice >= dLeg2chgAtAftnRef/dLeg2MrngClosePrice+_dAftnLeg2ConfPctg/100) iTradeDir = 1;
    else if (iCondition == 9 && dLeg2chg/dLeg2MrngClosePrice <= dLeg2chgAtAftnRef/dLeg2MrngClosePrice-_dAftnLeg2ConfPctg/100) iTradeDir = -1;
    else if (iCondition == 10 && dLeg2chg/dLeg2MrngClosePrice >= dLeg2chgAtAftnRef/dLeg2MrngClosePrice+_dAftnLeg2ConfPctg/100) iTradeDir = 1;
  }
  else if (HKFE::GetTrdgSess(bA.GetYYYYMMDD(),bA.GetHHMMSS()) != HKFE::AFTN_SESS)
  {
    iTradeDir = 0;
  }

  return;
}
