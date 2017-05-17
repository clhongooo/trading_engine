#ifndef S10_H
#define S10_H

#include "RuleSys.h"
#include "../Math/OBLMA.h"
#include "../Util/HKFE.h"
#include <deque>
#include "../Util/SException.h"
#include "../Util/STool.h"

using namespace std;

class S10 : public RuleSys
{
	private:
    OBLMA _oblma;
    deque<double> _dqOBLMA;

		//Input parameters
		int _iPermittedTrdDir; //1 = Long only; -1 = Short only; 0 = Long Short allowed
		int _iReverse;

	public:
		const char * Name();

		S10(const double *);
		void DailyInit();
		void ResetChild();
		void Add(const BarProvider &);
};

#endif
