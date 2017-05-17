#ifndef ACCT_HPP
#define ACCT_HPP

#include <map>
#include "../smath/BlackScholes.hpp"
#include "HKFE.hpp"
#include "IParam.hpp"
#include "STool.hpp"
#include "SException.hpp"
#include "ValidityChk.hpp"

using namespace std;

class Acct
{
	private:
		map<const string,int> _mPos;
		double _dCPnL;

	public:
		Acct();
		double CPnL() const;
		void Reset();
		void ResetCPnL() { _dCPnL = 0; }

		void Trade(const string &,int,double,double);
		void Trade(const string &,int,double);
		int GetPos(const string &) const;

};


Acct::Acct()
{
	Reset();
}

void Acct::Reset()
{
	ResetCPnL();
	_mPos.clear();
}

void Acct::Trade(const string &sSym, int iQty, double dPx, double dFrictPerQty)
{
	_dCPnL -= abs(dFrictPerQty) * abs(iQty);
	Trade(sSym, iQty, dPx);
}

void Acct::Trade(const string &sSym, int iQty, double dPx)
{
	if (iQty == 0) return;

	if (!STool::ContainsKey(_mPos, sSym)) _mPos[sSym] = 0;
	_mPos[sSym] += iQty;

	_dCPnL -= (double)iQty * dPx;

	if (_mPos[sSym] == 0) _mPos.erase(sSym);
}

int Acct::GetPos(const string &sSym) const
{
	if (!STool::ContainsKey(_mPos, sSym)) return 0;
	//else return _mPos[sSym]; // Can't use [] to make 'const' work
	else return _mPos.at(sSym);
}

double Acct::CPnL() const
{
	if (!_mPos.empty()) return IParam::MAGIC_NUM;
	else return _dCPnL;
}


#endif
