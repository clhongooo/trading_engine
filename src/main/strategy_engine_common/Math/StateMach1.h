#ifndef STATEMACH1_HPP
#define STATEMACH1_HPP

#include "PCH.h"
#include "Constants.h"
#include "StateMach.h"
#include "Sma.hpp"
#include "Wr.hpp"
#include "SException.h"
#include "STool.h"

using namespace std;

class StateMach1 : public StateMach
{
	private:
		Sma<double> _sma1;
		Sma<double> _sma2;
		Sma<double> _sma3;
		Wr<double> _wr1;
		deque<double> _dqVhsi;
		double _dH,_dL,_dC;

	public:
		int TotalStates();
		const char * Name();

		StateMach1();
		virtual ~StateMach1() {}
		void Add(double dH, double dL, double dC, double dVHSI);
		bool Ready() const;
		int State();
		string StateDesc(int);
};

#endif

