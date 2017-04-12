#include <NumberPosition.h>

NumberPosition::NumberPosition()
{
}

NumberPosition::~NumberPosition()
{
}


bool NumberPosition::IsPriceApart(const double dAtLeastApart, const double dCurNum) const
{
  if (m_setNum.empty()) return true;

  set<double>::iterator it = m_setNum.begin();
  double dSmallest = *it;


  it = m_setNum.end(); it--;
  double dLargest = *it;

  if (dCurNum > dLargest + dAtLeastApart || dCurNum < dSmallest - dAtLeastApart) return true;

  return false;
}

void NumberPosition::Reset()
{
  m_setNum.clear();
}

void NumberPosition::AddNum(const double dCurNum)
{
  m_setNum.insert(dCurNum);
}

void NumberPosition::RemoveNum(const double dNum)
{
  m_setNum.erase(dNum);
}

size_t NumberPosition::Size()
{
  return m_setNum.size();
}
