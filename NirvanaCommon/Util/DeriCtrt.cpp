#include "DeriCtrt.h"

DeriCtrt::DeriCtrt() :
  m_Maturity_ymdhms(0)
{
  memset(m_UnderlyingSymbol,'\0',SYMSIZE);
}

DeriCtrt::DeriCtrt(const char * sUndSym, const unsigned long ulymdhms)
{
  memset(m_UnderlyingSymbol,'\0',SYMSIZE);
  strncpy(m_UnderlyingSymbol,sUndSym,STool::Min(strlen(sUndSym),SYMSIZE-7));
  m_Maturity_ymdhms = ulymdhms;
}

void DeriCtrt::Reset()
{
  memset(m_UnderlyingSymbol,'\0',SYMSIZE);
  m_Maturity_ymdhms = 0;
}

