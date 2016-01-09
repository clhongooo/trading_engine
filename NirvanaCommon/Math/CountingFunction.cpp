#include <CountingFunction.h>

CountingFunction::CountingFunction()
{
}

CountingFunction::~CountingFunction()
{
}


void CountingFunction::Add(const bool bArg)
{
  //--------------------------------------------------
  // init
  //--------------------------------------------------
  if (m_NumList.empty())
  {
    if (bArg)
    {
      m_NumList.push_back(1);
    }
    else
    {
      m_NumList.push_back(0);
    }
  }
  else
  {
    if (bArg)
    {
      m_NumList.push_back(m_NumList.back()+1);
    }
    else
    {
      m_NumList.push_back(m_NumList.back());
    }
  }

}


size_t CountingFunction::Size() const
{
  return m_NumList.size();
}

long CountingFunction::Count(const long a, const long b) const
{
  return CountInclusive(a,b-1);
}

long CountingFunction::CountInclusive(const long a, const long b) const
{
  if (a == b) return 0;

  if (
      m_NumList.size() <= a ||
      m_NumList.size() <= b
     )
    return -1;

  if (a > b) return -1;

  return (m_NumList[b] - m_NumList[a]);

}

long CountingFunction::CountLastNElements(const long a) const
{
  long lLastEle = m_NumList.size()-1;
  return Count(lLastEle-a+1,lLastEle);
}

void CountingFunction::Reset()
{
  m_NumList.clear();
  return;
}


long CountingFunction::HowManyElemFromTailToAccum(const long a) const
{
  if (m_NumList.empty()) return -1;

  long lLoc = m_NumList.size()-1;
  long lLast = lLoc;

  while (lLoc > 0)
  {
    if (m_NumList[lLast] - m_NumList[lLoc] >= a) break;
    lLoc--;
  }

  return (lLast-lLoc+1);
}

long CountingFunction::HowManyElemFromTailToAccumOpp(const long a) const
{
  if (m_NumList.empty()) return -1;

  long lLoc = m_NumList.size()-1;
  long lLast = lLoc;

  while (lLoc > 0)
  {
    if ((lLast-lLoc) - (m_NumList[lLast] - m_NumList[lLoc]) >= a) break;
    lLoc--;
  }

  return (lLast-lLoc+1);
}

