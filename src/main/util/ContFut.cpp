#include <ContFut.h>

ContFut::ContFut()
{
  m_Exchg  = Exchange::Instance();
  m_HKFE   = HKFE::Instance();
  m_CME    = CME::Instance();

  m_SymList.insert(UNDERLYING_HSI);
  m_SymList.insert(UNDERLYING_HHI);
  m_SymList.insert(UNDERLYING_ES);
  m_SymList.insert(UNDERLYING_NQ);
  m_SymList.insert(UNDERLYING_YM);
  m_SymList.insert(UNDERLYING_TF);

  for (set<string>::iterator it = m_SymList.begin(); it != m_SymList.end(); ++it)
  {
    const string & symbol = *it;

    m_ContFutAlrRolled                [symbol] = set<YYYYMMDD>();
    m_ContFutAdjustment               [symbol] = 0;
    m_LatestSnapshot_Sym_1FM          [symbol] = ATU_MDI_marketfeed_struct();
    m_LatestSnapshot_Sym_2FM          [symbol] = ATU_MDI_marketfeed_struct();
    m_LatestSnapshot_Sym_c1           [symbol] = ATU_MDI_marketfeed_struct();
    m_LatestSnapshotUpdateTime_Sym_c1 [symbol] = YYYYMMDDHHMMSS();
    m_Sym_c1Ready                     [symbol] = false;
  }
}

ContFut::~ContFut()
{
}

void ContFut::SetHKFERollFwdTime(const HHMMSS &hms)
{
  m_HKFE_RollFwdTime.Set(hms);
}

void ContFut::SetCMERollFwdTime(const HHMMSS &hms)
{
  m_CME_RollFwdTime.Set(hms);
}

