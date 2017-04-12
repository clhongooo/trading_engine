#include <MeanAccumulator.h>

MeanAccumulator::MeanAccumulator() :
  m_Total(0),
  m_Count(0)
{
}

MeanAccumulator::~MeanAccumulator()
{
}



void MeanAccumulator::Add(const double d)
{
  if (!std::isnan(d))
  {
    m_Total += d;
    m_Count += 1;
  }
  return;
}

void MeanAccumulator::Reset()
{
  m_Total = 0;
  m_Count = 0;
}


double MeanAccumulator::GetMean() const
{
  if (!Ready()) return NAN;
  else          return (m_Total / (double) m_Count);
}

bool MeanAccumulator::Ready() const
{
  if (m_Count == 0) return false;
  else              return true;
}
