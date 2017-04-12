#include "TickReturn.h"

const double TickReturn::m_undefined = -999;

TickReturn::TickReturn() : m_b_Ready(false)
{
}

TickReturn::TickReturn(const long lRollgWinInSeconds) :
  m_lRollgWinInSeconds(lRollgWinInSeconds),
  m_b_Ready(false)
{
}

TickReturn::~TickReturn()
{
}

void TickReturn::Reset()
{
  m_dqYMDHMS.clear();
  m_dqTradePrice.clear();
  m_b_Ready = false;
  return;
}

void TickReturn::Reset(const long lRollgWinInSeconds)
{
  m_lRollgWinInSeconds = lRollgWinInSeconds;
  m_dqYMDHMS.clear();
  m_dqTradePrice.clear();
  return;
}


bool TickReturn::Ready()
{
  return m_b_Ready;
}

void TickReturn::Add(const YYYYMMDDHHMMSS & ymdhmsA, const double dTrdPx)
{
  m_dqYMDHMS.push_back(ymdhmsA);
  m_dqTradePrice.push_back(dTrdPx);

  //--------------------------------------------------
  // Remove out-dated items
  //--------------------------------------------------
  while (abs(ymdhmsA - m_dqYMDHMS.front()) > m_lRollgWinInSeconds)
  {
    m_dqYMDHMS.pop_front();
    m_dqTradePrice.pop_front();
    m_b_Ready = true;
  }
  return;
}

double TickReturn::Return()
{
  if (m_b_Ready && m_dqTradePrice.size() >= 2) return m_dqTradePrice.back() / m_dqTradePrice.front() - 1;
  else return m_undefined;
}

double TickReturn::ReturnR()
{
  if (m_b_Ready && m_dqTradePrice.size() >= 2) return m_dqTradePrice.back() / m_dqTradePrice.front();
  else return m_undefined;
}

double TickReturn::LnReturnR()
{
  if (m_b_Ready && m_dqTradePrice.size() >= 2) return log (m_dqTradePrice.back() / m_dqTradePrice.front());
  else return m_undefined;
}