void ContFut::Add(const YYYYMMDDHHMMSS & ymdhms, const ATU_MDI_marketfeed_struct & sMD)
{
  if (sMD.m_traded_price == 0 || !STool::IsValidPriceOrVol(sMD.m_traded_price)) return;

  const string & symbol = sMD.m_instrument;

  EExchange pri_ex = m_Exchg->GetPrimaryExchange(symbol);

  if (pri_ex != EX_HKFE && pri_ex != EX_CME) return;

  if (!m_Exchg->IsTradingHour(symbol,ymdhms)) return;

  //--------------------------------------------------
  // No need to check trading hour for CME
  //--------------------------------------------------

  string sFM1Code  = "";
  string sFM2Code  = "";
  string sUndlySym = "";
  string sMthCode  = "";

  if (pri_ex == EX_HKFE)
  {
    sFM1Code  = m_HKFE->GetCtrtCode1(HKFE::FUTURES,ymdhms.GetYYYYMMDD(),1);
    sFM2Code  = m_HKFE->GetCtrtCode1(HKFE::FUTURES,ymdhms.GetYYYYMMDD(),2);
    sUndlySym = symbol.substr(0,3);
    sMthCode  = symbol.substr(3,2);
  }
  else if (pri_ex == EX_CME)
  {
    sFM1Code  = m_CME->GetCtrtCode1(CME::FUTURES,ymdhms.GetYYYYMMDD(),1);
    sFM2Code  = m_CME->GetCtrtCode1(CME::FUTURES,ymdhms.GetYYYYMMDD(),2);
    sUndlySym = symbol.substr(0,2);
    sMthCode  = symbol.substr(2,2);
  }

  //--------------------------------------------------
  // Update raw contract last traded price
  //--------------------------------------------------
  boost::unique_lock<boost::shared_mutex> lock(m_ContFutMutex);

  if (sMthCode == sFM1Code)
  {
    m_LatestSnapshot_Sym_1FM[sUndlySym].m_traded_price = sMD.m_traded_price;
    m_LatestSnapshotUpdateTime_Sym_1FM[sUndlySym] = ymdhms;
  }
  else if (sMthCode == sFM2Code)
  {
    m_LatestSnapshot_Sym_2FM[sUndlySym].m_traded_price = sMD.m_traded_price;
    m_LatestSnapshotUpdateTime_Sym_2FM[sUndlySym] = ymdhms;
  }
  else
  {
    return;
  }

  //--------------------------------------------------
  // Roll forward if needed
  //--------------------------------------------------

  const YYYYMMDD & ymdToday = ymdhms.GetYYYYMMDD();
  bool bIsOnARollDate = false;

  if      (pri_ex == EX_HKFE && ymdToday == m_HKFE->GetExpy(ymdhms.GetYYYYMMDD(),1)) bIsOnARollDate = true;
  else if (pri_ex == EX_CME  &&             m_CME->IsRollDate(ymdhms.GetYYYYMMDD())) bIsOnARollDate = true;

  //--------------------------------------------------
  // Roll time is in HKT
  //--------------------------------------------------
  HHMMSS hmsRollTime;
  if      (pri_ex == EX_HKFE) hmsRollTime.Set(m_HKFE_RollFwdTime);
  else if (pri_ex == EX_CME)  hmsRollTime.Set(m_CME_RollFwdTime);


  //--------------------------------------------------
  if (bIsOnARollDate)
  {

    if (CheckIfContFutAlrRolled(sUndlySym,ymdToday))
    {
      if (sMthCode == sFM2Code)
      {
        UpdateContFutLatestSnapshot(ymdhms,sUndlySym,sMD,GetContFutAdjustment(sUndlySym));
      }
    }
    else
    {
      if (
        ymdhms.GetHHMMSS() >= hmsRollTime &&
        m_LatestSnapshot_Sym_1FM[sUndlySym].m_traded_price > 0 && abs(ymdhms - m_LatestSnapshotUpdateTime_Sym_1FM[sUndlySym]) < 5 && STool::IsValidPriceOrVol(m_LatestSnapshot_Sym_1FM[sUndlySym].m_traded_price) &&
        m_LatestSnapshot_Sym_2FM[sUndlySym].m_traded_price > 0 && abs(ymdhms - m_LatestSnapshotUpdateTime_Sym_2FM[sUndlySym]) < 5 && STool::IsValidPriceOrVol(m_LatestSnapshot_Sym_2FM[sUndlySym].m_traded_price)
        )
      {
        //--------------------------------------------------
        // Roll forward if the data is recent enought
        //--------------------------------------------------
        m_ContFutAdjustment[sUndlySym] = GetContFutAdjustment(sUndlySym)+(m_LatestSnapshot_Sym_1FM[sUndlySym].m_traded_price - m_LatestSnapshot_Sym_2FM[sUndlySym].m_traded_price);
        m_ContFutAlrRolled[sUndlySym].insert(ymdToday);
      }
      else if (sMthCode == sFM1Code)
      {
        UpdateContFutLatestSnapshot(ymdhms,sUndlySym,sMD,GetContFutAdjustment(sUndlySym));
      }
    }

  }
  else if (sMthCode == sFM1Code)
  {
    UpdateContFutLatestSnapshot(ymdhms,sUndlySym,sMD,GetContFutAdjustment(sUndlySym));
  }

  //--------------------------------------------------

  return;
}

void ContFut::UpdateContFutLatestSnapshot(const YYYYMMDDHHMMSS & ymdhms, const string & sUndlySym, const ATU_MDI_marketfeed_struct & sMD, const double price_offset)
{
  m_LatestSnapshot_Sym_c1[sUndlySym].m_traded_price  = sMD.m_traded_price  + price_offset;
  m_LatestSnapshot_Sym_c1[sUndlySym].m_traded_volume = sMD.m_traded_volume;
  m_LatestSnapshot_Sym_c1[sUndlySym].m_bid_price_1   = sMD.m_bid_price_1   + price_offset;
  m_LatestSnapshot_Sym_c1[sUndlySym].m_bid_volume_1  = sMD.m_bid_volume_1 ;
  m_LatestSnapshot_Sym_c1[sUndlySym].m_ask_price_1   = sMD.m_ask_price_1   + price_offset;
  m_LatestSnapshot_Sym_c1[sUndlySym].m_ask_volume_1  = sMD.m_ask_volume_1 ;
  m_LatestSnapshotUpdateTime_Sym_c1[sUndlySym].Set(ymdhms);
  m_Sym_c1Ready[sUndlySym] = true;
}

