#include "GKYZ.h"

GKYZ::GKYZ()
{
  Reset(20,true);
}

GKYZ::GKYZ(const unsigned int winsize)
{
  Reset(winsize,true);
}

//--------------------------------------------------
// Copy constructor
//--------------------------------------------------
GKYZ::GKYZ(const GKYZ & gkyz)
{
  this->m_WinSize  = gkyz.m_WinSize;
  this->m_PrevClose = gkyz.m_PrevClose;

  this->m_sumLogOoverPreviousCSquared = gkyz.m_sumLogOoverPreviousCSquared;
  this->m_sumLogHoverLSquared         = gkyz.m_sumLogHoverLSquared;
  this->m_sumLogCoverOSquared         = gkyz.m_sumLogCoverOSquared;

  this->m_dq_LogOoverPreviousCSquared   = gkyz.m_dq_LogOoverPreviousCSquared;
  this->m_dq_LogHoverLSquared           = gkyz.m_dq_LogHoverLSquared;
  this->m_dq_LogCoverOSquared           = gkyz.m_dq_LogCoverOSquared;

}

//--------------------------------------------------
// Assignment operator
//--------------------------------------------------

GKYZ& GKYZ::operator=(const GKYZ & gkyz)
{
  if(this == &gkyz)
    return *this;

  this->m_WinSize  = gkyz.m_WinSize;
  this->m_PrevClose = gkyz.m_PrevClose;

  this->m_sumLogOoverPreviousCSquared = gkyz.m_sumLogOoverPreviousCSquared;
  this->m_sumLogHoverLSquared         = gkyz.m_sumLogHoverLSquared;
  this->m_sumLogCoverOSquared         = gkyz.m_sumLogCoverOSquared;

  this->m_dq_LogOoverPreviousCSquared   = gkyz.m_dq_LogOoverPreviousCSquared;
  this->m_dq_LogHoverLSquared           = gkyz.m_dq_LogHoverLSquared;
  this->m_dq_LogCoverOSquared           = gkyz.m_dq_LogCoverOSquared;

  return *this;
}

GKYZ::GKYZ(const unsigned int winsize, bool bC)
{
  Reset(winsize,bC);
}

void GKYZ::Reset(const unsigned int winsize, bool bC)
{
  this->_iTargetSize = 0;
  this->_bCheckDataAsPrice = bC;

  m_WinSize = winsize;

  m_PrevClose = 0;
  m_dq_LogOoverPreviousCSquared.clear();
  m_dq_LogHoverLSquared.clear();
  m_dq_LogCoverOSquared.clear();

  m_sumLogOoverPreviousCSquared = 0;
  m_sumLogHoverLSquared = 0;
  m_sumLogCoverOSquared = 0;

}

bool GKYZ::Ready() const
{
  if (m_dq_LogOoverPreviousCSquared.size() >= m_WinSize) return true;
  else return false;
}

void GKYZ::AddChild(double dO,double dH,double dL,double dC)
{

  if (m_PrevClose == 0)
  {
    m_PrevClose = dC;
    return;
  }

  double dLogOoverPreviousCSquared = pow(log(dO / m_PrevClose),2);
  double dLogHoverLSquared         = pow(log(dH / dL),2);
  double dLogCoverOSquared         = pow(log(dC / dO),2);

  m_sumLogOoverPreviousCSquared += dLogOoverPreviousCSquared;
  m_sumLogHoverLSquared         += dLogHoverLSquared;
  m_sumLogCoverOSquared         += dLogCoverOSquared;

  m_dq_LogOoverPreviousCSquared.push_back(dLogOoverPreviousCSquared);
  m_dq_LogHoverLSquared        .push_back(dLogHoverLSquared);
  m_dq_LogCoverOSquared        .push_back(dLogCoverOSquared);

  //--------------------------------------------------
  // queue size exceeds window size, pop the first one
  //--------------------------------------------------
  if (m_dq_LogOoverPreviousCSquared.size() > m_WinSize)
  {

    m_sumLogOoverPreviousCSquared -= m_dq_LogOoverPreviousCSquared.front();
    m_sumLogHoverLSquared         -= m_dq_LogHoverLSquared.front();
    m_sumLogCoverOSquared         -= m_dq_LogCoverOSquared.front();

    m_dq_LogOoverPreviousCSquared.pop_front();
    m_dq_LogHoverLSquared.pop_front();
    m_dq_LogCoverOSquared.pop_front();

  }

  //--------------------------------------------------
  // Update
  //--------------------------------------------------
  m_PrevClose = dC;

}

double GKYZ::Value() const
{
  if (!Ready()) return NAN;

  // cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << m_sumLogOoverPreviousCSquared << endl << flush;
  // cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << m_sumLogHoverLSquared << endl << flush;
  // cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << m_sumLogCoverOSquared << endl << flush;
  // cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << DAYS_IN_YEAR << endl << flush;
  // cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << m_WinSize << endl << flush;

  return sqrt(m_sumLogOoverPreviousCSquared + (m_sumLogHoverLSquared/2) - ((2*log(2))-1) * m_sumLogCoverOSquared) * sqrt((double)DAYS_IN_YEAR / m_WinSize);
}

void GKYZ::debugPrint()
{
  cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << m_sumLogOoverPreviousCSquared << endl << flush;
  cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << m_sumLogHoverLSquared << endl << flush;
  cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << m_sumLogCoverOSquared << endl << flush;
  cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << DAYS_IN_YEAR << endl << flush;
  cout << "SunnyDebug: " << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << m_WinSize << endl << flush;
}

