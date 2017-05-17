#ifndef RULESYS_H
#define RULESYS_H

#include "../Util/BarProvider.h"

using namespace std;

class RuleSys
{
	protected:
		int _iPos;

    //--------------------------------------------------
    // must be overridden
    //--------------------------------------------------
		virtual void ResetChild()=0;
		virtual void DailyInit() = 0;

	public:
		virtual const char * Name()=0;

		void Reset() { _iPos = 0; ResetChild(); }
		virtual void Add(const BarProvider &)=0;
		int Pos() { return _iPos; };
};

#endif
