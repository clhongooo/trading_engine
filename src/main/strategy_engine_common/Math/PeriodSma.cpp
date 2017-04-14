#include "PeriodSma.h"

PeriodSma::PeriodSma() :
  m_d_RunningTotal(0),
  m_i_NoOfSeconds(0)
{
}

PeriodSma::PeriodSma(const long noofseconds)
{
  Reset(noofseconds);
}

PeriodSma::~PeriodSma()
{
}


void PeriodSma::Reset(const long noofseconds)
{
  m_i_NoOfSeconds = noofseconds;
  m_d_RunningTotal = 0;
  m_dq_yyyymmddhhmmss.clear();
  m_dq_dX.clear();
}

void PeriodSma::Add(const YYYYMMDDHHMMSS & ymdhms, double price)
{
  m_d_RunningTotal += price;
  m_dq_yyyymmddhhmmss.push_back(ymdhms);
  m_dq_dX.push_back(price);

  while (ymdhms - m_dq_yyyymmddhhmmss.front() >= m_i_NoOfSeconds)
  {
    m_d_RunningTotal -= m_dq_dX.front();
    m_dq_yyyymmddhhmmss.pop_front();
    m_dq_dX.pop_front();
  }
  return;
}

double PeriodSma::Value() const
{
  return m_d_RunningTotal / m_dq_dX.size();
}

bool PeriodSma::Ready() const
{
  return (!m_dq_dX.empty());
}

