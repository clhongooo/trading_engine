#ifndef STATEMACH_HPP
#define STATEMACH_HPP

#include <string>
using namespace std;

class StateMach
{
	private:

	public:
		//self introduction
		virtual int TotalStates()=0; //total number of states
		virtual const char * Name()=0; //name

		virtual bool WarmedUp() const =0;
		virtual int State()=0; //current state
		virtual string StateDesc(int)=0; //describe what the state means

		virtual void Add(double,double,double,double) {}
		virtual void Add(double,double,double) {}
};

#endif
