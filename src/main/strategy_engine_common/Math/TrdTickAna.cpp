#include "TrdTickAna.h"

TrdTickAna::TrdTickAna(const unsigned short usRollgWinInMinutes) :
  m_usRollgWinInMinutes(usRollgWinInMinutes),
  m_dSum_Initr_x_Vol(0),
  m_lTotalVol(0),
  m_lTotalBuyerInitiated(0)
{
}

TrdTickAna::~TrdTickAna()
{
}

void TrdTickAna::Reset()
{
  m_dqTrdTick.clear();
  m_dSum_Initr_x_Vol = 0;
  m_lTotalVol = 0;
  m_lTotalBuyerInitiated = 0;
  return;
}

void TrdTickAna::Add(const HMS & hmsA, const long lVolume, const int iInitr)
{
  TradeTick tt;
  tt.hms = hmsA;
  tt.Price = 0;
  tt.Volume = lVolume;
  tt.Initiator = iInitr;
  Add(tt);
}

void TrdTickAna::Add(TradeTick & cur_trade_tick)
{
  m_dqTrdTick.push_back(cur_trade_tick);
  m_lTotalVol += cur_trade_tick.Volume;
  if (cur_trade_tick.Initiator > 0) m_lTotalBuyerInitiated++;
  m_dSum_Initr_x_Vol += (cur_trade_tick.Initiator * cur_trade_tick.Volume);

  //--------------------------------------------------
  // Remove out-dated items
  //--------------------------------------------------
  cur_trade_tick.hms.AddMinute(-m_usRollgWinInMinutes); // can modify cur_trade_tick after pushing it into the deque
  HMS hms5MinBeforeNow(cur_trade_tick.hms);
  while (m_dqTrdTick.front().hms <= hms5MinBeforeNow)
  {
    m_lTotalVol -= m_dqTrdTick.front().Volume;
    if (m_dqTrdTick.front().Initiator > 0) m_lTotalBuyerInitiated--;
    m_dSum_Initr_x_Vol -= (m_dqTrdTick.front().Initiator * m_dqTrdTick.front().Volume);
    m_dqTrdTick.pop_front();
  }
  return;
}

double TrdTickAna::IMBNmlzd()
{
  if (m_lTotalVol != 0) return (m_dSum_Initr_x_Vol / (double) m_lTotalVol * m_dqTrdTick.size());
  else                  return 0;
}
double TrdTickAna::IMBRaw()
{
  if (m_lTotalVol != 0) return m_dSum_Initr_x_Vol;
  else                  return 0;
}
unsigned long TrdTickAna::NoOfTrades()
{
  return m_dqTrdTick.size();
}

long TrdTickAna::TotalVolume()
{
  return m_lTotalVol;
}
double TrdTickAna::MeanVolume()
{
  if (!m_dqTrdTick.empty()) return (double) m_lTotalVol / (double) m_dqTrdTick.size();
  else                      return 0;
}
double TrdTickAna::ProbBuyerInitiated()
{
  if (!m_dqTrdTick.empty()) return (double) m_lTotalBuyerInitiated / (double) m_dqTrdTick.size();
  else                      return -1;
}

double TrdTickAna::CovInitr()
{
  if (!m_dqTrdTick.empty() && m_dqTrdTick.size() >= 2)
  {
    //--------------------------------------------------
    // Calculate mean
    //--------------------------------------------------
    double p = ProbBuyerInitiated();
    double u = p * 1 + (1-p) * -1;
    double dSumPrdt = 0;
    for (unsigned int i = 0; i < m_dqTrdTick.size()-1; ++i)
    {
      dSumPrdt += (((double) m_dqTrdTick[i].Initiator - u) * ((double) m_dqTrdTick[i+1].Initiator - u));
    }
    return dSumPrdt / (double) (m_dqTrdTick.size()-1);
  }
  return 0;
}

double TrdTickAna::CorrInitr()
{
  double dCov = CovInitr();
  if (dCov == 0) return 0;

  double p = ProbBuyerInitiated();
  double u = p * 1 + (1-p) * -1;
  double dSumPrdt = 0;
  for (unsigned int i = 0; i < m_dqTrdTick.size(); ++i)
  {
    dSumPrdt += pow((double) m_dqTrdTick[i].Initiator - u, 2);
  }
  double dVar = dSumPrdt / (double) m_dqTrdTick.size();
  return dCov / dVar;
}

// long TrdTickAna::MeanInterarrivalTimeMillisec()
// {
//   if (m_dqTrdTick.size() > 30) return (double) m_dqTrdTick.hms.: / (double) m_dqTrdTick.size();
//   else                         return -1;
// }
