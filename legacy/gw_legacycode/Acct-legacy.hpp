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
		double _dCPnL;
		double _dAggFutPx;
		double _dAggOptPx;
		int _iFutPos;
		map<const string,int> _mOptPos; //Key=HKFE::Symbol
		map<const string,int> _mOptPosPrev;
		map<const string,map<const string,int> > _mIdenOptPos; //Key=e.g. FMD, Value=another map like _mOptPos

	public:
		Acct();
		void FastClone(Acct);
		double CPnL() { return _dCPnL; }
		void ResetCPnL() { _dCPnL = 0; }
		int FutPos() { return _iFutPos; }
		const map<const string,int>& GetOptPos() const { return _mOptPos; }
		const map<const string,map<const string,int> >& GetIdenOptPos() const { return _mIdenOptPos; }
		int AbsCallPos();
		int AbsPutPos();
		int AggCallPos();
		int AggPutPos();
		double AggFutPx() { return _dAggFutPx; }

		void TradeFut(bool,int,double);
		void TradeOpt(bool,string,int,int,double,double,string);
		void MarkToMarket(double,int,double,double);


};


Acct::Acct()
{
	_dCPnL = 0;
	_dAggFutPx = 0;
	_dAggOptPx = 0;
	_iFutPos = 0;
}

//only cloning the file paths and the positions, not even cloning the pnl
void Acct::FastClone(Acct pcA)
{
	//_mOptPos = pcA.GetOptPos();
	//_mIdenOptPos = pcA.GetIdenOptPos();
	//_iFutPos = pcA.FutPos();
}





int Acct::AbsCallPos()
{
	int iCall = 0;

	for (map<const string,int>::iterator it=_mOptPos.begin(); it!=_mOptPos.end(); ++it)
		if (HKFE::IsCall((string)it->first))
			iCall += STool::AbsInt((int)it->second);

	return iCall;
}

int Acct::AbsPutPos()
{
	int iPut = 0;

	for (map<const string,int>::iterator it=_mOptPos.begin(); it!=_mOptPos.end(); ++it)
		if (HKFE::IsPut((string)it->first))
			iPut += STool::AbsInt((int)it->second);

	return iPut;
}

int Acct::AggCallPos()
{
	int iCall = 0;

	for (map<const string,int>::iterator it=_mOptPos.begin(); it!=_mOptPos.end(); ++it)
		if (HKFE::IsCall((string)it->first))
			iCall += (int)it->second;

	return iCall;
}

int Acct::AggPutPos()
{
	int iPut = 0;

	for (map<const string,int>::iterator it=_mOptPos.begin(); it!=_mOptPos.end(); ++it)
		if (HKFE::IsPut((string)it->first))
			iPut += (int)it->second;

	return iPut;
}

void Acct::TradeFut(bool bIsLong, int iAbsPos, double dPrice)
{
	if (iAbsPos == 0) return;
	iAbsPos = STool::AbsInt(iAbsPos);

	_iFutPos += (bIsLong ? 1 : -1) * abs(iAbsPos);
	_dAggFutPx += (bIsLong ? 1 : -1) * abs(iAbsPos) * dPrice;
	_dAggFutPx += abs(iAbsPos * (IParam::TRDG_FRICT_FUT_SLIP+IParam::TRDG_FRICT_CMSN));
}

void Acct::TradeOpt(bool bIsLong, string sSym, int iCT_YMD, int iAbsPos, double dUndPrice, double dVol, string sIden)
{
	if (iAbsPos == 0) return;
	iAbsPos = STool::AbsInt(iAbsPos);

	double dStrike = HKFE::GetStrike(sSym);
	double dTTM = HKFE::GetTTM(iCT_YMD, sSym);

	double dOptPx = 0;
	bool bIsCall = HKFE::IsCall(sSym);
	if (bIsCall)
		dOptPx = BlackScholes::EuropeanCallPrice(dUndPrice, dStrike, IParam::R_F, IParam::DIV_YIELD, dTTM, dVol);
	else
		dOptPx = BlackScholes::EuropeanPutPrice(dUndPrice, dStrike, IParam::R_F, IParam::DIV_YIELD, dTTM, dVol);

	//master map
	if (STool::ContainsKey(_mOptPos,sSym))
	{
		_mOptPos[sSym] = (int)_mOptPos[sSym] + (bIsLong ? 1 : -1) * abs(iAbsPos);

		//clean up
		if ((int)(_mOptPos[sSym]) == 0) _mOptPos.erase(sSym);
	}
	else
	{
		_mOptPos[sSym] = (bIsLong ? 1 : -1) * abs(iAbsPos);
	}

	_dAggOptPx += (bIsLong ? 1 : -1) * abs(iAbsPos) * dOptPx;

	//friction
	if (!ValidityChk::Price(dOptPx))
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}

	_dAggOptPx += abs(iAbsPos) *
		(ceil(IParam::TRDG_FRICT_OPT_PCTG*dOptPx)+IParam::TRDG_FRICT_CMSN);

	//sub map
	if (STool::ContainsKey(_mIdenOptPos,sIden))
	{
		if ((STool::ContainsKey(_mIdenOptPos[sIden],sSym)))
		{
			(_mIdenOptPos[sIden])[sSym] +=
				(int)(_mIdenOptPos[sIden])[sSym] + (bIsLong ? 1 : -1) * abs(iAbsPos);

			//clean up
			if ((int)(_mIdenOptPos[sIden][sSym]) == 0)
			{
				(_mIdenOptPos[sIden]).erase(sSym);
			}
		}
		else
		{
			(_mIdenOptPos[sIden])[sSym] = (bIsLong ? 1 : -1) * abs(iAbsPos);
		}
	}
	else
	{
		_mIdenOptPos[sIden] = map<const string,int>();
		(_mIdenOptPos[sIden])[sSym] = (bIsLong ? 1 : -1) * abs(iAbsPos);
	}
}


