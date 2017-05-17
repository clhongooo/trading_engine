#ifndef S04_H
#define S04_H

#include "RuleSys.h"
#include "../Math/Sma.hpp"
#include "../Math/MACD.h"
#include "../Math/HighLow.hpp"
#include "../Util/STool.h"
#include "../Util/SDateTime.h"

using namespace std;

class S04 : public RuleSys
{
	private:
		Sma<double> _smaBuyEntry;
		Sma<double> _smaSellEntry;
		Sma<double> _smaBuyExit;
		Sma<double> _smaSellExit;
    MACD _macd;
    HighLow<double> _highlow;
    double _dOpenPrice;
    YMD _ymdCurrentDay;

		//Input parameters
    double _dKBuy;
    double _dKSell;
    int _iHLPeriod;
    int _iSmaPeriodBuyEntry;
    int _iSmaPeriodSellEntry;
    int _iSmaPeriodBuyExit;
    int _iSmaPeriodSellExit;
    int _iMacdParam1;
    int _iMacdParam2;
    int _iMacdParam3;

	public:
		const char * Name();

		S04(const double *);
		void DailyInit();
		void ResetChild();
		void Add(const BarProvider &);
};

#endif
