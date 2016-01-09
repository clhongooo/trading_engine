#ifndef STATEMACH2_HPP
#define STATEMACH2_HPP

#include "PCH.h"
#include "Constants.h"
#include "StateMach.h"
#include "Sma.hpp"
#include "Atr.hpp"
#include "SgnlPrc.h"
#include "../Util/SException.h"
#include "../Util/STool.h"

using namespace std;

class StateMach2 : public StateMach
{
	private:
		SgnlPrc _sp;
		double _dH,_dL,_dC,_dVHSI;

		Sma<double> _sma_Period;
		Sma<double> _sma_ATR;
		Sma<double> _sma_OBLMA_1;
		Atr<double> _atr;
		Sma<double> _sma_VHSI;
		Sma<double> _sma_distPASMA;
		//mega cycle oscillator

	public:
		int TotalStates();
		const char * Name();

		StateMach2();
		virtual ~StateMach2() {}
		void Add(double dH, double dL, double dC, double dVHSI);
		bool Ready() const;
		int State();
		string StateDesc(int);
};

#endif

