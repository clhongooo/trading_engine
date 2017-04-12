#include <RegularMiniCtrtManager.h>

RegularMiniCtrtManager::RegularMiniCtrtManager() : m_RMCM_Mode(RMCM_OPTZDELTA)
{
  m_Exchg = Exchange::Instance();
}

RegularMiniCtrtManager::~RegularMiniCtrtManager()
{
}

void RegularMiniCtrtManager::SetMode(const RMCM_Mode m)
{
  m_RMCM_Mode = m;
}

long RegularMiniCtrtManager::GetSuggestedCtrtSize(const string & symbol) const
{
  map<string,long>::const_iterator it = m_SuggestedPos.find(symbol);
  if (it == m_SuggestedPos.end()) return 0;
  return it->second; 
}

double RegularMiniCtrtManager::GetRegularCtrtTheoPos(const string & symbol)
{
  map<string,double>::iterator it = m_RegularCtrtTheoPos.find(symbol);
  if (it == m_RegularCtrtTheoPos.end()) return 0;
  return it->second;
}

void RegularMiniCtrtManager::SetRegularCtrtTheoPos(const string & symbol, const double theopos)
{
  m_RegularCtrtTheoPos[symbol] = theopos;
}

void RegularMiniCtrtManager::UpdateRegularCtrtTgtSgnl(const string & sRglrSym, const double dTgtRglrTheoPos)
{
  if (!m_Exchg->IsRegularFutCtrt(sRglrSym)) return;

  string sMiniSym = m_Exchg->GetMiniFrRglrFutCtrt(sRglrSym);

  const double dRglrMul = m_Exchg->GetContractMultiplier(sRglrSym);
  const double dMiniMul = m_Exchg->GetContractMultiplier(sMiniSym);
  const double dRglrMiniRatio = dRglrMul / dMiniMul;

  //--------------------------------------------------
  // Convert regular to mini, and truncate the odd amount
  //--------------------------------------------------
  const double dTgtMiniTheoPos = dTgtRglrTheoPos * dRglrMiniRatio;
  const double dTgtMiniTheoPosFloored = STool::Sign(dTgtMiniTheoPos) * floor(abs(dTgtMiniTheoPos));

  if (m_RMCM_Mode == RMCM_OPTZMARGINREQ)
  {
    const long lRglrPos = STool::Sign(dTgtMiniTheoPosFloored) * (long)floor(abs(dTgtMiniTheoPosFloored / dRglrMiniRatio));
    m_SuggestedPos[sRglrSym] = lRglrPos;
    m_SuggestedPos[sMiniSym] = (long)round((dTgtMiniTheoPosFloored - (double)lRglrPos * dRglrMiniRatio));
  }
  else if (m_RMCM_Mode == RMCM_OPTZDELTA)
  {
    const double dExistgRglrTheoPos = GetRegularCtrtTheoPos(sRglrSym);
    const double dExistgMiniTheoPos = dExistgRglrTheoPos * dRglrMiniRatio;
    const double dMiniTheoPosDelta  = dTgtMiniTheoPosFloored - dExistgMiniTheoPos;
    const long   lRglrDelta         = STool::Sign(dMiniTheoPosDelta) * (long)round(abs(dMiniTheoPosDelta / dRglrMiniRatio));

    m_SuggestedPos[sRglrSym] += lRglrDelta;
    m_SuggestedPos[sMiniSym] += (long)round((dMiniTheoPosDelta - (double)lRglrDelta * dRglrMiniRatio));
  }

  SetRegularCtrtTheoPos(sRglrSym,dTgtMiniTheoPosFloored/dRglrMiniRatio);

  return;
}

void RegularMiniCtrtManager::NetOffRndOffRegularMini()
{
  FForEach (m_RegularCtrtTheoPos,[&](const std::pair<string,double> & rglrtheo) {
    const string & sRglrSym = rglrtheo.first;
    long lRglrSugSize = m_SuggestedPos[sRglrSym];
    const string sMiniSym = m_Exchg->GetMiniFrRglrFutCtrt(sRglrSym);
    long lMiniSugSize = m_SuggestedPos[sMiniSym];

    //--------------------------------------------------
    // No need to do anything if both are zero
    //--------------------------------------------------
    if (lRglrSugSize == 0 && lMiniSugSize == 0) return;
    //--------------------------------------------------

    //--------------------------------------------------
    const double dRglrMul = m_Exchg->GetContractMultiplier(sRglrSym);
    const double dMiniMul = m_Exchg->GetContractMultiplier(sMiniSym);
    const double dRglrMiniRatio = dRglrMul / dMiniMul;

    if (abs(lMiniSugSize) > 0 && lRglrSugSize * lMiniSugSize >= 0)
    {
      //--------------------------------------------------
      // Round off
      //--------------------------------------------------
      long lRndOffSizeInRglr = abs(lMiniSugSize) / (long)dRglrMiniRatio;

      if (lRndOffSizeInRglr > 0)
      {
        m_SuggestedPos[sRglrSym] = lRglrSugSize + (lMiniSugSize > 0 ? 1 : -1) * lRndOffSizeInRglr;
        m_SuggestedPos[sMiniSym] = lMiniSugSize - (lMiniSugSize > 0 ? 1 : -1) * lRndOffSizeInRglr * dRglrMiniRatio;
      }
    }
    else if (lRglrSugSize * lMiniSugSize < 0)
    {
      //--------------------------------------------------
      // Net off
      //--------------------------------------------------
      long lNetOffSizeInRglr = min(abs(lMiniSugSize) / (long)dRglrMiniRatio, abs(lRglrSugSize));

      if (lNetOffSizeInRglr > 0)
      {
        m_SuggestedPos[sRglrSym] = lRglrSugSize - (lRglrSugSize > 0 ? 1 : -1) * lNetOffSizeInRglr;
        m_SuggestedPos[sMiniSym] = lMiniSugSize - (lMiniSugSize > 0 ? 1 : -1) * lNetOffSizeInRglr * dRglrMiniRatio;
      }
    }
    //--------------------------------------------------

  });
  return;
}

