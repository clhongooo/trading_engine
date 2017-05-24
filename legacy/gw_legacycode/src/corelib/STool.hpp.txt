#ifndef STOOL_HPP
#define STOOL_HPP

#include <limits>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <deque>
#include <map>
#include "Tokenizer.hpp"
#include "ValidityChk.hpp"
#include "SException.hpp"

using namespace std;

class STool
{
	public:
		inline static double Max(double dA, double dB);
		inline static double Min(double dA, double dB);
		inline static int Max(int iA, int iB);
		inline static int Min(int iA, int iB);
		inline static int Sign(int i);
		inline static double Sign(double d);
		inline static double IntPart1(double d);
		inline static double IntPart2(double d);
		inline static double Round(double d, unsigned u);
		inline static int Abs(int iA);
		inline static double RoundIntvl(double d, double dintvl);
		inline static string DecToBin(int iDec);
		inline static int BinToDec(const string &sBin);
		inline static string ToStr(int i);
		inline static string CharToStr(char c);
		inline static string ToStr(double d, unsigned u);
		inline static double ToDbl(const string &s);
		inline static double ToDbl(const string &s, unsigned u);
		inline static int ToInt(const string &s);
		inline static string PadLeft(unsigned iOri, char cPad, unsigned iLen);
		inline static string PadLeft(const string &sOri, char cPad, unsigned iLen);
		inline static string PadRight(unsigned iOri, char cPad, unsigned iLen);
		inline static string PadRight(string sOri, char cPad, unsigned iLen);
		inline static string ConstructStr(const string *s, int iSize, const string &sDelim);
		inline static bool IsNaN(double value);
		template<typename T1,typename T2>
			inline static bool ContainsKey(map<T1,T2> m,T1 k);
		static deque<string> ReadFile(const char *sPath);

};




//returns the larger
inline double STool::Max(double dA, double dB)
{
	if (IsNaN(dA) || IsNaN(dB))
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	return (dA > dB ? dA : dB);
}
//returns the smaller
inline double STool::Min(double dA, double dB)
{
	if (IsNaN(dA) || IsNaN(dB))
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	return (dA < dB ? dA : dB);
}
//returns the larger
inline int STool::Max(int iA, int iB)
{
	return (iA > iB ? iA : iB);
}
//returns the smaller
inline int STool::Min(int iA, int iB)
{
	return (iA < iB ? iA : iB);
}
//returns 1 if positive and -1 if negative, 1 otherwise
inline int STool::Sign(int i)
{
	if (i == 0) return 1;
	else if (i < 0) return -1;
	else if (i > 0) return 1;
}
//returns 1 if positive and -1 if negative, 1 otherwise
inline double STool::Sign(double d)
{
	if (d == 0) return 1;
	else if (d < 0) return -1;
	else if (d > 0) return 1;
}
//returns an int closer to zero
inline double STool::IntPart1(double d)
{
	if (d < 0.0) return ceil(d); else return floor(d);
}
//returns an int farther from zero
inline double STool::IntPart2(double d)
{
	if (d < 0.0) return floor(d); else return ceil(d);
}

//no round() in cmath
inline double STool::Round(double d, unsigned u)
{
	bool bPve = true; if (d < 0) bPve = false; d = abs(d);
	double dFac = 1;
	if (u > 0) dFac = pow(10,u); else dFac = 1;
	return floor((dFac*d) + 0.5) / dFac * (bPve?1:-1);
}

//no abs() for int
inline int STool::Abs(int iA)
{
	if (iA >= 0) return iA;
	else return -iA;
}


//round the double to the closest interval
inline double STool::RoundIntvl(double d, double dintvl)
{
	return Round(d/dintvl,0)*dintvl;
}

inline string STool::DecToBin(int iDec)
{
	if (iDec == 0) return "0";
	if (iDec == 1) return "1";

	if (iDec%2 == 0) return DecToBin(iDec/2)+"0";
	else return DecToBin(iDec/2)+"1";
}
inline int STool::BinToDec(const string &sBin)
{
	int iDec = 0;
	int pow = 1;
	for (int i = sBin.length()-1; i >= 0; --i, pow <<= 1)
		iDec += (sBin[i] - '0') * pow;
	return iDec;
}


//std::to_string() is missing
//boost::lexical_cast<string> requires the boost library
inline string STool::ToStr(int i)
{
	stringstream ss;
	ss << i;
	return ss.str();
}
inline string STool::CharToStr(char c)
{
	string sR("");
	sR += c;
	return sR;
}
inline string STool::ToStr(double d, unsigned u)
{
	stringstream ss;
	ss << Round(d,u);
	return ss.str();
}

inline double STool::ToDbl(const string &s)
{
	// can't distinguish error from conversion of 0
	//return (double)atof(s.c_str());
	// an alternative way
	//double num = std::strtod(nptr, NULL);

	istringstream ss(s);
	double d;
	if (!(ss >> d))
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	return d;
}
inline double STool::ToDbl(const string &s, unsigned u)
{
	return Round(ToDbl(s),u);
}
inline int STool::ToInt(const string &s)
{
	return IntPart1(ToDbl(s));
}

//PadLeft, PadRight
inline string STool::PadLeft(unsigned iOri, char cPad, unsigned iLen)
{
	return PadLeft(ToStr(iOri), cPad, iLen);
}
inline string STool::PadLeft(const string &sOri, char cPad, unsigned iLen)
{
	if (iLen < 0 || sOri.length() >= iLen) return sOri;
	else
	{
		string sPad; sPad = cPad;
		string sR(sOri);
		while (sR.length() < iLen) sR = sPad + sR;
		return sR;
	}
}
inline string STool::PadRight(unsigned iOri, char cPad, unsigned iLen)
{
	return PadRight(ToStr(iOri), cPad, iLen);
}
inline string STool::PadRight(string sOri, char cPad, unsigned iLen)
{
	if (iLen < 0 || sOri.length() >= iLen) return sOri;
	else
	{
		string sPad; sPad = cPad;
		string sR(sOri);
		while (sR.length() < iLen) sR = sR + sPad;
		return sR;
	}
}


//Construct String
inline string STool::ConstructStr(const string *s, int iSize, const string &sDelim)
{
	string sR = "";
	//int iSize = sizeof(s) / sizeof(s[0]);

	if (iSize == 0) return sR;
	else
	{
		for (int i = 0; i < iSize; ++i)
			if (i < iSize - 1) sR += s[i] + sDelim;
			else sR += s[i];
		return sR;
	}
}

//NaN
inline bool STool::IsNaN(double value)
{
	return value != value;
}

//map
	template<typename T1,typename T2>
inline bool STool::ContainsKey(map<T1,T2> m,T1 k)
{
	if (m.find(k) == m.end()) return false;
	else return true;
}

//encapsulation for reading file
deque<string> STool::ReadFile(const char *sPath)
{
	deque<string> dqS;
	ifstream ifs(sPath, ios::in);
	if (ifs.is_open())
	{
		for (string str; getline(ifs,str); ) dqS.push_back(str);
	}
	else
	{
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
	ifs.close();
	return dqS;
}



#endif
