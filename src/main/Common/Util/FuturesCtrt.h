#ifndef FUTURESCTRT_H_
#define FUTURESCTRT_H_

#include "PCH.h"
#include "Constants.h"
#include "../Util/STool.h"
#include "DeriCtrt.h"

using namespace std;
using namespace boost;

class FuturesCtrt : public DeriCtrt
{
  public:
    FuturesCtrt();
    virtual ~FuturesCtrt();
    void Reset();

};

#endif /* FUTURESCTRT_H_ */
