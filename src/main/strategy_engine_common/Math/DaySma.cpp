#include "DaySma.h"

DaySma::DaySma() :
  m_d_RunningTotal(0),
  m_i_NoOfDays(0)
{
}

DaySma::DaySma(const int noofdays)
{
  Reset(noofdays);
}

DaySma::~DaySma()
{
}


void DaySma::Reset(const int noofdays)
{
  m_i_NoOfDays = noofdays;
  m_d_RunningTotal = 0;
  m_dq_yyyymmdd.clear();
  m_dq_dX.clear();
}

void DaySma::Add(const YYYYMMDD & ymd, double price)
{
  m_d_RunningTotal += price;
  m_dq_yyyymmdd.push_back(ymd);
  m_dq_dX.push_back(price);

  while (abs(SDateTime::CalendarDayDiff(m_dq_yyyymmdd.front(),ymd)) >= m_i_NoOfDays)
  {
    m_d_RunningTotal -= m_dq_dX.front();
    m_dq_yyyymmdd.pop_front();
    m_dq_dX.pop_front();
  }
  return;
}

double DaySma::Value() const
{
  return m_d_RunningTotal / m_dq_dX.size();
}

bool DaySma::Ready() const
{
  return (!m_dq_dX.empty());
}

