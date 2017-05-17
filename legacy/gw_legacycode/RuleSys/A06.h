#ifndef A06_H
#define A06_H

#include "RuleSys.h"
#include "../Math/Atr.hpp"
#include "../Math/HighLow.hpp"
#include "../Util/SException.h"
#include "../Util/STool.h"
#include "../Util/HKFE.h"

using namespace std;

class A06 : public RuleSys
{
	private:
		Atr<double> _atr;
		HighLow<double> _hlChnlEntry;
		HighLow<double> _hlChnlExit;
		double _dEntryPrice;

		//Input parameters
		int _iEntryWindow; // Entry
		int _iExitWindow; // Exit
		double _dTkPftFrac; // Take profit fraction (of Entry Channel height)
		double _dSLFrac; // Stop loss fraction (of Entry Channel height)
		double _dMaxChnlHeight; // Max channel height
		double _dAtrAlpha; // ATR alpha
		int _iPermittedTrdDir; // 1 = Long only; -1 = Short only; 0 = Long Short allowed
		int _iReverse; // Reverse

	public:
		const char * Name();

		A06(const double *);
		void DailyInit();
		void ResetChild();
		void Add(const BarProvider &);
};

#endif