//1. Update our CPnL
//2. Perform clean up of expired options
void Acct::MarkToMarket(double dUndPrice, int iCT_YMD, double dVol, double dCloseShortOptBelowPx)
{
	//futures
	_dAggFutPx -= dUndPrice * _iFutPos;
	_dCPnL -= _dAggFutPx;
	_dAggFutPx = 0;
	_dAggFutPx = dUndPrice * _iFutPos;

	//options
	double _dAggOptPx_New = 0;

	//
	deque<string> dqRmSym1;

	for (map<const string,map<const string,int> >::iterator it1=_mIdenOptPos.begin(); it1!=_mIdenOptPos.end(); ++it1)
	{
		deque<string> dqRmSym2;

		for (map<string,int>::iterator it2=(it1->second).begin(); it2!=(it1->second).end(); ++it2)
		{
			double dOptPx = 0;
			string sSym = it2->first;
			double dTTM = HKFE::GetTTM(iCT_YMD, sSym);
			double dStrike = HKFE::GetStrike(sSym);

			if (HKFE::IsCall(sSym))
			{
				dOptPx = BlackScholes::EuropeanCallPrice(dUndPrice, dStrike, IParam::R_F, IParam::DIV_YIELD, dTTM, dVol);
			}
			else if (HKFE::IsPut(sSym))
			{
				dOptPx = BlackScholes::EuropeanPutPrice(dUndPrice, dStrike, IParam::R_F, IParam::DIV_YIELD, dTTM, dVol);
			}
			else
			{
				SException se;
				se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
				throw se;
			}

			//close that position virtually first
			_dAggOptPx -= dOptPx * (int)(it2->second);

			//clean-up if TTM < 1 minute, and
			if (dTTM < (double)1 / (365 * 24 * 60))
			{
				dqRmSym1.push_back(sSym);
				dqRmSym2.push_back(sSym);
				if (STool::ContainsKey(_mOptPos,sSym)) _mOptPos.erase(sSym);
			}
			//close those shorted options that are below this price
			else if (dOptPx < dCloseShortOptBelowPx && (int)(it2->second) < 0)
			{
				if (STool::ContainsKey(_mOptPos,sSym))
				{
					_mOptPos[sSym] += (int)_mOptPos[sSym] - (int)(it2->second);
				}
				else
				{
					_mOptPos[sSym] = -(int)(it2->second);
				}
				dqRmSym1.push_back(sSym);
				dqRmSym2.push_back(sSym);
			}
			//otherwise continue holding that position
			else _dAggOptPx_New += dOptPx * (int)(it2->second);
		}

		for (int i = 0; i < dqRmSym2.size(); i++) (it1->second).erase(dqRmSym2[i]);

	}

	for (int i = 0; i < dqRmSym1.size(); i++) _mOptPos.erase(dqRmSym1[i]);


	_dCPnL -= _dAggOptPx;
	_dAggOptPx = _dAggOptPx_New;


}