bool ContFut::CheckIfContFutAlrRolled(const string & sUndlySym,const YYYYMMDD & ymd)
{
  set<YYYYMMDD>::iterator it;
  it = m_ContFutAlrRolled[sUndlySym].find(ymd);
  if (it == m_ContFutAlrRolled[sUndlySym].end()) return false;
  else return true;
}

double ContFut::GetContFutAdjustment(const string & sUndlySym)
{
  return m_ContFutAdjustment[sUndlySym];
}

bool ContFut::GetContFutLatestSnapshot(const string & symbol, ATU_MDI_marketfeed_struct & mdi_struct, YYYYMMDDHHMMSS & ymdhms)
{
  boost::shared_lock<boost::shared_mutex> lock(m_ContFutMutex);

  string sUndlySym = "";

  EExchange pri_ex = m_Exchg->GetPrimaryExchange(symbol);
  sUndlySym = symbol.substr(0,symbol.length()-2);

  if (m_Sym_c1Ready[sUndlySym])
  {
    // memcpy(&mdi_struct,&m_LatestSnapshot_HSIc1,sizeof(ATU_MDI_marketfeed_struct));
    memcpy(&(mdi_struct.m_traded_price),&(m_LatestSnapshot_Sym_c1[sUndlySym].m_traded_price),sizeof(double)*22);
    ymdhms.Set(m_LatestSnapshotUpdateTime_Sym_c1[sUndlySym]);
    return true;
  }
  return false;
}

bool ContFut::GetContFutLatestSnapshot(const string & symbol, ATU_MDI_marketfeed_struct & mdi_struct)
{
  boost::shared_lock<boost::shared_mutex> lock(m_ContFutMutex);

  string sUndlySym = "";

  EExchange pri_ex = m_Exchg->GetPrimaryExchange(symbol);
  if      (pri_ex == EX_HKFE) sUndlySym = symbol.substr(0,3);
  else if (pri_ex == EX_CME ) sUndlySym = symbol.substr(0,2);

  if (m_Sym_c1Ready[sUndlySym])
  {
    // memcpy(&mdi_struct,&m_LatestSnapshot_HSIc1,sizeof(ATU_MDI_marketfeed_struct));
    memcpy(&(mdi_struct.m_traded_price),&(m_LatestSnapshot_Sym_c1[sUndlySym].m_traded_price),sizeof(double)*22);
    return true;
  }
  return false;
}

bool ContFut::IsSymContFut(const string & symbol)
{
  if (symbol.length() < 2) return false;
  string sSubStr = symbol.substr(symbol.length()-2,2);
  if (sSubStr == "c1" || sSubStr == "C1") return true;
  else                                    return false;
}

string ContFut::GetContFutUndly(const string & symbol)
{
  return symbol.substr(0,symbol.length()-2);
}


string ContFut::GetContFutFrRglrSym(const string & symbol, const int fm)
{
  if (fm != 1 && fm != 2) return "";
  return GetContFutUndly(symbol)+"c"+boost::lexical_cast<string>(fm);
}

string ContFut::GetContFutFrUndly(const string & undly, const int fm)
{
  if (fm != 1 && fm != 2) return "";
  return undly+"c"+boost::lexical_cast<string>(fm);
}

bool ContFut::DoesSymHaveContFut(const string & symbol)
{
  if (symbol.length() <= 2) return false;
  string sSubStr = symbol.substr(0,symbol.length()-2);

  set<string>::iterator it = m_SymList.find(sSubStr);
  if (it == m_SymList.end()) return false;
  else return true;
}


set<string> ContFut::GetAvbContFut()
{
  return m_SymList;
}
