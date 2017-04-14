#ifndef MODELSYS_HPP
#define MODELSYS_HPP

#include "../Util/BarProvider.hpp"

using namespace std;

class ModelSys
{
	protected:
		int _iPos;

	public:
		virtual const char * Name() const =0;

		void Reset() { _iPos = 0; ResetChild(); }
		virtual void ResetChild()=0; // must be overridden
		virtual void Add(const BarProvider &)=0;
		int Pos() const { return _iPos; };
		virtual void PrintExtraInfo() const =0;
};

#endif

