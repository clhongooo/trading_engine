#ifndef HKFE_HPP
#define HKFE_HPP

#include <map>
#include <fstream>
#include <string>
#include <cstdlib>
#include "IParam.hpp"
#include "STool.hpp"
#include "SDateTime.hpp"
#include "SLowerBound.hpp"
#include "Tokenizer.hpp"
#include "SException.hpp"
#include "ValidityChk.hpp"

using namespace std;

class HKFE
{
	private:
		static vector<YYYYMMDD> _vTrdgD;
		static vector<YYYYMMDD> _vExpyDIdx; //Idx of TrdgD at each ExpyD
		static map<const int,int> _mCtrt_TrdgD; //mapping ctrt and TrdgD Idx
		static const double _TRDG_DAY_PER_YR;

		static void LoadCalendar();
		static int LocateTrdgD(int);
		static double GetTTM_hidden(YYYYMMDD ymd, int iCtrtYM);
		static int VerbatimYMi(const string &sSym);

	public:
		static string GetSymbol(char cInstr, int iY, int iM);
		static string GetSymbol(char cInstr, int iK, int iY, int iM);
		static bool IsCall(const string &sSym);
		static bool IsPut(const string &sSym);
		static bool IsFut(const string &sSym);
		static double GetStrike(const string &sSym);
		static int GetExpyYMDi(const string &sSym, int iFM);
		static int GetExpyYMDi(int iA, int iFM);
		static string FutCode(int iY, int iM);
		static int WhichFrontMth(int iCurYMD, int iCtrtYM);
		static double GetDTM(YYYYMMDD ymd, int iFM);
		static double GetTTM(YYYYMMDD ymd, int iFM_or_CtrtYMD);
		static double GetTTM(YYYYMMDD ymd, const string &sSym);
		static int NextTrdgDate(YYYYMMDD ymd, int iNumDay);
		static int NextTrdgMin(YYYYMMDD ymd, int iYM);
		static bool IsRealTrdgHour(YYYYMMDD ymd, int iHM);
		static bool IsTrdgHour(YYYYMMDD ymd, int iHM);
		static int GetMktCloseHMi();
		static int GetMktCloseHMSi();
};



vector<YYYYMMDD> HKFE::_vTrdgD;
vector<YYYYMMDD> HKFE::_vExpyDIdx;
map<const int,int> HKFE::_mCtrt_TrdgD;

const double HKFE::_TRDG_DAY_PER_YR = 248;





void HKFE::LoadCalendar()
{
	deque<string> dqRawFile = STool::ReadFile(IParam::HKFE_CALENDAR);
	if (dqRawFile.empty()) return;
	for (int i = 0; i < dqRawFile.size(); ++i)
	{
		string sTD, sE="";

		Tokenizer tz(dqRawFile[i],",");
		if (tz.NextToken()) sTD = tz.GetToken();
		if (tz.NextToken()) sE = tz.GetToken();

		_vTrdgD.push_back(STool::YMDi(sTD));
		if (sE.at(0)=='E')
		{
			_vExpyDIdx.push_back(_vTrdgD.size()-1);
			_mCtrt_TrdgD[STool::YMi(sTD)] = _vTrdgD.size()-1;
		}
	}
}
int HKFE::LocateTrdgD(YYYYMMDD ymd)
{
	vector<YYYYMMDD>::iterator it;
	if (!SLowerBound<vector<YYYYMMDD>::iterator,int>::JustLarger(_vTrdgD.begin(),_vTrdgD.end(),ymd,it))
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	int iPsn = distance(_vTrdgD.begin(),it);
	return iPsn;
}


//Shorthand for Futures
string HKFE::GetSymbol(char cInstr, int iY, int iM)
{
	return GetSymbol(cInstr,100,iY,iM);
}
//return format:
//Futures: F-2011-08
//Options: C-21400-2012-04
//Options: P-19800-2012-11
string HKFE::GetSymbol(char cInstr, int iK, int iY, int iM)
{
	if (!ValidityChk::Year(iY) ||
			!ValidityChk::Month(iM) ||
			!ValidityChk::HKFEDeriType(cInstr) ||
			!ValidityChk::HKFEOptStrike(iK))
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}

	if (cInstr == 'F')
	{
		string s[3];
		s[0] = STool::CharToStr('F');
		s[1] = STool::ToStr(iY);
		s[2] = STool::PadLeft(STool::ToStr(iM),'0',2);
		return STool::ConstructStr(s,3,"-");
	}
	else
	{
		string s[4];
		s[0] = STool::CharToStr(cInstr);
		s[1] = STool::PadLeft(STool::ToStr(iK),'0',5);
		s[2] = STool::ToStr(iY);
		s[3] = STool::PadLeft(STool::ToStr(iM),'0',2);
		return STool::ConstructStr(s,4,"-");
	}
}

