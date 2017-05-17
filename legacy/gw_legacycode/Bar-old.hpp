#ifndef BAR_HPP
#define BAR_HPP

#include "STool.hpp"
#include "Tokenizer.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace std;
using namespace boost::gregorian;
using namespace boost::posix_time;

template <typename T1, typename T2>
class Bar
{
	private:
		ptime _bptTime;
		T1 _dOpen;
		T1 _dHigh;
		T1 _dLow;
		T1 _dClose;
		T2 _lVolume;

	public:
		Bar(const string &,const string &,T1,T1,T1,T1,T2);
		~Bar();

		string YYYYMMDDs() const;
		int YMDi() const;
		int YMi() const;
		int HMSi() const;
		int HMi() const;
		ptime TimeBPT() const;

		int Year() const;
		int Month() const;
		int Day() const;
		int Hour() const;
		int Minute() const;
		int Second() const;

		T1 Open const;
		T1 High const;
		T1 Low const;
		T1 Close const;
		T2 Volume const;
};


template <typename T1, typename T2>
Bar<T1,T2>::Bar(const string &sD, const string &sT, T1 dO, T1 dH, T1 dL, T1 dC, T2 lV)
{
	//Date and Time
	int iYMD[3];
	int iHMS[3];
	int iCnt = 0;

	for (int i = 0; i < sizeof(iYMD)/sizeof(int); ++i)
	{
		iYMD[i] = 0;
		iHMS[i] = 0;
	}

	{
		Tokenizer tz(sD, "-/:");
		while (tz.NextToken() && iCnt < sizeof(iYMD)/sizeof(int))
		{
			istringstream buf((string)(tz.GetToken()));
			buf >> iYMD[iCnt++];
		}
	}

	if (sT.length() > 0)
	{
		iCnt = 0;
		Tokenizer tz(sT, "-/:");
		while (tz.NextToken() && iCnt < sizeof(iHMS)/sizeof(int))
		{
			istringstream buf((string)(tz.GetToken()));
			buf >> iHMS[iCnt++];
		}
	}

	ptime pt(date(iYMD[0],iYMD[1],iYMD[2]),hours(iHMS[0])+minutes(iHMS[1])+seconds(iHMS[2]));
	_bptTime = pt;



	//OHLCV
	_dOpen = dO;
	_dHigh = dH;
	_dLow = dL;
	_dClose = dC;
	_lVolume = lV;
}

template <typename T1, typename T2>
Bar<T1,T2>::~Bar()
{
}

template <typename T1, typename T2>
ptime Bar<T1,T2>::TimeBPT() const { return _bptTime; }

template <typename T1, typename T2>
string Bar<T1,T2>::YYYYMMDDs() const
{
	date d = _bptTime.date();
	int iY = d.year();
	int iM = d.month();
	int iD = d.day();

	string sRtn = STool::YMDs(iY,iM,iD);

	return sRtn;
}

template <typename T1, typename T2>
int Bar<T1,T2>::YMDi() const { return STool::YMDi(Year(),Month(),Day()); }
template <typename T1, typename T2>
int Bar<T1,T2>::YMi() const { return STool::YMi(Year(),Month()); }
template <typename T1, typename T2>
int Bar<T1,T2>::HMSi() const { return STool::HMSi(Hour(),Minute(),Second()); }
template <typename T1, typename T2>
int Bar<T1,T2>::HMi() const { return STool::HMi(Hour(),Minute()); }

template <typename T1, typename T2>
int Bar<T1,T2>::Year() const { int iY = _bptTime.date().year(); return iY; }
template <typename T1, typename T2>
int Bar<T1,T2>::Month() const { int iM = _bptTime.date().month(); return iM; }
template <typename T1, typename T2>
int Bar<T1,T2>::Day() const { int iD = _bptTime.date().day(); return iD; }

template <typename T1, typename T2>
int Bar<T1,T2>::Hour() const { int iH = _bptTime.time_of_day().hours(); return iH; }
template <typename T1, typename T2>
int Bar<T1,T2>::Minute() const { int iM = _bptTime.time_of_day().minutes(); return iM; }
template <typename T1, typename T2>
int Bar<T1,T2>::Second() const { int iS = _bptTime.time_of_day().seconds(); return iS;}

template <typename T1, typename T2>
T1 Bar<T1,T2>::Open const { return _dOpen; }
template <typename T1, typename T2>
T1 Bar<T1,T2>::High const { return _dHigh; }
template <typename T1, typename T2>
T1 Bar<T1,T2>::Low const { return _dLow; }
template <typename T1, typename T2>
T1 Bar<T1,T2>::Close const { return _dClose; }
template <typename T1, typename T2>
T2 Bar<T1,T2>::Volume const { return _lVolume; }

#endif
