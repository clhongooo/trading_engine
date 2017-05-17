#ifndef R08_H
#define R08_H

#include "RuleSys.h"
#include "../Math/HighLow.hpp"
#include "../Math/LinRegr.h"
#include "../Util/SException.h"
#include "../Util/STool.h"

using namespace std;

class R08 : public RuleSys
{
	private:
		int _iCnt;
		double _dSlopeCurrent;
		double _dSlopeLast;
		double _dDayHigh;
		double _dEntryPrice;
		LinRegr _lr;
		HighLow<double> _hlShortTermHigh;
		HighLow<double> _hlProfitTarget;

		//Input parameters
		int _iRegrLag; //Regression lag
		double _dRegrSlopeMax; //Regression slope
		double _dRegrSlopeMin; //Regression slope
		int _iNBarHigh; //max(N bar high)
		double _dTkPftFraction; //Take profit fraction
		double _dSLFraction; //Stop loss fraction

	public:
		const char * Name();

		R08(const double *);
		void DailyInit();
		void ResetChild();
		void Add(const BarProvider &);
};

#endif