bool HKFE::IsCall(const string &sSym)
{
	if (sSym.length() < 9)
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	if (sSym.at(0) == 'C') return true; else return false;
}

bool HKFE::IsPut(const string &sSym)
{
	if (sSym.length() < 9)
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	if (sSym.at(0) == 'P') return true; else return false;
}

bool HKFE::IsFut(const string &sSym)
{
	if (sSym.length() < 9)
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	if (sSym.at(0) == 'F') return true; else return false;
}


double HKFE::GetStrike(const string &sSym)
{
	if (!IsCall(sSym) && !IsPut(sSym))
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}

	string sK;
	Tokenizer tz(sSym);
	if (tz.NextToken()) tz.GetToken();
	if (tz.NextToken())
	{
		sK = tz.GetToken();
		return STool::ToDbl(sK);
	}
	else
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
}


int HKFE::VerbatimYMi(const string &sSym)
{
	if (!IsFut(sSym) && !IsCall(sSym) && !IsPut(sSym))
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	else
	{
		string sY = "";
		string sM = "";
		Tokenizer tz(sSym);
		if (tz.NextToken()) tz.GetToken();
		if (!IsFut(sSym)) if (tz.NextToken()) tz.GetToken();
		if (tz.NextToken())
		{
			sY = tz.GetToken();
		}
		if (tz.NextToken())
		{
			sM = tz.GetToken();
		}
		if (sY == "" || sM == "")
		{
			SException se;
			se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
			throw se;
		}
		else return STool::YMi(STool::ToInt(sY),STool::ToInt(sM));
	}
}
int HKFE::GetExpyYMDi(const string &sSym, int iFM)
{
	int iCtrtYM = VerbatimYMi(sSym);

	if (!STool::ContainsKey(_mCtrt_TrdgD,iCtrtYM))
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}

	return _vTrdgD[_mCtrt_TrdgD[iCtrtYM]+iFM-1];

}

int HKFE::GetExpyYMDi(YYYYMMDD ymd, int iFM)
{
	if (_vTrdgD.empty() || _vExpyDIdx.empty()) LoadCalendar();

	int iPsn = LocateTrdgD(ymd);

	vector<YYYYMMDD>::iterator it;
	if (!SLowerBound<vector<YYYYMMDD>::iterator,YYYYMMDD>::JustLarger(_vExpyDIdx.begin(),_vExpyDIdx.end(),iPsn,it))
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}

	return _vTrdgD[*(it+iFM-1)];
}



//return: e.g. F12, G10
string HKFE::FutCode(int iY, int iM)
{
	string sR = "";

	switch (iM)
	{
		case 1: sR += "F"; break;
		case 2: sR += "G"; break;
		case 3: sR += "H"; break;
		case 4: sR += "J"; break;
		case 5: sR += "K"; break;
		case 6: sR += "M"; break;
		case 7: sR += "N"; break;
		case 8: sR += "Q"; break;
		case 9: sR += "U"; break;
		case 10: sR += "V"; break;
		case 11: sR += "X"; break;
		case 12: sR += "Z"; break;
	}
	string sY = "";
	if ((iY % 100) == 0) sY = "00";
	else if ((iY % 100) < 10) sY = "0" + STool::ToStr(iY % 100);
	else sY = STool::ToStr(iY % 100);

	sR += sY;
	return sR;
}

//========================================================
//Time to Maturity - Begin
//========================================================
int HKFE::WhichFrontMth(int iCurYMD, int iCtrtYM)
{
	if (_vTrdgD.empty() || _vExpyDIdx.empty()) LoadCalendar();

	if (!STool::ContainsKey(_mCtrt_TrdgD,iCtrtYM))
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}

	int iPsnCtrt = _mCtrt_TrdgD[iCtrtYM];
	int iPsnCur = LocateTrdgD(iCurYMD);
	int iTgtDTM = iPsnCtrt-iPsnCur;

	double dDTM;
	int iTgtFM = 1;
	int iTgtDiff = 0;

	do
	{
		iTgtDiff = GetDTM(iCurYMD,iTgtFM) - iTgtDTM - 1;
		if (iTgtDiff > 0) iTgtFM -= 1;
		else if (iTgtDiff < 0) iTgtFM += 1;
	}
	while (iTgtDiff != 0);
	return iTgtFM;
}

//iFM = Front Month. e.g. 1 = 1st First Mth, 2 = 2nd Front Mth
double HKFE::GetDTM(YYYYMMDD ymd, int iFM)
{
	if (_vTrdgD.empty() || _vExpyDIdx.empty()) LoadCalendar();

	//Error case
	if (_vTrdgD.size() < 10)
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}

	//Locate the specified date
	vector<YYYYMMDD>::iterator it;
	int iPsn = LocateTrdgD(ymd);

	if (!SLowerBound<vector<YYYYMMDD>::iterator,int>::JustLarger(_vExpyDIdx.begin(),_vExpyDIdx.end(),iPsn,it))
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	else
	{
		return *(it+iFM-1) - iPsn + 1;
	}

}

