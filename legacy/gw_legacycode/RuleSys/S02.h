#ifndef S02_H
#define S02_H

#include <string>
#include "RuleSys.h"
#include "../Math/HighLow.hpp"
#include "../Math/LinRegr.h"
#include "../Math/Sma.hpp"
#include "../Util/STool.h"
#include "../Util/HKFE.h"
#include "../Util/CustType.h"
#include "../Util/SDateTime.h"

using namespace std;


class S02 : public RuleSys
{
  private:

    Sma<double> smaLeg1;
    Sma<double> smaLeg2;

    int iCondition;
    int iTradeDir;

    double dLeg1chgAtMrngRef;
    double dLeg2chgAtMrngRef;
    double dLeg1chgAtAftnRef;
    double dLeg2chgAtAftnRef;

    double dLeg1MrngClosePrice;
    double dLeg2MrngClosePrice;
    double dLeg1AftnClosePrice;
    double dLeg2AftnClosePrice;

    double dLeg1CurrentPrice;
    double dLeg2CurrentPrice;

    // Input parameters
    string _sLeg1Sym;
    string _sLeg2Sym;
    double _dLeg1_1st2ndLevBoundary;
    double _dLeg2_1st2ndLevBoundary;
    HMS    _hmsMrngRefTime;
    HMS    _hmsAftnRefTime;
    double _dMrngLeg2ConfPctg;
    double _dAftnLeg2ConfPctg;
    int _iSmaPeriod;


  public:
    const char * Name();

    S02(const string &, const string &, const double *);
    void ResetChild();
    void DailyInit();
    void Add(const BarProvider &) {}
    void Add(const string &, const BarProvider &);

    // --------------------------------
    int DetLeg1_1st2ndLevel(double, double);
    int DetLeg2_1st2ndLevel(double, double);
    void DetermineMrngAction(const string &, const BarProvider &);
    void DetermineAftnAction(const string &, const BarProvider &);

};

#endif
