#ifndef TAYD_HPP
#define TAYD_HPP

#include <cmath>
#include <deque>
#include "RuleSys.h"
#include "../Math/Sma.hpp"
#include "../Math/Wma.hpp"
#include "../Math/Ema.hpp"
#include "../Util/SException.h"
#include "../Util/STool.h"

using namespace std;

class TayD : public RuleSys
{
	private:
		Wma<double> _wma;
		Ema<double> _ema10;
		Ema<double> _ema20;
		Ema<double> _ema19;
		Ema<double> _ema18;
		Ema<double> _ema17;
		Ema<double> _ema16;
    deque<double> _dqema10;
    deque<double> _dqema20;
    deque<double> _dqema19;
    deque<double> _dqema18;
    deque<double> _dqema17;
    deque<double> _dqema16;

	public:
		const char * Name();

		TayD();
		void DailyInit() {}
		void ResetChild();
		void Add(const BarProvider &);


};

#endif