double HKFE::GetTTM(YYYYMMDD ymd, int iFM_or_CtrtYMD)
{
	if (iFM_or_CtrtYMD < 99) //assume iFM
		return GetDTM(ymd,iFM_or_CtrtYMD) / _TRDG_DAY_PER_YR;

	if (iFM_or_CtrtYMD > 999999) //can't process YMD
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}

	//assume YM
	return GetTTM_hidden(ymd, iFM_or_CtrtYMD);
}

double HKFE::GetTTM(YYYYMMDD ymd, const string &sSym)
{
	map<const int,int>::iterator it;
	int iSymYM = VerbatimYMi(sSym);
	return GetTTM_hidden(ymd, iSymYM);
}

double HKFE::GetTTM_hidden(YYYYMMDD ymd, int iCtrtYM)
{
	if (_vTrdgD.empty() || _vExpyDIdx.empty()) LoadCalendar();

	if (_mCtrt_TrdgD.find(iCtrtYM) == _mCtrt_TrdgD.end())
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	else
	{
		int iPsnE = _mCtrt_TrdgD[iCtrtYM];
		int iPsn = LocateTrdgD(ymd);
		if (iPsnE < iPsn)
		{
			SException se;
			se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
			throw se;
		}
		return (double)(iPsnE-iPsn+1) / _TRDG_DAY_PER_YR;
	}
}

//========================================================
//Time to Maturity - End
//========================================================



//returns -1 if no more
int HKFE::NextTrdgDate(YYYYMMDD ymd, int iNumDay)
{
	// if (ymd < 0) return -1;
	if (_vTrdgD.empty() || _vExpyDIdx.empty()) LoadCalendar();

	int iPsn = LocateTrdgD(ymd);

	if (iPsn+iNumDay >= _vTrdgD.size()-1) return -1;
	else return _vTrdgD[iPsn+iNumDay];
}



//returns -1 if that's the last trading minute for the trading day
int HKFE::NextTrdgMin(YYYYMMDD ymd, int iHM)
{

	int iRtn = STool::NextHMi(iHM,1);
	if (IsRealTrdgHour(ymd,iRtn))
	{
		return iRtn;
	}
	else
	{
		//Try advancing 30 min
		do
		{
			iRtn = STool::NextHMi(iRtn,30);
		}
		while (!IsRealTrdgHour(ymd,iRtn) && iRtn > iHM);

		if (iRtn < iHM) return -1; //We went past 2359 already

		//Travel back
		while (IsRealTrdgHour(ymd,iRtn))
		{
			iRtn = STool::NextHMi(iRtn,-1);
		}
		iRtn = STool::NextHMi(iRtn,1);

		return iRtn;
	}

}



bool HKFE::IsRealTrdgHour(YYYYMMDD ymd, int iHM)
{
	if (iHM > 9999) iHM /= 100;

	//????????-20110304: 945-1230, 1430-1615
	//20110307-20120302: 915-1200, 1330-1615
	//20120305-20130405: 915-1200, 1300-1615
	//20130408-????????: 915-1200, 1300-1615, 1700-2300
	YYYYMMDD ymd20130408(20130408);
	YYYYMMDD ymd20120305(20120305);
	YYYYMMDD ymd20110307(20110307);
	YYYYMMDD ymd20000000(20000000);
	if      (ymd >= ymd20130408) { if ((iHM >= 915 && iHM <= 1200) || (iHM >= 1300 && iHM <= 1615)) return true; else return false; }
	else if (ymd >= ymd20120305) { if ((iHM >= 915 && iHM <= 1200) || (iHM >= 1300 && iHM <= 1615)) return true; else return false; }
	else if (ymd >= ymd20110307) { if ((iHM >= 915 && iHM <= 1200) || (iHM >= 1330 && iHM <= 1615)) return true; else return false; }
	else if (ymd >= ymd20000000) { if ((iHM >= 945 && iHM <= 1230) || (iHM >= 1430 && iHM <= 1615)) return true; else return false; }
	else return false;

}



bool HKFE::IsTrdgHour(YYYYMMDD ymd, int iHM)
{
	if (iHM > 9999) iHM /= 100;

	//10 min after open, 5 min before close
	bool b10m = false;
	bool b5m = false;

	b5m = IsRealTrdgHour(ymd,STool::NextHMi(iHM,5));
	b10m = IsRealTrdgHour(ymd,STool::NextHMi(iHM,-10));

	return b5m && b10m;

}

int HKFE::GetMktCloseHMi() { return 1615; }
int HKFE::GetMktCloseHMSi() { return 161500; }



#endif