/*


	 public int OptionContractNum
	 {
	 get
	 {
	 int iNum = 0;
	 foreach (DictionaryEntry de in _mOptPos)
	 {
	 if ((int)abs((int)(de.Value)) != 0) iNum++;
	 }
	 return iNum;
	 }
//strangely found to slower the optimization down...
//get
//{
//    return _mOptPos.Count;
//}
}

public void PenalizeCPnLPctgFrict(double dUndPrice1, double dUndPrice2, int iCT_YMD, int iRnd)
{
object[] keys = new object[_mOptPos.Count];
_mOptPos.Keys.CopyTo(keys, 0);

foreach (object key in keys)
{
double dOptPx = 0;
double dTTM = hdh.GetCalendarTTMHK(iCT_YMD, SymbolCodeHelper.GetFutCodeFromOptCode((string)key));

double dUndPrice = dUndPrice1;
int iFrontMth = 1;
string s1FrontMthFutCode = hdh.GetFutCodeHK_WthMinDTM(iCT_YMD, 5 / 1440d);
if (SymbolCodeHelper.GetFutCodeFromOptCode((string)key).CompareTo(s1FrontMthFutCode) == 0)
{
iFrontMth = 1;
dUndPrice = dUndPrice1;
}
else
{
iFrontMth = 2;
dUndPrice = dUndPrice2;
}


//first calculate option price for that contract
if (SymbolCodeHelper.IsCallFromOptCode((string)key))
{
dOptPx = BlackScholesAlt.GetCallValue(
dUndPrice,
SymbolCodeHelper.GetStrikeFromOptCode((string)key),
IParam.rf,
dTTM,
vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)key), iCT_YMD, iFrontMth));
}
else if (SymbolCodeHelper.IsPutFromOptCode((string)key))
{
dOptPx = BlackScholesAlt.GetPutValue(
dUndPrice,
SymbolCodeHelper.GetStrikeFromOptCode((string)key),
IParam.rf,
dTTM,
vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)key), iCT_YMD, iFrontMth));
}

_dCPnL -=  abs((int)_mOptPos[key]) *
(Math.Ceiling(IParam.FRICTIONPCTG * dOptPx) + IParam.FRICTIONTRXCOST) *
iRnd; //1 round or 2 rounds

_dCPnL -= abs((int)_iFutPos) * IParam.FRICTION * iRnd;
}
}

public void LoadPortfolioFrmFile(string sFilePath)
{
	if (!IParam.DataDepotLoadPort.htFileNameContent.ContainsKey(sFilePath))
	{
		using (StreamReader sr = new StreamReader(sFilePath))
		{
			try
			{
				string sLine;
				List<string> lContent = new List<string>();

				while ((sLine = sr.ReadLine()) != null) lContent.Add(sLine);

				IParam.DataDepotLoadPort.htFileNameContent.Add(sFilePath, lContent);

				sr.Close;
			}
			catch (Exception e)
			{
				Console.WriteLine("Default portfolio file could not be read:");
				Console.WriteLine(e.Message);
				Environment.Exit(0);
			}
		}
	}


	List<string> lStrings = (List<string>)IParam.DataDepotLoadPort.htFileNameContent[sFilePath];


	for (int i = 0; i < lStrings.Count; i++)
	{
		string sLine = lStrings[i];
		string[] sParts = sLine.Split(',');

		if (SymbolCodeHelper.IsCallFromOptCode(sParts[0]) ||
				SymbolCodeHelper.IsPutFromOptCode(sParts[0]))
		{
			TradeOpt(Double.Parse(sParts[1]) < 0 ? false : true,
					SymbolCodeHelper.IsCallFromOptCode(sParts[0]),
					SymbolCodeHelper.GetStrikeFromOptCode(sParts[0]),
					IParam.UGLYHARDCODEDATE,
					SymbolCodeHelper.GetFutCodeFromOptCode(sParts[0]),
					(int)abs(Double.Parse(sParts[1])),
					IParam.UGLYHARDCODEUNDPX, IParam.UGLYHARDCODEUNDPX,
					false, IParam.NOFRICTION, "ASS");
		}
		else if (sParts[0].CompareTo("FUT") == 0)
		{
			TradeFut(Double.Parse(sParts[1]) < 0 ? false : true,
					(int)abs(Double.Parse(sParts[1])),
					IParam.UGLYHARDCODEUNDPX,
					IParam.FRICTION);
		}
	}

}

//any price dPx will do, because after knocking out the randome ctrt, we'll reset pnl
public void KnockOut1RandCtrt(RandomEngine re, double dPx, int iCT_YMD, string sIden)
{
	int iFP = (FutPos == 0 ? 0 : 1);
	int iOP = _mOptPos.Count; //deliberately not use OptionContractNum

	object[] keys = new object[_mOptPos.Count];
	_mOptPos.Keys.CopyTo(keys, 0);


	if (iFP + iOP <= 0) return;
	else
	{
		int iTheLuckyOne = re.Next(0, iFP + iOP);

		if (iTheLuckyOne == 0 && iFP == 1) //get rid of futures
		{
			TradeFut((FutPos > 0 ? false : true), abs(FutPos), dPx, IParam.FRICTION);
		}
		else //get rid of options
		{
			string sOptCode = (string)keys[iTheLuckyOne - iFP];

			TradeOpt((int)_mOptPos[keys[iTheLuckyOne - iFP]] > 0 ? false : true,
					SymbolCodeHelper.IsCallFromOptCode(sOptCode),
					SymbolCodeHelper.GetStrikeFromOptCode(sOptCode),
					iCT_YMD,
					SymbolCodeHelper.GetFutCodeFromOptCode(sOptCode),
					abs((int)_mOptPos[sOptCode]),
					dPx,dPx,
					false, IParam.NOFRICTION,
					sIden);
		}
	}
}

//any price for dPx will do since we'll reset pnl afterwards
public void SlashPosByHalf(double dPx, int iCT_YMD, string sIden)
{
	//futures
	if (FutPos != 0)
	{
		TradeFut(FutPos > 0 ? false : true,
				(int)Math.Floor(abs(FutPos) / (double)2),
				dPx, IParam.FRICTION);
	}

	//options
	object[] keys = new object[_mOptPos.Count];
	_mOptPos.Keys.CopyTo(keys, 0);

	foreach (object key in keys)
	{
		int lPos = (int)(_mOptPos[key]);

		bool bIsCall = SymbolCodeHelper.IsCallFromOptCode((string)key);
		bool bIsPut = SymbolCodeHelper.IsPutFromOptCode((string)key);

		bool bCallPut = true;
		if (bIsCall && !bIsPut) bCallPut = true;
		else if (!bIsCall && bIsPut) bCallPut = false;

		if (lPos != 0)
			TradeOpt(lPos > 0 ? false : true,
					bCallPut, SymbolCodeHelper.GetStrikeFromOptCode((string)key),
					iCT_YMD, SymbolCodeHelper.GetFutCodeFromOptCode((string)key),
					(int)Math.Floor(abs(lPos) / (double)2), dPx, dPx,
					false, IParam.NOFRICTION, sIden);
	}
}

//any price for dPx will do since we'll reset pnl afterwards
public void DoublePosSize(double dPx, int iCT_YMD, string sIden)
{
	//futures
	if (FutPos != 0)
	{
		TradeFut(FutPos > 0 ? true : false,
				(int)Math.Max(IParam.MAXNUMFUTCONTRACT-abs(FutPos),0),
				dPx, IParam.FRICTION);
	}

	//options
	object[] keys = new object[_mOptPos.Count];
	_mOptPos.Keys.CopyTo(keys, 0);

	foreach (object key in keys)
	{
		int lPos = (int)(_mOptPos[key]);

		bool bIsCall = SymbolCodeHelper.IsCallFromOptCode((string)key);
		bool bIsPut = SymbolCodeHelper.IsPutFromOptCode((string)key);

		bool bCallPut = true;
		if (bIsCall && !bIsPut) bCallPut = true;
		else if (!bIsCall && bIsPut) bCallPut = false;

		if (lPos != 0)
			TradeOpt(lPos > 0 ? true : false,
					bCallPut, SymbolCodeHelper.GetStrikeFromOptCode((string)key),
					iCT_YMD, SymbolCodeHelper.GetFutCodeFromOptCode((string)key),
					(int)abs(lPos), dPx, dPx,
					false, IParam.NOFRICTION, sIden);
	}
}

//any price for dPx will do since we just want to get the position size
public void MultiplyFutPsnSz(double dPx, int iCT_YMD, double dMul, string sIden)
{
	//futures
	if (FutPos != 0)
	{
		bool bLong = (FutPos > 0);
		int lAPos = (int)abs(FutPos);

		//first unwind
		TradeFut(!bLong,
				lAPos,
				dPx, IParam.FRICTION);
		//then rebuild
		TradeFut(bLong,
				(int)Math.Round((double)lAPos * dMul,0),
				dPx, IParam.FRICTION);
	}
}

//any price for dPx will do since we just want to get the position size
public void MultiplyOptPsnSz(double dPx, int iCT_YMD, double dMul, string sIden)
{
	//options
	object[] keys = new object[_mOptPos.Count];
	_mOptPos.Keys.CopyTo(keys, 0);

	foreach (object key in keys)
	{
		int lPos = (int)(_mOptPos[key]);
		int lAPos = (int)abs(lPos);
		bool bLong = (lPos > 0);

		bool bIsCall = SymbolCodeHelper.IsCallFromOptCode((string)key);
		bool bIsPut = SymbolCodeHelper.IsPutFromOptCode((string)key);

		bool bCallPut = true;
		if (bIsCall && !bIsPut) bCallPut = true;
		else if (!bIsCall && bIsPut) bCallPut = false;

		if (lPos != 0)
		{
			//first unwind
			TradeOpt(!bLong,
					bCallPut, SymbolCodeHelper.GetStrikeFromOptCode((string)key),
					iCT_YMD, SymbolCodeHelper.GetFutCodeFromOptCode((string)key),
					lAPos,
					dPx, dPx,
					false, IParam.NOFRICTION, sIden);
			//then rebuild
			TradeOpt(bLong,
					bCallPut, SymbolCodeHelper.GetStrikeFromOptCode((string)key),
					iCT_YMD, SymbolCodeHelper.GetFutCodeFromOptCode((string)key),
					(int)Math.Round((double)lAPos * dMul, 0),
					dPx, dPx,
					false, IParam.NOFRICTION, sIden);
		}
	}
}



public string[] GetHTOptionCode()
{
	string[] saRtn = new string[_mOptPos.Count]; ;

	_mOptPos.Keys.CopyTo(saRtn, 0);

	return saRtn;
}

public int CompareOptCtrtDiff(PositionController pcA)
{
	int iDiff = 0;

	if (IParam.RUNMODE == 1 || IParam.RUNMODE == 3) return iDiff;

	map htA = pcA.GetOptPos();

	//starting from this HT
	foreach (DictionaryEntry de in _mOptPos)
	{
		if (!htA.ContainsKey(de.Key)) //uniq in this HT
		{
			iDiff += (int)abs((int)de.Value);
		}
		else //common entries
		{
			iDiff += (int)abs((int)de.Value - (int)(htA[de.Key]));
		}
	}

	//then the other way round from htA
	foreach (DictionaryEntry de in htA)
	{
		if (!_mOptPos.ContainsKey(de.Key)) //uniq in htA
		{
			iDiff += (int)abs((int)de.Value);
		}
	}

	return iDiff;
}



public bool HasReachMaxTrdgCap
{
	get
	{
		if ((OptionContractNum + (FutPos == 0 ? 0 : 1)) >= IParam.MAXNUMCONTRACTCODE) return true;
		else return false;
	}
}

public double GetPortOptDelta(int iCT_YMD, double dUndPrice1, double dUndPrice2)
{
	double dPortOptDelta = 0;

	foreach (DictionaryEntry de in _mOptPos)
	{
		double dOptDelta = 0;
		double dTTM = hdh.GetCalendarTTMHK(iCT_YMD, SymbolCodeHelper.GetFutCodeFromOptCode((string)de.Key));


		double dUndPrice = dUndPrice1;
		int iFrontMth = 1;
		string s1FrontMthFutCode = hdh.GetFutCodeHK_WthMinDTM(iCT_YMD, 5 / 1440d);
		if (SymbolCodeHelper.GetFutCodeFromOptCode((string)de.Key).CompareTo(s1FrontMthFutCode) == 0)
		{
			iFrontMth = 1;
			dUndPrice = dUndPrice1;
		}
		else
		{
			iFrontMth = 2;
			dUndPrice = dUndPrice2;
		}



		if (SymbolCodeHelper.IsCallFromOptCode((string)de.Key))
		{
			dOptDelta = BlackScholesAlt.GetCallDelta(dUndPrice,
					SymbolCodeHelper.GetStrikeFromOptCode((string)de.Key),
					IParam.rf,
					dTTM,
					vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)de.Key), iCT_YMD, iFrontMth));
		}
		else if (SymbolCodeHelper.IsPutFromOptCode((string)de.Key))
		{
			dOptDelta = BlackScholesAlt.GetPutDelta(dUndPrice,
					SymbolCodeHelper.GetStrikeFromOptCode((string)de.Key),
					IParam.rf,
					dTTM,
					vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)de.Key), iCT_YMD, iFrontMth));
		}
		else continue;

		dPortOptDelta += (int)de.Value * dOptDelta;
	}
	return dPortOptDelta;
}
public double GetPortDelta(int iCT_YMD, double dUndPrice1, double dUndPrice2)
{
	double dPortDelta = 0;
	dPortDelta = GetPortOptDelta(iCT_YMD, dUndPrice1, dUndPrice2);
	dPortDelta += _iFutPos;
	return dPortDelta;
}
public double GetPortOptGamma(int iCT_YMD, double dUndPrice1, double dUndPrice2)
{
	double dPortOptGamma = 0;

	foreach (DictionaryEntry de in _mOptPos)
	{
		double dOptGamma = 0;
		double dTTM = hdh.GetCalendarTTMHK(iCT_YMD, SymbolCodeHelper.GetFutCodeFromOptCode((string)de.Key));

		double dUndPrice = dUndPrice1;
		int iFrontMth = 1;
		string s1FrontMthFutCode = hdh.GetFutCodeHK_WthMinDTM(iCT_YMD, 5 / 1440d);
		if (SymbolCodeHelper.GetFutCodeFromOptCode((string)de.Key).CompareTo(s1FrontMthFutCode) == 0)
		{
			iFrontMth = 1;
			dUndPrice = dUndPrice1;
		}
		else
		{
			iFrontMth = 2;
			dUndPrice = dUndPrice2;
		}


		dOptGamma = BlackScholesAlt.GetGamma(dUndPrice,
				SymbolCodeHelper.GetStrikeFromOptCode((string)de.Key),
				IParam.rf,
				dTTM,
				vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)de.Key), iCT_YMD, iFrontMth));

		dPortOptGamma += (int)de.Value * dOptGamma;
	}
	return dPortOptGamma;
}

public void CloseFutPos(double dPrice, double dFriction)
{
	TradeFut(_iFutPos > 0 ? false : true, abs(_iFutPos), dPrice, dFriction);
}
public void CloseOptPos(double dUndPrice1, double dUndPrice2, int iCT_YMD, bool bUsePctgFrict, double dFriction, string sIden)
{
	if (!_mIdenOptPos.ContainsKey(sIden)) return;

	object[] keys = new object[((map)_mIdenOptPos[sIden]).Count];
	((map)_mIdenOptPos[sIden]).Keys.CopyTo(keys, 0);

	foreach (object key in keys)
	{
		double dOptPx = 0;
		double dTTM = hdh.GetCalendarTTMHK(iCT_YMD, SymbolCodeHelper.GetFutCodeFromOptCode((string)key));

		double dUndPrice = dUndPrice1;
		int iFrontMth = 1;
		string s1FrontMthFutCode = hdh.GetFutCodeHK_WthMinDTM(iCT_YMD, 5 / 1440d);
		if (SymbolCodeHelper.GetFutCodeFromOptCode((string)key).CompareTo(s1FrontMthFutCode) == 0)
		{
			iFrontMth = 1;
			dUndPrice = dUndPrice1;
		}
		else
		{
			iFrontMth = 2;
			dUndPrice = dUndPrice2;
		}


		//first calculate option price for that contract
		if (SymbolCodeHelper.IsCallFromOptCode((string)key))
		{
			dOptPx = BlackScholesAlt.GetCallValue(
					dUndPrice,
					SymbolCodeHelper.GetStrikeFromOptCode((string)key),
					IParam.rf,
					dTTM,
					vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)key), iCT_YMD, iFrontMth));
		}
		else if (SymbolCodeHelper.IsPutFromOptCode((string)key))
		{
			dOptPx = BlackScholesAlt.GetPutValue(
					dUndPrice,
					SymbolCodeHelper.GetStrikeFromOptCode((string)key),
					IParam.rf,
					dTTM,
					vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)key), iCT_YMD, iFrontMth));
		}
		// remove those erroneous records
		else
		{
			((map)_mIdenOptPos[sIden]).Remove((string)key);
			continue;
		}

		//close position virtually
		_dAggOptPx -= dOptPx * (int)((map)_mIdenOptPos[sIden])[key];

		//friction
		if (bUsePctgFrict && !Double.IsNaN(dOptPx))
		{
			_dAggOptPx += abs((int)((map)_mIdenOptPos[sIden])[key]) *
				(Math.Ceiling(dOptPx * IParam.FRICTIONPCTG) + IParam.FRICTIONTRXCOST);
		}
		else
		{
			_dAggOptPx += abs(dFriction * (int)((map)_mIdenOptPos[sIden])[key]);
		}

		//clean-up!!!
		if (_mOptPos.ContainsKey(key))
		{
			_mOptPos[key] = (int)_mOptPos[key] - (int)((map)_mIdenOptPos[sIden])[key];
		}
		else
		{
			_mOptPos.Add(key, -(int)((map)_mIdenOptPos[sIden])[key]);
		}
		((map)_mIdenOptPos[sIden]).Remove((string)key);
	}

}

public void ResetOutputFile()
{
	using (StreamWriter fout1 = new StreamWriter(sCPnLOutputPath, false))
	{
		fout1.WriteLine();
		fout1.Close;
	}
	using (StreamWriter fout2 = new StreamWriter(sMROutputPath, false))
	{
		fout2.WriteLine();
		fout2.Close;
	}
	using (StreamWriter fout = new StreamWriter(sDetailedOutputPath, false))
	{
		fout.WriteLine();
		fout.Close;
	}
}

public void OutputDetails(string sTmp)
{
	using (StreamWriter fout = new StreamWriter(sDetailedOutputPath, true))
	{
		fout.WriteLine(sTmp);
		fout.Close;
	}
}


public void OutputPositionInfoOpen(int iCT_YMD, double dUndPrice)
{
	using (StreamWriter fout = new StreamWriter(sDetailedOutputPath, true))
	{
		fout.WriteLine("=============================================================");
		fout.WriteLine("| " + iCT_YMD.ToString() + " | Open |");
		fout.WriteLine("=============================================================");
		fout.WriteLine("Underlying: Open: " + dUndPrice.ToString());
		fout.WriteLine("CPnL (pts)= " + Math.Round(_dCPnL, 2).ToString());
		fout.WriteLine("Futures Position = " + FutPos.ToString());
		fout.Close;
	}
}

public void OutputPositionGenetic(double dUtil)
{
	using (StreamWriter fout = new StreamWriter(sDetailedOutputPath, true))
	{
		fout.WriteLine("-------------------------------------------------------------");
		fout.WriteLine("Util: " + dUtil.ToString());
		fout.WriteLine("FUT," + FutPos.ToString());

		object[] keys = new object[_mOptPos.Count];
		_mOptPos.Keys.CopyTo(keys, 0);

		Array.Sort(keys);
		Array.Reverse(keys);

		foreach (object key in keys)
		{
			if ((int)_mOptPos[key] != 0)
			{
				fout.Write(key.ToString());
				fout.WriteLine("," + ((int)_mOptPos[key]).ToString());
			}
		}

		fout.Close;
	}
}



//iMode:
//       2 detailed: basic option systems
//       3 assess.txt: position and option price only
//       4 position size only
public void OutputPositionInfoClose(int iCT_YMD, double dUndPrice1, double dUndPrice2, int iMode)
{
	if (iMode != 3 && iMode != 4)
		using (StreamWriter fout1 = new StreamWriter(sCPnLOutputPath, true))
		{
			fout1.Write(iCT_YMD.ToString());
			fout1.Write("\t");
			fout1.WriteLine(Math.Round(_dCPnL, 2).ToString());
			fout1.Close;
		}

	if (iMode != 3 && iMode != 4)
		using (StreamWriter fout2 = new StreamWriter(sMROutputPath, true))
		{
			fout2.Write(iCT_YMD.ToString());
			fout2.Write("\t");
			//fout2.WriteLine(Math.Round(CalcMarginReq(dUndPrice, dUndPrice, iCT_YMD)).ToString());
			fout2.Close;
		}

	using (StreamWriter fout = new StreamWriter(sDetailedOutputPath, true))
	{
		if (iMode != 3 && iMode != 4)
		{
			fout.WriteLine("=============================================================");
			fout.WriteLine("| " + iCT_YMD.ToString() + " | Close |");
			fout.WriteLine("=============================================================");
			fout.WriteLine("Underlying1: Close: " + dUndPrice1.ToString());
			fout.WriteLine("Underlying2: Close: " + dUndPrice2.ToString());
			fout.WriteLine("VHSI: " + vs.VHSILatest.ToString());
			fout.WriteLine("ATR= " + Math.Round(vs.ATR).ToString());
			//fout.WriteLine("AbsCallPos= " + AbsCallPos.ToString());
			//fout.WriteLine("AbsPutPos= " + AbsPutPos.ToString());
			//fout.WriteLine("AggCallPos= " + AggCallPos.ToString());
			//fout.WriteLine("AggPutPos= " + AggPutPos.ToString());
			//fout.WriteLine("AggFutPx= " + _dAggFutPx.ToString());
			//fout.WriteLine("AggOptPx= " + _dAggOptPx.ToString());
			fout.WriteLine("Delta= " + (GetPortDelta(iCT_YMD, dUndPrice1, dUndPrice2)).ToString());
			fout.WriteLine("Gamma= " + (GetPortOptGamma(iCT_YMD, dUndPrice1, dUndPrice2)).ToString());
			fout.WriteLine("CPnL (pts)= " + Math.Round(_dCPnL, 2).ToString());
			//fout.WriteLine("Margin req (Pt)= " + Math.Round(CalcMarginReq(dUndPrice, dUndPrice, iCT_YMD)).ToString());
			//fout.WriteLine("Margin req (Ptx10)= " + Math.Round(CalcMarginReq(dUndPrice, dUndPrice, iCT_YMD) * 10).ToString());
			fout.WriteLine("-------------------------------------------------------------");
		}

		if (iMode == 4)
		{
			fout.WriteLine("Mini Futures =\t\t[" + FutPos.ToString() + "]");
			fout.WriteLine("Regular Options:");
		}
		else
		{
			fout.WriteLine("Futures Posn =\t\t[" + FutPos.ToString() + "]");
		}

		if (iMode != 3  && iMode != 4)
			fout.WriteLine("Options Aggregate Position:");

		object[] keys = new object[_mOptPos.Count];
		_mOptPos.Keys.CopyTo(keys, 0);

		Array.Sort(keys);
		Array.Reverse(keys);

		foreach (object key in keys)
		{
			double dOptPx = 0;
			double dTTM = hdh.GetCalendarTTMHK(iCT_YMD, SymbolCodeHelper.GetFutCodeFromOptCode((string)key));

			double dUndPrice = dUndPrice1;
			int iFrontMth = 1;
			string s1FrontMthFutCode = hdh.GetFutCodeHK_WthMinDTM(iCT_YMD, 5 / 1440d);
			if (SymbolCodeHelper.GetFutCodeFromOptCode((string)key).CompareTo(s1FrontMthFutCode) == 0)
			{
				iFrontMth = 1;
				dUndPrice = dUndPrice1;
			}
			else
			{
				iFrontMth = 2;
				dUndPrice = dUndPrice2;
			}

			//first calculate option price for that contract
			if (SymbolCodeHelper.IsCallFromOptCode((string)key))
			{
				dOptPx = BlackScholesAlt.GetCallValue(
						dUndPrice,
						SymbolCodeHelper.GetStrikeFromOptCode((string)key),
						IParam.rf,
						dTTM,
						vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)key), iCT_YMD, iFrontMth));
			}
			else if (SymbolCodeHelper.IsPutFromOptCode((string)key))
			{
				dOptPx = BlackScholesAlt.GetPutValue(
						dUndPrice,
						SymbolCodeHelper.GetStrikeFromOptCode((string)key),
						IParam.rf,
						dTTM,
						vs.GetLocalVolatility(dUndPrice1, dUndPrice2, SymbolCodeHelper.GetStrikeFromOptCode((string)key), iCT_YMD, iFrontMth));
			}

			fout.Write("Opt: " + key.ToString());
			fout.Write("\t[" + ((int)_mOptPos[key]).ToString());
			fout.WriteLine("]\t" + Math.Round(dOptPx, 2).ToString());
		}


		if (iMode != 3 && iMode != 4)
		{
			fout.WriteLine("-------------------------------------------------------------");
			object[] keys2 = new object[_mIdenOptPos.Count];
			_mIdenOptPos.Keys.CopyTo(keys2, 0);

			Array.Sort(keys2);

			foreach (object key2 in keys2)
			{
				object[] keys3 = new object[((map)_mIdenOptPos[key2]).Count];
				((map)_mIdenOptPos[key2]).Keys.CopyTo(keys3, 0);

				Array.Sort(keys3);
				Array.Reverse(keys3);

				fout.WriteLine("Identifier: " + key2.ToString());

				foreach (object key3 in keys3)
				{
					fout.Write("Opt: " + key3.ToString());
					fout.WriteLine("\t[" + ((map)_mIdenOptPos[key2])[key3].ToString() + "]");
				}
			}

			fout.WriteLine("-------------------------------------------------------------");
			fout.WriteLine("Change in Options Aggregate Position:");

			//now compare the new position hashtable with the old hashtable
			if (_mOptPosPrev.Count > 0)
			{
				List<string> sKeysTotal = new List<string>();
				List<string> sKeysTotalUniq = new List<string>();

				foreach (string s in _mOptPos.Keys)
					if ((int)_mOptPos[s] != 0) sKeysTotal.Add((string)s);
				foreach (string s in _mOptPosPrev.Keys)
					if ((int)_mOptPosPrev[s] != 0) sKeysTotal.Add((string)s);

				sKeysTotal.Sort();

				string sPrev = "";
				foreach (string s in sKeysTotal)
				{
					if (s.CompareTo(sPrev) != 0) sKeysTotalUniq.Add(s);
					sPrev = s;
				}


				foreach (string s in sKeysTotalUniq)
				{
					bool bOld = _mOptPosPrev.ContainsKey(s);
					bool bNew = _mOptPos.ContainsKey(s);

					if (bOld && (int)_mOptPosPrev[s] == 0) bOld = false;
					if (bNew && (int)_mOptPos[s] == 0) bNew = false;

					//new
					if (!bOld && bNew)
					{
						fout.Write("Opt: " + s.ToString());
						fout.WriteLine("\t[" + _mOptPos[s].ToString() + "]");
					}
					//old closed
					else if (bOld && !bNew)
					{
						fout.Write("Opt: " + s.ToString());
						fout.WriteLine("\t[" + (-(int)_mOptPosPrev[s]).ToString() + "]");
					}
					//pos changed
					else if (bOld && bNew && ((int)_mOptPos[s] - (int)_mOptPosPrev[s] != 0))
					{
						fout.Write("Opt: " + s.ToString());
						fout.WriteLine("\t[" + ((int)_mOptPos[s] - (int)_mOptPosPrev[s]).ToString() + "]");
					}
				}

			}

			_mOptPosPrev = (map)_mOptPos.Clone();
		}

		fout.Close;
	}
}


*/




















#endif
