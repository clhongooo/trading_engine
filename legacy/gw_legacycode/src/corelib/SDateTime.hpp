#ifndef SDATE_HPP
#define SDATE_HPP

#include "STool.hpp"
#include <iomanip>

using namespace std;

// A collection of date related functions
// 
// Design considerations:
// 
// Frequency of operations:
// = Market data feed:
// . Just put and get whole date / time as YYYYMMDD / HHMMSS
// . Compare with the current time in trade engine
// 
// = Trade engine:
// . Compare HHMMSS against market open / close time (Every clock tick)
// . Date / time arithmetic: Add 1 day / 1 month (Every clock tick)
// . Calculate date time difference (Mark-to-market)
// 
// = Strategy:
// . Get individual components (In strategy, to check AM / PM, whether specific time period is reached etc)
// . Set individual components (???)

// Date / time is deliberately not checked for correctness to achieve max speed.

typedef double GregorianDayNum;
typedef double JulianDayNum;

int BAD_DATETIME = -4667;
int DAYSINMTH[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

class YYYYMMDD;
class HHMMSS;
class YMD;
class HMS;
class SDateTime;


class SDateTime
{
	public:
		//static inline void JulianDayNum2YMD(JulianDayNum jdn, YMD & ymd);
		static inline JulianDayNum CalcJulianDayNum(int y, int m, int d);
		static inline GregorianDayNum CalcGregorianDayNum(int y, int m, int d);
		static inline int CalendarDayDiff(YMD & ymd1, YMD & ymd2);

		static inline bool IsLeapYear(int y);
		static inline bool IsValidYMD(int year, int month, int day);
		static inline int DaysInMonth(int y,int m);
		static inline int DaysInYear(int year);
		static inline int DayOfWeek(const YMD & ymd);
		static inline bool IsEndOfMonth(const YMD & ymd);
		static inline bool IsWeekDay(const YMD & ymd);

};


class YYYYMMDD
{
	friend class YMD;

	private:
		int _iYMD;

	public:
		YYYYMMDD(int iYMD) { _iYMD = iYMD; }
		YYYYMMDD(const string &s) { _iYMD = STool::ToInt(s); }

		// setter and getter
		void Set(int iYMD) { _iYMD = iYMD; }
		void Set(int iY,int iM,int iD) { _iYMD = iY*10000 + iM*100 + iD; }
		int Value() const { return _iYMD; }

		// supported functions
		// comparators 1
		bool operator==(const YYYYMMDD& ymd) const;
		bool operator!=(const YYYYMMDD& ymd) const;
		bool operator> (const YYYYMMDD& ymd) const;
		bool operator>=(const YYYYMMDD& ymd) const;
		bool operator< (const YYYYMMDD& ymd) const;
		bool operator<=(const YYYYMMDD& ymd) const;
		// comparators 2
		bool operator==(const YMD& ymd) const;
		bool operator!=(const YMD& ymd) const;
		bool operator> (const YMD& ymd) const;
		bool operator>=(const YMD& ymd) const;
		bool operator< (const YMD& ymd) const;
		bool operator<=(const YMD& ymd) const;

		// export
		string ToStrYYYYMM() const { return STool::ToStr(_iYMD/100); }
		string ToStrYYYYMMDD() const { return STool::ToStr(_iYMD); }
		void ToYMD(YMD & ymd) const;
};

class YMD
{
	friend class YYYYMMDD;

	private:
		short _iY;
		short _iM;
		short _iD;
		// export
		int ToInt() const { return (_iY*10000+_iM*100+_iD); }

	public:
		YMD() { _iY = -1; _iM = -1; _iD = -1; }
		YMD(int iY,int iM,int iD) { _iY = iY; _iM = iM; _iD = iD; }

		// setters and getters
		void SetYear(short iY)  { _iY = iY; }
		void SetMonth(short iM) { _iM = iM; }
		void SetDay(short iD)   { _iD = iD; }
		void Set(short iY, short iM, short iD)  { _iY = iY; _iM = iM; _iD = iD; }
		short Year()  const { return _iY; }
		short Month() const { return _iM; }
		short Day()   const { return _iD; }

		// supported functions
		// comparators 1
		bool operator==(const YMD& ymd) const;
		bool operator!=(const YMD& ymd) const;
		bool operator> (const YMD& ymd) const;
		bool operator>=(const YMD& ymd) const;
		bool operator< (const YMD& ymd) const;
		bool operator<=(const YMD& ymd) const;
		// comparators 2
		bool operator==(const YYYYMMDD& ymd) const;
		bool operator!=(const YYYYMMDD& ymd) const;
		bool operator> (const YYYYMMDD& ymd) const;
		bool operator>=(const YYYYMMDD& ymd) const;
		bool operator< (const YYYYMMDD& ymd) const;
		bool operator<=(const YYYYMMDD& ymd) const;

		// changing date
		void AddYear(short iY) { _iY += iY; }
		void SubtractYear(short iY) { _iY -= iY; }
		void AddMonth(short iM) { if (iM < 0) { SubtractMonth(-iM); return; } _iM += iM; _iY += ((_iM-1)/12); _iM = ((_iM-1)%12)+1; }
		void SubtractMonth(short iM) { if (iM < 0) { AddMonth(-iM); return; } _iY -= (iM / 12); iM = (iM % 12); if (iM >= _iM) { --_iY; iM-=_iM; _iM=12; } _iM-=iM; }
		// definitely favouring small increments / decrements in days; avoid conversion to Julian date for efficiency's sake
		void AddDay(short iD)
		{
			if (iD < 0) { SubtractDay(-iD); return; }

			int iDaysInMth = SDateTime::DaysInMonth(_iY,_iM);
			while (_iD + iD > iDaysInMth)
			{
				_iD = 1;
				iD -= (iDaysInMth - _iD);
				AddMonth(1);
				iDaysInMth = SDateTime::DaysInMonth(_iY,_iM);
			}
			_iD += iD;
		}
		void SubtractDay(short iD)
		{
			if (iD < 0) { AddDay(-iD); return; }

			while (_iD - iD <= 0)
			{
				SubtractMonth(1);
				iD -= _iD;
				_iD = SDateTime::DaysInMonth(_iY,_iM);
			}
			_iD -= iD;
		}

		// export
		void ToYYYYMMDD(YYYYMMDD & yyyymmdd) const { yyyymmdd.Set(_iY,_iM,_iD); }
		string ToStrYYYYMM() { return STool::ToStr(_iY*100 + _iM); }
		string ToStrYYYYMMDD() { return STool::ToStr(_iY*10000 + _iM*100 + _iD); }
};


bool YYYYMMDD::operator==(const YYYYMMDD& ymd) const { return (_iYMD == ymd._iYMD) ? true : false; }
bool YYYYMMDD::operator!=(const YYYYMMDD& ymd) const { return (_iYMD != ymd._iYMD) ? true : false; }
bool YYYYMMDD::operator> (const YYYYMMDD& ymd) const { return (_iYMD >  ymd._iYMD) ? true : false; }
bool YYYYMMDD::operator>=(const YYYYMMDD& ymd) const { return (_iYMD >= ymd._iYMD) ? true : false; }
bool YYYYMMDD::operator< (const YYYYMMDD& ymd) const { return (_iYMD <  ymd._iYMD) ? true : false; }
bool YYYYMMDD::operator<=(const YYYYMMDD& ymd) const { return (_iYMD <= ymd._iYMD) ? true : false; }
bool YYYYMMDD::operator==(const YMD& ymd) const { return (_iYMD == ymd.ToInt()) ? true : false; }
bool YYYYMMDD::operator!=(const YMD& ymd) const { return (_iYMD != ymd.ToInt()) ? true : false; }
bool YYYYMMDD::operator> (const YMD& ymd) const { return (_iYMD >  ymd.ToInt()) ? true : false; }
bool YYYYMMDD::operator>=(const YMD& ymd) const { return (_iYMD >= ymd.ToInt()) ? true : false; }
bool YYYYMMDD::operator< (const YMD& ymd) const { return (_iYMD <  ymd.ToInt()) ? true : false; }
bool YYYYMMDD::operator<=(const YMD& ymd) const { return (_iYMD <= ymd.ToInt()) ? true : false; }
bool YMD::operator==(const YYYYMMDD& ymd) const { return (this->ToInt() == ymd._iYMD) ? true : false; }
bool YMD::operator!=(const YYYYMMDD& ymd) const { return (this->ToInt() != ymd._iYMD) ? true : false; }
bool YMD::operator> (const YYYYMMDD& ymd) const { return (this->ToInt() >  ymd._iYMD) ? true : false; }
bool YMD::operator>=(const YYYYMMDD& ymd) const { return (this->ToInt() >= ymd._iYMD) ? true : false; }
bool YMD::operator< (const YYYYMMDD& ymd) const { return (this->ToInt() <  ymd._iYMD) ? true : false; }
bool YMD::operator<=(const YYYYMMDD& ymd) const { return (this->ToInt() <= ymd._iYMD) ? true : false; }
bool YMD::operator==(const YMD& ymd) const { return (this->ToInt() == ymd.ToInt()) ? true : false; }
bool YMD::operator!=(const YMD& ymd) const { return (this->ToInt() != ymd.ToInt()) ? true : false; }
bool YMD::operator> (const YMD& ymd) const { return (this->ToInt() >  ymd.ToInt()) ? true : false; }
bool YMD::operator>=(const YMD& ymd) const { return (this->ToInt() >= ymd.ToInt()) ? true : false; }
bool YMD::operator< (const YMD& ymd) const { return (this->ToInt() <  ymd.ToInt()) ? true : false; }
bool YMD::operator<=(const YMD& ymd) const { return (this->ToInt() <= ymd.ToInt()) ? true : false; }


void YYYYMMDD::ToYMD(YMD & ymd) const { ymd.SetYear(_iYMD/10000); ymd.SetMonth((_iYMD%10000)/100); ymd.SetDay(_iYMD%100); }


class HHMMSS
{
	friend class HMS;

	private:
		int _iHMS;

	public:
		HHMMSS(int iHMS) { if (iHMS > 9999) _iHMS = iHMS; else _iHMS = iHMS*100; }
		HHMMSS(const string &s) { HHMMSS(STool::ToInt(s)); }

		// setter and getter
		void Set(int iHMS) { if (iHMS > 9999) _iHMS = iHMS; else _iHMS = iHMS*100; }
		void Set(int iH,int iM,int iS) { _iHMS = iH*10000 + iM*100 + iS; }
		void Set(int iH,int iM) { _iHMS = iH*10000 + iM*100; }
		int Value() const { return _iHMS; }

		// supported functions
		// comparators 1
		bool operator==(const HHMMSS& hms) const;
		bool operator!=(const HHMMSS& hms) const;
		bool operator> (const HHMMSS& hms) const;
		bool operator>=(const HHMMSS& hms) const;
		bool operator< (const HHMMSS& hms) const;
		bool operator<=(const HHMMSS& hms) const;
		// comparators 2
		bool operator==(const HMS& hms) const;
		bool operator!=(const HMS& hms) const;
		bool operator> (const HMS& hms) const;
		bool operator>=(const HMS& hms) const;
		bool operator< (const HMS& hms) const;
		bool operator<=(const HMS& hms) const;

		// export
		string ToStrHHMM() const { std::ostringstream ss; ss << std::setw(4) << std::setfill('0') << _iHMS/100; return ss.str(); }
		string ToStrHHMMSS() const { std::ostringstream ss; ss << std::setw(6) << std::setfill('0') << _iHMS; return ss.str(); }

		void ToHMS(HMS & hms) const;
};

class HMS
{
	friend class HHMMSS;

	private:
		short _iH;
		short _iM;
		short _iS;
		// export
		int ToInt() const { return (_iH*10000+_iM*100+_iS); }

	public:
		HMS(int iH,int iM,int iS) { _iH = iH; _iM = iM; _iS = iS; }

		// setters and getters
		void SetHour(short iH)  { _iH = iH; }
		void SetMinute(short iM) { _iM = iM; }
		void SetSecond(short iS)   { _iS = iS; }
		short Hour()  const { return _iH; }
		short Minute() const { return _iM; }
		short Second()   const { return _iS; }

		// supported functions
		// comparators 1
		bool operator==(const HMS& hms) const;
		bool operator!=(const HMS& hms) const;
		bool operator> (const HMS& hms) const;
		bool operator>=(const HMS& hms) const;
		bool operator< (const HMS& hms) const;
		bool operator<=(const HMS& hms) const;
		// comparators 2
		bool operator==(const HHMMSS& hms) const;
		bool operator!=(const HHMMSS& hms) const;
		bool operator> (const HHMMSS& hms) const;
		bool operator>=(const HHMMSS& hms) const;
		bool operator< (const HHMMSS& hms) const;
		bool operator<=(const HHMMSS& hms) const;

		// changing time
		void AddMinute(short iM);
		void SubtractMinute(short iM);

		// export
		void ToHHMMSS(HHMMSS & hhmmss) const { hhmmss.Set(_iH,_iM,_iS); }
		string ToStrHHMM() const { std::ostringstream ss; ss << std::setw(4) << std::setfill('0') << _iH*100 + _iM; return ss.str(); }
		string ToStrHHMMSS() const { std::ostringstream ss; ss << std::setw(6) << std::setfill('0') << _iH*10000 + _iM*100 + _iS; return ss.str(); }


};


bool HHMMSS::operator==(const HHMMSS& hms) const { return (_iHMS == hms._iHMS) ? true : false; }
bool HHMMSS::operator!=(const HHMMSS& hms) const { return (_iHMS != hms._iHMS) ? true : false; }
bool HHMMSS::operator> (const HHMMSS& hms) const { return (_iHMS >  hms._iHMS) ? true : false; }
bool HHMMSS::operator>=(const HHMMSS& hms) const { return (_iHMS >= hms._iHMS) ? true : false; }
bool HHMMSS::operator< (const HHMMSS& hms) const { return (_iHMS <  hms._iHMS) ? true : false; }
bool HHMMSS::operator<=(const HHMMSS& hms) const { return (_iHMS <= hms._iHMS) ? true : false; }
bool HHMMSS::operator==(const HMS& hms) const { return (_iHMS == hms.ToInt()) ? true : false; }
bool HHMMSS::operator!=(const HMS& hms) const { return (_iHMS != hms.ToInt()) ? true : false; }
bool HHMMSS::operator> (const HMS& hms) const { return (_iHMS >  hms.ToInt()) ? true : false; }
bool HHMMSS::operator>=(const HMS& hms) const { return (_iHMS >= hms.ToInt()) ? true : false; }
bool HHMMSS::operator< (const HMS& hms) const { return (_iHMS <  hms.ToInt()) ? true : false; }
bool HHMMSS::operator<=(const HMS& hms) const { return (_iHMS <= hms.ToInt()) ? true : false; }
bool HMS::operator==(const HMS& hms) const { return (this->ToInt() == hms.ToInt()) ? true : false; }
bool HMS::operator!=(const HMS& hms) const { return (this->ToInt() != hms.ToInt()) ? true : false; }
bool HMS::operator> (const HMS& hms) const { return (this->ToInt() >  hms.ToInt()) ? true : false; }
bool HMS::operator>=(const HMS& hms) const { return (this->ToInt() >= hms.ToInt()) ? true : false; }
bool HMS::operator< (const HMS& hms) const { return (this->ToInt() <  hms.ToInt()) ? true : false; }
bool HMS::operator<=(const HMS& hms) const { return (this->ToInt() <= hms.ToInt()) ? true : false; }
bool HMS::operator==(const HHMMSS& hms) const { return (this->ToInt() == hms._iHMS) ? true : false; }
bool HMS::operator!=(const HHMMSS& hms) const { return (this->ToInt() != hms._iHMS) ? true : false; }
bool HMS::operator> (const HHMMSS& hms) const { return (this->ToInt() >  hms._iHMS) ? true : false; }
bool HMS::operator>=(const HHMMSS& hms) const { return (this->ToInt() >= hms._iHMS) ? true : false; }
bool HMS::operator< (const HHMMSS& hms) const { return (this->ToInt() <  hms._iHMS) ? true : false; }
bool HMS::operator<=(const HHMMSS& hms) const { return (this->ToInt() <= hms._iHMS) ? true : false; }

void HMS::AddMinute(short iM) { if (iM < 0) { SubtractMinute(-iM); return; } }
void HMS::SubtractMinute(short iM) { if (iM < 0) { AddMinute(-iM); return; } }

void HHMMSS::ToHMS(HMS & hms) const { hms.SetHour(_iHMS/10000); hms.SetMinute((_iHMS%10000)/100); hms.SetSecond(_iHMS%100); }


//----------------------------------------------------------------------------------------------------


inline bool SDateTime::IsLeapYear(int y)
{
	if (y % 400 == 0 || (y % 4 == 0 && y % 100 != 0)) return true;
	else return false;    // not a leap year
}

// inline void SDateTime::JulianDayNum2YMD(JulianDayNum jdn, YMD & ymd)
// {
// 	if (jdn <= 0)
// 	{
// 		SException se;
// 		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
// 		throw se;
// 	}
// 
// 	// Algorithm From Wikipedia
// 	int
// 		y = 4716,
// 			j = 1401,
// 			m = 2,
// 			n = 12,
// 			r = 4,
// 			p = 1461,
// 			v = 3,
// 			u = 5,
// 			s = 153,
// 			w = 2,
// 			B = 274277,
// 			C = -38;
// 
// 	int f = jdn + j + (((4 * jdn + B) / 146097) * 3)/4 + C;
// 	int e = r * f + v;
// 	int g = (e % p) / r;
// 	int h = u * g + w;
// 
// 	int iM = ((h/s + m) % n) + 1;
// 	int iY = e / p - y + (n + m - iM)/n;
// 	int iD = (h % s) / u + 1;
// 	ymd.SetYear(iY);
// 	ymd.SetMonth(iM);
// 	ymd.SetDay(iD);
// 	return;
// 
// }

inline bool SDateTime::IsValidYMD(int year, int month, int day)
{
	if (year < 1900 ||
			year > 2100 ||
			month <= 0 ||
			month > 12 ||
			day <= 0)
		return false;

	if (month != 2 || !SDateTime::IsLeapYear(year))
	{
		if (day > DAYSINMTH[month-1]) return false;
	}
	else
	{
		if (day > DAYSINMTH[month-1]+1) return false;
	}

	return true;
}

inline GregorianDayNum SDateTime::CalcGregorianDayNum(int y, int m, int d)
{
	// Stack Overflow
	m = (m + 9) % 12;
	y = y - m/10;
	return 365*y + y/4 - y/100 + y/400 + (m*306 + 5)/10 + ( d - 1 );
}

inline JulianDayNum SDateTime::CalcJulianDayNum(int y, int m, int d)
{
	// Following the calculation method in Wikipedia
	JulianDayNum a = (14 - m)/12;
	JulianDayNum y1 = y+4800-a;
	JulianDayNum m1 = m+12*a-3;
	return d+(153*m1+2)/5+365*y1+y1/4-y1/100+y1/400-32045;
}


// 20130916 - 20130917 = 1 day of difference
// 20130917 - 20130917 = 0 day of difference
inline int SDateTime::CalendarDayDiff(YMD & ymd1, YMD & ymd2)
{
	if (ymd1 == ymd2) return 0;
	else
	{
		return STool::Abs((CalcGregorianDayNum(ymd2.Year(),ymd2.Month(),ymd2.Day()) - CalcGregorianDayNum(ymd1.Year(),ymd1.Month(),ymd1.Day())));
	}
}

// Getting the no of days for the particular month
inline int SDateTime::DaysInMonth(int y,int m)
{
	return (m == 2 && IsLeapYear(y)) ? 29 : DAYSINMTH[m-1];
}

inline int SDateTime::DaysInYear(int year)
{
	return (IsLeapYear(year) ? 366 : 365);
}

inline bool SDateTime::IsEndOfMonth(const YMD & ymd)
{
	if (ymd.Month() == 2 && IsLeapYear(ymd.Year()))
		return (ymd == 29) ? true : false;
	else
		return (ymd.Day() == DAYSINMTH[ymd.Month()-1]) ? true : false;
}

// Sunday = 0, Monday = 1, ... Saturday = 6
inline int SDateTime::DayOfWeek(const YMD & ymd)
{
	JulianDayNum jdn = CalcJulianDayNum(ymd.Year(),ymd.Month(),ymd.Day());

	int wkday = ((int)jdn+1)%7;
	return wkday;
}

// Weekday means Mon - Fri
inline bool SDateTime::IsWeekDay(const YMD & ymd)
{
	switch (DayOfWeek(ymd))
	{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			return true;
		default:
			return false;
	}
}




#endif

//----------------------------------------------------------------------------------------
		// Legacy functions
		// inline int HMSi(const string &s);
		// inline int HMSi(int iH, int iM, int iS);
		// inline int HMi(const string &s);
		// inline int HMi(int iH, int iM);
		// inline int HMi(int iH, int iM, int iS);
		// inline int MthAbsDiff(int iYM1, int iYM2);
		// inline int MthDiff(int iYM1, int iYM2);
		// inline int NextHMi(int iHM, int iNext);
		// inline int NextYMi(int iA, int iNext);
		// inline int YMDi(int iY, int iM, int iD);
		// inline int YMi(const string &s);
		// inline int YMi(int iA);
		// inline int YMi(int iY, int iM);
		// inline int YMi(int iY, int iM, int iD);
		// inline string HMSs(int iA);
		// inline string HMSs(int iH, int iM, int iS);
		// inline string HMs(int iA);
		// inline string HMs(int iH, int iM);
		// inline string HMs(int iH, int iM, int iS);
		// inline string YMDs(int iA);
		// inline string YMDs(int iY, int iM, int iD);
		// inline string YMs(int iA);
		// inline string YMs(int iY, int iM);
		// inline string YMs(int iY, int iM, int iD);
		// inline string YYYY_YY(int iY);

//----------------------------------------------------------------------------------------

// inline int SDateTime::MthDiff(int iYM1, int iYM2)
// {
// 	if (iYM1 > 999999 || iYM2 > 999999)
// 	{
// 		SException se;
// 		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
// 		throw se;
// 	}
// 	if (iYM1 == iYM2) return 0;
// 
// 	int iYM1_ = (iYM1/100)*12 + (iYM1%100);
// 	int iYM2_ = (iYM2/100)*12 + (iYM2%100);
// 	return iYM2_-iYM1_;
// }
// inline int SDateTime::MthAbsDiff(int iYM1, int iYM2)
// {
// 	return (int)abs(MthDiff(iYM1,iYM2));
// }
// 
// inline string SDateTime::YYYY_YY(int iY)
// {
// 	if (!ValidityChk::Year(iY))
// 	{
// 		SException se;
// 		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
// 		throw se;
// 	}
// 	return PadLeft(iY%100,'0',2);
// }
// 
// 
// //YMD (string) to YM (int)
// inline int SDateTime::YMi(const string &s)
// {
// 	int iR = 0;
// 	string sY,sM;
// 
// 	Tokenizer tz(s);
// 	if (tz.NextToken()) sY = tz.GetToken();
// 	else
// 	{
// 		SException se;
// 		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
// 		throw se;
// 	}
// 	if (tz.NextToken()) sM = tz.GetToken();
// 	else
// 	{
// 		SException se;
// 		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
// 		throw se;
// 	}
// 
// 	iR += ToInt(sY)*100;
// 	iR += ToInt(sM)*1;
// 	return iR;
// }
// //YMD (int) to YM (int)
// inline int SDateTime::YMi(int iA)
// {
// 	if (iA > 999999) return iA / 100;
// 	else return iA;
// }
// 
// //HMS to int
// inline int SDateTime::HMSi(const string &s)
// {
// 	int iR = 0;
// 	string sH,sM,sS;
// 
// 	Tokenizer tz(s,":");
// 	if (tz.NextToken()) sH = tz.GetToken();
// 	else
// 	{
// 		SException se;
// 		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
// 		throw se;
// 	}
// 	if (tz.NextToken()) sM = tz.GetToken();
// 	else
// 	{
// 		SException se;
// 		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
// 		throw se;
// 	}
// 	if (tz.NextToken()) sS = tz.GetToken();
// 	else sS = "00";
// 
// 	iR += ToInt(sH)*10000;
// 	iR += ToInt(sM)*100;
// 	iR += ToInt(sS)*1;
// 	return iR;
// }
// inline int SDateTime::HMi(const string &s)
// {
// 	int iR = 0;
// 	string sH,sM;
// 
// 	Tokenizer tz(s,":");
// 	if (tz.NextToken()) sH = tz.GetToken();
// 	else
// 	{
// 		SException se;
// 		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
// 		throw se;
// 	}
// 	if (tz.NextToken()) sM = tz.GetToken();
// 	else
// 	{
// 		SException se;
// 		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
// 		throw se;
// 	}
// 
// 	iR += ToInt(sH)*100;
// 	iR += ToInt(sM)*1;
// 	return iR;
// }
// 
// //YMD (int) to YMD (string)
// inline string SDateTime::YMDs(int iA)
// {
// 	if (iA < 19000000)
// 	{
// 		SException se;
// 		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
// 		throw se;
// 	}
// 	int iY = iA / 10000;
// 	int iM = (iA % 10000) / 100;
// 	int iD = (iA % 100);
// 
// 	return YMDs(iY,iM,iD);
// }
// //Int to YM
// inline string SDateTime::YMs(int iA)
// {
// 	if (iA < 190000)
// 	{
// 		SException se;
// 		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
// 		throw se;
// 	}
// 	if (iA > 999999)
// 	{
// 		int iY = iA / 10000;
// 		int iM = (iA % 10000) / 100;
// 		int iD = (iA % 100);
// 
// 		return YMs(iY,iM,iD);
// 	}
// 	else
// 	{
// 		int iY = iA / 100;
// 		int iM = iA % 100;
// 
// 		return YMs(iY,iM);
// 	}
// }
// 
// //Int to HMS
// inline string SDateTime::HMSs(int iA)
// {
// 	if (iA < 9999)
// 	{
// 		SException se;
// 		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
// 		throw se;
// 	}
// 	int iH = iA / 10000;
// 	int iM = (iA % 10000) / 100;
// 	int iS = (iA % 100);
// 
// 	return HMSs(iH,iM,iS);
// }
// //Int to HM
// inline string SDateTime::HMs(int iA)
// {
// 	if (iA < 99)
// 	{
// 		SException se;
// 		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
// 		throw se;
// 	}
// 	if (iA > 9999)
// 	{
// 		int iH = iA / 10000;
// 		int iM = (iA % 10000) / 100;
// 		int iS = (iA % 100);
// 
// 		return HMs(iH,iM,iS);
// 	}
// 	else
// 	{
// 		int iH = iA / 100;
// 		int iM = iA % 100;
// 
// 		return HMs(iH,iM);
// 	}
// }
// 
// //Int to YMD
// inline string SDateTime::YMDs(int iY, int iM, int iD)
// {
// 	return ToStr(iY) + "-" + PadLeft(ToStr(iM),'0',2) + "-" + PadLeft(ToStr(iD),'0',2);
// }
// 
// //Int to YM
// inline string SDateTime::YMs(int iY, int iM, int iD)
// {
// 	return YMs(iY,iM);
// }
// 
// //Int to YM
// inline string SDateTime::YMs(int iY, int iM)
// {
// 	return ToStr(iY) + "-" + PadLeft(ToStr(iM),'0',2);
// }
// 
// 
// //Int to HMS
// inline string SDateTime::HMSs(int iH, int iM, int iS)
// {
// 	return PadLeft(ToStr(iH),'0',2) + ":" + PadLeft(ToStr(iM),'0',2) + ":" + PadLeft(ToStr(iS),'0',2);
// }
// 
// 
// //Int to HM
// inline string SDateTime::HMs(int iH, int iM, int iS)
// {
// 	return HMs(iH,iM);
// }
// 
// //Int to YMD
// inline int SDateTime::YMDi(int iY, int iM, int iD)
// {
// 	return iY*10000 + iM*100 + iD;
// }
// 
// //Int to YM
// inline int SDateTime::YMi(int iY, int iM, int iD)
// {
// 	return YMi(iY,iM);
// }
// 
// //Int to YM
// inline int SDateTime::YMi(int iY, int iM)
// {
// 	return iY*100 + iM;
// }
// 
// //Int to HMS
// inline int SDateTime::HMSi(int iH, int iM, int iS)
// {
// 	return iH*10000 + iM*100 + iS;
// }
// 
// //Int to HM
// inline int SDateTime::HMi(int iH, int iM, int iS)
// {
// 	return HMi(iH,iM);
// }
// 
// //Int to HM
// inline string SDateTime::HMs(int iH, int iM)
// {
// 	return ToStr(iH) + ":" + PadLeft(ToStr(iM),'0',2);
// }
// 
// //Int to HM
// inline int SDateTime::HMi(int iH, int iM)
// {
// 	return iH*100 + iM;
// }
// 
// inline int SDateTime::NextYMi(int iA, int iNext)
// {
// 	if (iA < 999999)
// 	{
// 		int iY = iA / 100;
// 		int iM = iA % 100;
// 
// 		if (iM == 13) { iY += 1; iM = 1; }
// 		if (iM == 0) { iY -= 1; iM = 12; }
// 
// 		if (!ValidityChk::Year(iY) || !ValidityChk::Month(iM))
// 		{
// 			SException se;
// 			se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
// 			throw se;
// 		}
// 
// 		if (iNext > 0) return NextYMi(iY*100+iM+1,iNext-1);
// 		else if (iNext < 0) return NextYMi(iY*100+iM-1,iNext+1);
// 		else return iY*100+iM;
// 	}
// 	else
// 	{
// 		SException se;
// 		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
// 		throw se;
// 	}
// }
// 
// 
// inline int SDateTime::NextHMi(int iHM, int iNext)
// {
// 	if (iHM < 9999)
// 	{
// 		int iH = iHM / 100;
// 		int iM = iHM % 100;
// 
// 		if (iM == 60) { iH += 1; iM = 0; }
// 		if (iM == 99) { iM = 59; } //Hour already reduced by 1
// 		if (iM == -1) { iH = 23; iM = 59; } //going back past 0000
// 		if (iH == 24) { iH = 0; } //2359+1=2360>2400
// 
// 		if (!ValidityChk::Hour(iH) || !ValidityChk::Minute(iM))
// 		{
// 			SException se;
// 			se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
// 			throw se;
// 		}
// 
// 		if (iNext > 0) return NextHMi(iH*100+iM+1,iNext-1);
// 		else if (iNext < 0) return NextHMi(iH*100+iM-1,iNext+1);
// 		else return iH*100+iM;
// 	}
// 	else
// 	{
// 		SException se;
// 		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
// 		throw se;
// 	}
// }



