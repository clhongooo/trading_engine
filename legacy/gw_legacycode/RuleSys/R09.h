#ifndef R09_H
#define R09_H

#include "RuleSys.h"
#include "../Util/SDateTime.h"
#include "../Util/STool.h"
#include "../Util/HKFE.h"

using namespace std;

class R09 : public RuleSys
{
	private:
		double _dStopLossPrice;
		double _dTakeProfitPrice;
    HMS _hmsFromTime;
    HMS _hmsToTime;

		//Input parameters
		double _dPctgStopLoss;
		double _dPctgTkPft;
		int _iFromHHMM;
		int _iToHHMM;

	public:
		const char * Name();

		R09(const double *);
		void DailyInit();
		void ResetChild();
		void Add(const BarProvider &);
};

#endif
