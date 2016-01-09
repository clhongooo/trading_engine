#include "ProbDistrnFunc.h"

ProbDistrnFunc::ProbDistrnFunc()
{
}

ProbDistrnFunc::~ProbDistrnFunc()
{
}


ProbDistrnFuncBld::ProbDistrnFuncBld()
{
}

ProbDistrnFuncBld::~ProbDistrnFuncBld()
{
}


double ProbDistrnFunc::GetPr(const unsigned long ymdhms, const double dRangeStart, const double dRangeEnd)
{
  map<unsigned long, boost::shared_mutex*>::iterator itm;
  itm = m_CumProbMutex.find(ymdhms);
  if (itm == m_CumProbMutex.end())
  {
    m_CumProbMutex[ymdhms] = new boost::shared_mutex();
  }

  boost::shared_lock<boost::shared_mutex> lock(*(m_CumProbMutex[ymdhms]));

  map<unsigned long, map<unsigned long,unsigned long> >::iterator it;
  it = m_CumProb.find(ymdhms);
  if (it == m_CumProb.end())
  {
    return 0;
  }
  else
  {
    // map<double,double>::iterator it2;
    // it2 = 
    //   (it->second);
    return 0;
  }

}

bool ProbDistrnFunc::CheckValidity(const unsigned long ymdhms)
{
  return true;
}


bool ProbDistrnFuncBld::GenerateDistrnFunc(Method meth, const unsigned long ymdhms)
{
  return true;
}


bool ProbDistrnFuncBld::GetProbDistrnFunc(const unsigned long ymdhms, ProbDistrnFunc & pdf)
{
  return true;
}

