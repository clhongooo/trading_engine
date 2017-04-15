#ifndef DERICTRT_H_
#define DERICTRT_H_

#include "PCH.h"
#include "Constants.h"
#include "STool.h"
#include "SDateTime.h"

using namespace std;
using namespace boost;

class DeriCtrt
{
  enum { SYMSIZE = 8 };

  public:
    DeriCtrt();
    DeriCtrt(const char *,const unsigned long);
    virtual ~DeriCtrt() = 0;
    virtual void Reset();

  private:
    char          m_UnderlyingSymbol[SYMSIZE];
    unsigned long m_Maturity_ymdhms;
};

#endif /* DERICTRT_H_ */
