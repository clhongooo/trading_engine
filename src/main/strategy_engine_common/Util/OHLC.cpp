#include <OHLC.h>

OHLC::OHLC() :
  m_Open       (NAN),
  m_High       (NAN),
  m_Low        (NAN),
  m_Close      (NAN),
  m_Initialized(false)
{
}

OHLC::~OHLC()
{
}


void OHLC::Add(const double p)
{
  if (!m_Initialized)
  {
    m_Open  = p;
    m_High  = p;
    m_Low   = p;
    m_Close = p;
  }
  else
  {
    if (p > m_High) m_High = p;
    if (p < m_Low ) m_Low  = p;
    m_Close = p;
  }

  m_Initialized = true;
  return;
}


void OHLC::Add(const double o,const double h,const double l,const double c)
{
  if (!m_Initialized)
  {
    m_Open  = o;
    m_High  = h;
    m_Low   = l;
    m_Close = c;
  }
  else
  {
    if (h > m_High) m_High = h;
    if (l < m_Low ) m_Low  = l;
    m_Close = c;
  }

  m_Initialized = true;
  return;
}


void OHLC::Reset()
{
  m_Initialized = false;
  m_Open  = NAN;
  m_High  = NAN;
  m_Low   = NAN;
  m_Close = NAN;
  return;
}


double OHLC::Open  () const { if (m_Initialized) return m_Open ; else return NAN; }
double OHLC::High  () const { if (m_Initialized) return m_High ; else return NAN; }
double OHLC::Low   () const { if (m_Initialized) return m_Low  ; else return NAN; }
double OHLC::Close () const { if (m_Initialized) return m_Close; else return NAN; }

