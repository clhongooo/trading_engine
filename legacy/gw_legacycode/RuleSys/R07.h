#ifndef R07_H
#define R07_H

#include "RuleSys.h"
#include "../Math/SNYStat.h"
#include "../Util/STool.h"
#include "../Util/HKFE.h"

using namespace std;

class R07 : public RuleSys
{
	private:
		SNYStat _snyStat;
    HMS _hmsFromTime;
    HMS _hmsToTime;
    double _dStopLossPrice;

		//Input parameters
		int _iPeriod;
		double _dBandDist;
		double _dPctgStopLoss;
		int _iFromHHMM;
		int _iToHHMM;

	public:
		const char * Name();

		R07(const double *);
		void DailyInit();
		void ResetChild();
		void Add(const BarProvider &);
};

#endif
