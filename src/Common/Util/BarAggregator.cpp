#include "BarAggregator.h"

BarAggregator::BarAggregator() :
  m_BarInterval(UNSET),
  m_BarIsValid(false),
  // m_YYYYMMDD
  // m_HHMMSS
  m_Open(0),
  m_High(0),
  m_Low(0),
  m_Close(0),
  m_Volume(0)
{
}

BarAggregator::~BarAggregator()
{
}

void BarAggregator::SetBarInterval(BarInterval barinterval)
{
  m_BarInterval = barinterval;
}

void BarAggregator::AddTick(const YYYYMMDD & yyyymmdd, const HHMMSS & hhmmss, double dPrice, long lVolume)
{
  if (m_BarInterval == UNSET) return;

  switch (m_BarInterval)
  {
    case SECOND:
      {
        if (yyyymmdd > m_YYYYMMDD || hhmmss > m_HHMMSS) Cutoff();
        break;
      }
    case MINUTE:
      {
        if (yyyymmdd > m_YYYYMMDD || (hhmmss.ToInt() / 100) > (m_HHMMSS.ToInt() / 100)) Cutoff();
        break;
      }
    case HOUR:
      {
        if (yyyymmdd > m_YYYYMMDD || (hhmmss.ToInt() / 10000) > (m_HHMMSS.ToInt() / 10000)) Cutoff();
        break;
      }
    case DAY:
      {
        if (yyyymmdd > m_YYYYMMDD) Cutoff();
        break;
      }
    default:
      {

        break;
      }
  }

  if (!m_BarIsValid)
  {
    //--------------------------------------------------
    // Init
    //--------------------------------------------------
    m_YYYYMMDD    = yyyymmdd;
    m_HHMMSS      = hhmmss;
    m_Open        = dPrice;
    m_High        = dPrice;
    m_Low         = dPrice;
    m_Close       = dPrice;
    m_Volume      = lVolume;
    m_BarIsValid  = true;
  }
  else
  {
    m_YYYYMMDD                  = yyyymmdd;
    m_HHMMSS                    = hhmmss;
    if (dPrice > m_High) m_High = dPrice;
    if (dPrice < m_Low) m_Low   = dPrice;
    m_Close                     = dPrice;
    m_Volume                   += lVolume;
  }

  return;
}

void BarAggregator::Cutoff()
{
  if (m_BarInterval == UNSET) return;
  if (!m_BarIsValid) return;

  m_dq_YYYYMMDD .push_back(m_YYYYMMDD);
  m_dq_HHMMSS   .push_back(m_HHMMSS);
  m_dq_Open     .push_back(m_Open);
  m_dq_High     .push_back(m_High);
  m_dq_Low      .push_back(m_Low);
  m_dq_Close    .push_back(m_Close);
  m_dq_Volume   .push_back(m_Volume);

  //--------------------------------------------------
  // Reset individual
  //--------------------------------------------------
  m_BarIsValid  = false;
  // m_YYYYMMDD
  // m_HHMMSS
  m_Open     = 0;
  m_Close    = 0;
  m_High     = 0;
  m_Low      = 0;
  m_Volume   = 0;
}

bool BarAggregator::GetOHLCBar(YYYYMMDD & yyyymmdd, HHMMSS & hhmmss, double & dOpen, double & dHigh, double & dLow, double & dClose, long & lVolume)
{
  if (m_BarInterval == UNSET) return false;
  if (m_dq_Open.empty()) return false;

  yyyymmdd = m_dq_YYYYMMDD.front();
  hhmmss   = m_dq_HHMMSS.front();
  dOpen    = m_dq_Open.front();
  dHigh    = m_dq_High.front();
  dLow     = m_dq_Low.front();
  dClose   = m_dq_Close.front();
  lVolume  = m_dq_Volume.front();

  m_dq_YYYYMMDD .pop_front();
  m_dq_HHMMSS   .pop_front();
  m_dq_Open     .pop_front();
  m_dq_Close    .pop_front();
  m_dq_High     .pop_front();
  m_dq_Low      .pop_front();
  m_dq_Volume   .pop_front();

  return true;
}

bool BarAggregator::PeekOHLCBar(YYYYMMDD & yyyymmdd, HHMMSS & hhmmss, double & dOpen, double & dHigh, double & dLow, double & dClose, long & lVolume)
{
  yyyymmdd =  m_YYYYMMDD;
  hhmmss   =  m_HHMMSS;
  dOpen    =  m_Open;
  dHigh    =  m_High;
  dLow     =  m_Low;
  dClose   =  m_Close;
  lVolume  =  m_Volume;

  return true;
}

void BarAggregator::DebugPrintContents()
{
  cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") No of completed bars: " << m_dq_YYYYMMDD.size() << endl << flush;

  for (unsigned int i = 0; i < m_dq_YYYYMMDD.size(); ++i)
  {
    cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") First completed bar: " <<
      m_dq_YYYYMMDD[i].ToInt() << " " <<
      m_dq_HHMMSS[i].ToInt() << " " <<
      m_dq_Open[i] << " " <<
      m_dq_High[i] << " " <<
      m_dq_Low[i] << " " <<
      m_dq_Close[i] << " " <<
      m_dq_Volume[i] << " " <<
      endl << flush;
  }

  cout << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") Incompleted bar: " <<
    m_YYYYMMDD.ToInt() << " " <<
    m_HHMMSS.ToInt() << " " <<
    m_Open << " " <<
    m_High << " " <<
    m_Low << " " <<
    m_Close << " " <<
    m_Volume << " " <<
    endl << flush;

  return;
}

