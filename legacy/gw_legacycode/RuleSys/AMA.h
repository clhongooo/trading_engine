#ifndef AMA_H
#define AMA_H

#define AMANUMOFMA 4

#include "RuleSys.h"
#include "../Math/OBLMA.h"
#include "../Math/Sma.hpp"
#include "../Util/STool.h"
#include "../Util/HKFE.h"
#include <vector>
#include <deque>

using namespace std;

class AMA : public RuleSys
{
	public:
		const char * Name();

		AMA(const double *);
		void DailyInit();
		void ResetChild();
		void Add(const BarProvider &);

	private:
		OBLMA _oblma;
		vector<Sma<double> > _vSma;

		//Input parameters
		vector<int> _viSmaN;
		deque<long> _dqState;

    deque<double> _dDistOblma;
    vector<deque<double> > _dDistSma;

    long _lAll1State;
};

#endif
