#ifndef R01_H
#define R01_H

#include "RuleSys.h"
#include "../Math/HighLow.hpp"
#include "../Util/SException.h"
#include "../Util/STool.h"
#include "../Util/HKFE.h"

using namespace std;

class R01 : public RuleSys
{
	private:
		int _iTrialTaken;
		double _dEntryPrice;
		HighLow<double> _hlDay;

		//Input parameters
		int _iMinB4Close; // Minutes before close
		double _dThresPctg; // Threshold
		double _dSLossPctg; // Stop loss from POE
		int _iReverse; // Reverse
		int _iMaxNoTrial ; // Max no trial
		int _iPermittedTrdDir; // Permitted trade direction

	public:
		const char * Name();

		R01(const double *);
		void DailyInit();
		void ResetChild();
		void Add(const BarProvider &);
};

#endif
