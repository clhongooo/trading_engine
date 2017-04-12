#ifndef OPTIONCTRT_H_
#define OPTIONCTRT_H_

#include "PCH.h"
#include "Constants.h"
#include "../Util/STool.h"
#include "DeriCtrt.h"

using namespace std;
using namespace boost;

class OptionCtrt : public DeriCtrt
{
  public:
    OptionCtrt();
    virtual ~OptionCtrt();
    void Reset();

  private:
    unsigned long m_Strike;
};

#endif /* OPTIONCTRT_H_ */
