#include "OptionCtrt.h"

OptionCtrt::OptionCtrt() :
  m_Strike(0)
{
}

OptionCtrt::~OptionCtrt()
{
}

void OptionCtrt::Reset()
{
  //super->Reset();
  m_Strike = 0;
}
