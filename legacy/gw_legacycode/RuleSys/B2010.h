#ifndef B2010_H
#define B2010_H

#include <cmath>
#include "RuleSys.h"
#include "../Math/Sma.hpp"
#include "../Math/Atr.hpp"
#include "../Util/SException.h"
#include "../Util/STool.h"

using namespace std;

class B2010 : public RuleSys
{
	private:
		Sma<double> _sma3;
		Sma<double> _sma5;
		Sma<double> _sma10;
		Atr<double> _atr;

		//Input parameters
		double _dInitSize[3]; //B3, B5, B10

	public:
		const char * Name();

		B2010(const double *);
		void ResetChild();
		void DailyInitChild() {}
		void Add(const BarProvider &) {
			SException se;
			se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
			throw se;
		}
		void Add(const BarProvider &, bool);


};

#endif
