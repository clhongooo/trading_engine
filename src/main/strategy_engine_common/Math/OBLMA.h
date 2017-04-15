#ifndef OBLMA_H
#define OBLMA_H

#include "PCH.h"
#include "Constants.h"
#include "STool.h"
#include "Ema.hpp"
#include "DataQue.hpp"

class OBLMA : public DataQue<double>
{
	private:
		//Prices
		deque<double> dqHistM;
		Ema<double> ema10_M;
		deque<double> dq_ema10_M;

		//One-bar Lag Moving Average
		deque<double> dq_1bl_wma_431;
		deque<double> dq_1bl_wma_42;
		deque<double> dq_1bl_taylor;

		void Reset(int,bool) {}

		void AddChild(double) {}
		void AddChild(double,double) {}
		void AddChild(double dH, double dL, double dC);
		void AddChild(double,double,double,double) {}
		void AddChild(double,double,double,double,double) {}

	public:
		OBLMA();
		virtual ~OBLMA() {}
		bool Ready() const;
		void Reset();
    double Value();
};

#endif
