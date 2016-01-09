#include <Exchange.h>

boost::weak_ptr<Exchange> Exchange::m_pInstance;

boost::shared_ptr<Exchange> Exchange::Instance() {
  boost::shared_ptr<Exchange> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new Exchange());
    m_pInstance = instance;
  }
  return instance;
}

Exchange::Exchange() :
  m_HardDayClose(170000),
  m_DefaultTimeZone(SDateTime::HKT)
{
  m_HKFE = HKFE::Instance();
  m_HKSE = HKSE::Instance();
  m_NYSE = NYSE::Instance();
  m_CME  = CME::Instance();
}

Exchange::~Exchange()
{
}

EExchange Exchange::GetPrimaryExchange(const string & symbol)
{
  map<string,EExchange>::iterator it = m_CachedPrimaryExchg.find(symbol);
  if (it != m_CachedPrimaryExchg.end()) return it->second;

  if      (m_HKFE->IsAProductInThisMarket(symbol)) { m_CachedPrimaryExchg[symbol] = EX_HKFE; return EX_HKFE; }
  else if (m_HKSE->IsAProductInThisMarket(symbol)) { m_CachedPrimaryExchg[symbol] = EX_HKSE; return EX_HKSE; }
  else if (m_CME ->IsAProductInThisMarket(symbol)) { m_CachedPrimaryExchg[symbol] = EX_CME ; return EX_CME ; }
  else if (m_NYSE->IsAProductInThisMarket(symbol)) { m_CachedPrimaryExchg[symbol] = EX_NYSE; return EX_NYSE; }

  return EX_UNSET;
}

void Exchange::LoadTradingHours(const string & configpath)
{
  m_TrdgHr.ReadConfig(configpath);
}

bool Exchange::IsTradingHour(const string & symbol, const YYYYMMDDHHMMSS & ymdhms)
{
  return m_TrdgHr.IsTradingHour(symbol, ymdhms);
}

bool Exchange::IsTradingHour(const string & symbol, const YYYYMMDDHHMMSS & ymdhms, const int iStartBufferInSec, const int iEndBufferInSec, const int iBreakBufferInSec)
{
  return m_TrdgHr.IsTradingHourStartEndBreakBuffer(symbol, ymdhms, iStartBufferInSec, iEndBufferInSec, iBreakBufferInSec);
}

HHMMSS Exchange::GetTimeNSecBefClose(const string & symbol, const YYYYMMDD & ymd, const int iSecBefClose)
{
  return m_TrdgHr.GetTimeNSecBefClose(symbol, ymd, iSecBefClose);
}

bool Exchange::IsNewDay(const string & symbol, const YYYYMMDDHHMMSS & ymdhmsCurTimeHKT, const YYYYMMDDHHMMSS & ymdhmsLastMonTimeHKT)
{
  //--------------------------------------------------
  // Default return true
  //--------------------------------------------------
  if (!ymdhmsCurTimeHKT.IsValid() || !ymdhmsLastMonTimeHKT.IsValid()) return true;

  //--------------------------------------------------
  // Can have 1 more input parameter for the market of the instrument to avoid searching
  //--------------------------------------------------
  YYYYMMDDHHMMSS ymdhmsCurTimeLocal(ymdhmsCurTimeHKT);
  YYYYMMDDHHMMSS ymdhmsLastMonTimeLocal(ymdhmsLastMonTimeHKT);

  if (
    GetPrimaryExchange(symbol) == EX_NYSE ||
    GetPrimaryExchange(symbol) == EX_CME
    )
  {
    ymdhmsCurTimeLocal.Set(SDateTime::ChangeTimeZone(ymdhmsCurTimeHKT,m_DefaultTimeZone,SDateTime::EST));
    ymdhmsLastMonTimeLocal.Set(SDateTime::ChangeTimeZone(ymdhmsLastMonTimeHKT,m_DefaultTimeZone,SDateTime::EST));
  }

  if (
    (ymdhmsCurTimeLocal.GetYYYYMMDD() != ymdhmsLastMonTimeLocal.GetYYYYMMDD())
    ||
    (
      ymdhmsCurTimeLocal.GetYYYYMMDD() == ymdhmsLastMonTimeLocal.GetYYYYMMDD()
      &&
      ymdhmsCurTimeLocal.GetHHMMSS() > m_HardDayClose
      &&
      ymdhmsLastMonTimeLocal.GetHHMMSS() < m_HardDayClose
    )
    )
  {
    return true;
  }
  else
  {
    return false;
  }

  return true;
}


bool Exchange::IsRegularFutCtrt(const string & symbol)
{
  if (symbol.length() < 3) return false;

  string subs2 = symbol.substr(0,2);
  string subs3 = symbol.substr(0,3);

  if (subs3 == UNDERLYING_HSI)  return true;
  if (subs3 == UNDERLYING_HHI)  return true;
  if (subs3 == UNDERLYING_MHI)  return false;
  if (subs3 == UNDERLYING_MCH)  return false;
  if (subs2 == UNDERLYING_ES)   return false;
  if (subs2 == UNDERLYING_NQ)   return false;
  if (subs2 == UNDERLYING_YM)   return false;
  if (subs2 == UNDERLYING_TF)   return false;

  return false;
}

bool Exchange::IsMiniFutCtrt(const string & symbol)
{
  if (symbol.length() < 3) return false;

  string subs2 = symbol.substr(0,2);
  string subs3 = symbol.substr(0,3);

  if (subs3 == UNDERLYING_HSI)  return false;
  if (subs3 == UNDERLYING_HHI)  return false;
  if (subs3 == UNDERLYING_MHI)  return true;
  if (subs3 == UNDERLYING_MCH)  return true;
  if (subs2 == UNDERLYING_ES)   return true;
  if (subs2 == UNDERLYING_NQ)   return true;
  if (subs2 == UNDERLYING_YM)   return true;
  if (subs2 == UNDERLYING_TF)   return true;

  return false;
}

string Exchange::GetMiniFrRglrFutCtrt(const string & symbol)
{
  if (symbol.length() < 3) return "";

  string subs3     = symbol.substr(0,3);
  string subs3code = symbol.substr(3,symbol.length()-3);

  if (subs3 == UNDERLYING_HSI) return UNDERLYING_MHI + subs3code;
  if (subs3 == UNDERLYING_HHI) return UNDERLYING_MCH + subs3code;

  return "";
}

double Exchange::GetContractMultiplier(const string & symbol)
{
  EExchange ex = GetPrimaryExchange(symbol);
  if (ex == EX_NYSE) return 1;
  if (ex == EX_HKSE) return 1;
  if (ex == EX_CME)  return m_CME->GetContractMultiplier(symbol);
  if (ex == EX_HKFE) return m_HKFE->GetContractMultiplier(symbol);

  return 1;
}

