#ifndef FSMC_HPP
#define FSMC_HPP

#include "PCH.h"
#include "Constants.h"
using namespace std;

class FSMC
{
	private:

	public:
		//self introduction
		virtual int TotalStates()=0; //total number of states
		virtual const char * Name()=0; //name

		virtual bool Ready() const =0;
		virtual int State()=0; //current state
		virtual string StateDesc(int)=0; //describe what the state means

		virtual void Add(double,double,double,double) {}
		virtual void Add(double,double,double) {}
};

#endif
