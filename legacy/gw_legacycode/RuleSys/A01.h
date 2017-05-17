#ifndef A01_H
#define A01_H

#include "RuleSys.h"
#include "../Math/Sma.hpp"
#include "../Util/SException.h"
#include "../Util/STool.h"

using namespace std;

class A01 : public RuleSys
{
	private:
		Sma<double> _smaEntry1;
		Sma<double> _smaEntry2;
		Sma<double> _smaExit1;
		Sma<double> _smaExit2;

		//Input parameters
		int _iEntry1;
		int _iEntry2;
		int _iExit1;
		int _iExit2;
		int _iPermittedTrdDir; //1 = Long only; -1 = Short only; 0 = Long Short allowed
		int _iReverse;

	public:
		const char * Name();

		A01(const double *);
		void DailyInit();
		void ResetChild();
		void Add(const BarProvider &);
};

#endif
