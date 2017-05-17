#ifndef BARPROVIDER_HPP
#define BARPROVIDER_HPP

#include "boost/iostreams/filtering_stream.hpp"
#include "boost/iostreams/filter/gzip.hpp"
#include "STool.hpp"
#include "Tokenizer.hpp"
#include "SException.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <deque>

using namespace std;

// The design goal is to make this backbone class as fast and clean as possible
// Template for flexible member variable type is removed, because there is no such need
class BarProvider
{
	private:
		bool _bMaster;
		int _iPsn;
		int _iNumOfBars;
		char _cFwdBkwd; // Determines the direction _iPsn moves

		bool _bTimeFmt; // HMS = true; HM = false

		// Low level structure exposed, but speed is more desirable than encapsulation. Also, we don't expect the bar structure to change at all
		vector<YYYYMMDD> * _viYMD; //YYYYMMDD
		vector<HHMMSS> * _viTime; //HHMMSS or HHMM

		// Can't use float in place of double because of the lack of support from GSL
		vector<double> * _vdOpen;
		vector<double> * _vdHigh;
		vector<double> * _vdLow;
		vector<double> * _vdClose;
		vector<double> * _vdReturn;
		vector<int> * _viVolume;

		// Forcing the use of Copy Constructor
		BarProvider& operator=(const BarProvider& rhs ) {}

	public:
		const static string _sDOHLCV;
		const static string _sDTOHLCV;

		BarProvider() {}
		BarProvider(const char *, const string &, int, char, char, char);
		BarProvider(const BarProvider &);
		~BarProvider();
		void FreeVec();
		void LoadDataFile(const char *, const string &, int, char, char, char);
		void ResetPsn();
		int GetNumOfBars() const;
		bool HasNextBar() const;
		const int PeekNextYMDi() const;
		const int PeekPrevYMDi();
		const double PeekPrevClose();
		void MoveNext();
		const double* GetCloseArr(int);
		const double* GetReturnArr(int);
		const int YMDi() const;
		const int HMSi() const;
		const int HMi() const;
		const double Open() const;
		const double High() const;
		const double Low() const;
		const double Close() const;
		const double Return() const;
		const int Volume() const;

};


const string BarProvider::_sDOHLCV = "DOHLCV";
const string BarProvider::_sDTOHLCV = "DTOHLCV";

BarProvider::BarProvider(
		const char *ccPath,
		const string &sFormat, 
		int iRndDecimalPlc, 
		char cReverseInputOrder, 
		char cHeader,
		char cCompressionType)
	:
		_bMaster(true),
		_viYMD(NULL),
		_viTime(NULL),
		_vdOpen(NULL),
		_vdHigh(NULL),
		_vdLow(NULL),
		_vdClose(NULL),
		_vdReturn(NULL),
		_viVolume(NULL)
{
	LoadDataFile(
			ccPath,
			sFormat,
			iRndDecimalPlc,
			cReverseInputOrder,
			cHeader,
			cCompressionType);
}

BarProvider::BarProvider(const BarProvider & rhs)
{
	_bMaster = false;
	_iPsn = rhs._iPsn;
	_iNumOfBars = rhs._iNumOfBars;
	_cFwdBkwd = rhs._cFwdBkwd;
	_bTimeFmt = rhs._bTimeFmt;

	_viYMD = rhs._viYMD;
	_viTime = rhs._viTime;
	_vdOpen = rhs._vdOpen;
	_vdHigh = rhs._vdHigh;
	_vdLow = rhs._vdLow;
	_vdClose = rhs._vdClose;
	_vdReturn = rhs._vdReturn;
	_viVolume = rhs._viVolume;
}

void BarProvider::FreeVec() {
	if (_bMaster)
	{
		if (_viYMD != NULL)
		{
			for (int i = 0; i < _viYMD.size(); ++i) delete _viYMD[i];
			delete _viYMD; _viYMD = NULL;
		}
		if (_viTime != NULL)
		{
			for (int i = 0; i < _viTime.size(); ++i) delete _viTime[i];
			delete _viTime; _viTime = NULL;
		}
		if (_vdOpen != NULL)  { delete _vdOpen; _vdOpen = NULL; }
		if (_vdHigh != NULL)  { delete _vdHigh; _vdHigh = NULL; }
		if (_vdLow != NULL)  { delete _vdLow; _vdLow = NULL; }
		if (_vdClose != NULL)  { delete _vdClose; _vdClose = NULL; }
		if (_vdReturn != NULL)  { delete _vdReturn; _vdReturn = NULL; }
		if (_viVolume != NULL)  { delete _viVolume; _viVolume = NULL; }
	}
}
/*
	 BarProvider & operator=(const BarProvider &rhs)
 */

BarProvider::~BarProvider() {
	FreeVec();
}

// Compression Type: 'G' = gzip, 'P' = plain
// Format: "DOHLCV" for Date,O,H,L,C,V
// Forward / Backward: 'F' forward 'B' backward
// Header: 'H' has header, 'N' has no header
void BarProvider::LoadDataFile(
		const char *ccPath,
		const string &sFormat,
		int iRndDecimalPlc,
		char cFwdBkwd, 
		char cHeader,
		char cCompressionType)
{
	// Won't call LoadDataFile() very often, can afford to new vectors here
	int iReserveSize = 60*8*22*12*10;
	if (_viYMD == NULL) { _viYMD = new vector<YYYYMMDD>(); _viYMD->reserve(iReserveSize); }
	if (_viTime == NULL) { _viTime = new vector<HHMMSS>(); _viTime->reserve(iReserveSize); }
	if (_vdOpen == NULL) { _vdOpen = new vector<double>(); _vdOpen->reserve(iReserveSize); }
	if (_vdHigh == NULL) { _vdHigh = new vector<double>(); _vdHigh->reserve(iReserveSize); }
	if (_vdLow == NULL) { _vdLow = new vector<double>(); _vdLow->reserve(iReserveSize); }
	if (_vdClose == NULL) { _vdClose = new vector<double>(); _vdClose->reserve(iReserveSize); }
	if (_vdReturn == NULL) { _vdReturn = new vector<double>(); _vdReturn->reserve(iReserveSize); }
	if (_viVolume == NULL) { _viVolume = new vector<int>(); _viVolume->reserve(iReserveSize); }

	if (
			(sFormat.compare(BarProvider::_sDOHLCV) != 0 && sFormat.compare(BarProvider::_sDTOHLCV) != 0) ||
			(cCompressionType != 'G' && cCompressionType != 'P') ||
			(cFwdBkwd != 'F' && cFwdBkwd != 'B') ||
			(cHeader != 'H' && cHeader != 'N'))
	{
		FreeVec();
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}

	_cFwdBkwd = cFwdBkwd;
	_bTimeFmt = false; // Initially making the assumption HM

	ifstream ifs(ccPath, std::ios_base::in | std::ios_base::binary);

	try {
		boost::iostreams::filtering_istream fis;


		if (cCompressionType == 'G') fis.push(boost::iostreams::gzip_decompressor());
		fis.push(ifs);

		int iLineNum = 0;


		for (string str; getline(fis, str); )
		{
			++iLineNum;
			if (cHeader == 'H' && iLineNum == 1) continue; // Skip header row

			string sDate;
			string sTime="";
			double dOHLCV[5];
			Tokenizer tknzr(str, ",");

			if (tknzr.NextToken()) sDate = tknzr.GetToken();
			if (sFormat.compare(BarProvider::_sDTOHLCV) == 0 && tknzr.NextToken()) sTime = tknzr.GetToken();

			int iCnt = 0;
			while (tknzr.NextToken() && iCnt < sizeof(dOHLCV)/sizeof(double))
			{
				dOHLCV[iCnt] = STool::ToDbl((string)(tknzr.GetToken()), iRndDecimalPlc);
				++iCnt;
			}

			// If the data is in reverse order, then of course the vectors will contain reverse order data
			_viYMD->push_back(new YYYYMMDD(sDate));
			if (sFormat.compare(BarProvider::_sDTOHLCV) == 0)
			{
				int iHMSTmp = STool::HMSi(sTime);
				if (!_bTimeFmt && iHMSTmp > 9999) _bTimeFmt = true;
				_viTime->push_back(new HHMMSS(sTime));
			}
			else
			{
				_viTime->push_back(0);
			}

			if (!_vdClose->empty())
			{
				if (_cFwdBkwd == 'F')
					_vdReturn->push_back(dOHLCV[3] / _vdClose->back());
				else if (_cFwdBkwd == 'B')
					_vdReturn->push_back(_vdClose->back() / dOHLCV[3]);
			}
			else
			{
				if (_cFwdBkwd == 'F') _vdReturn->push_back(0);
			}

			_vdOpen->push_back(dOHLCV[0]);
			_vdHigh->push_back(dOHLCV[1]);
			_vdLow->push_back(dOHLCV[2]);
			_vdClose->push_back(dOHLCV[3]);
			_viVolume->push_back(dOHLCV[4]);

		}

		if (_cFwdBkwd == 'B') _vdReturn->push_back(0);

		if (_viYMD->size() != _viTime->size() ||
				_viYMD->size() != _vdOpen->size() ||
				_viYMD->size() != _vdHigh->size() ||
				_viYMD->size() != _vdLow->size() ||
				_viYMD->size() != _vdClose->size() ||
				_viYMD->size() != _vdReturn->size() ||
				_viYMD->size() != _viVolume->size() )
		{
			FreeVec();
			SException se;
			se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
			throw se;
		}
		else _iNumOfBars = _viYMD->size();

		if (cFwdBkwd == 'F') _iPsn = -1;
		else _iPsn = _iNumOfBars;

		if (iLineNum == 0)
		{
			FreeVec();
			SException se;
			se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
			throw se;
		}
	}
	catch(const boost::iostreams::gzip_error& e)
	{
		cout << e.what() << endl;
		{
			FreeVec();
			SException se;
			se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
			throw se;
		}
	}

}

void BarProvider::ResetPsn()
{
	if (_cFwdBkwd == 'F') _iPsn = -1;
	else _iPsn = _iNumOfBars;
}

int BarProvider::GetNumOfBars() const
{
	return _iNumOfBars;
}

bool BarProvider::HasNextBar() const
{
	if (_cFwdBkwd == 'F')
	{
		if (_iPsn < _iNumOfBars-1) return true;
		else return false;
	}
	else
	{
		if (_iPsn > 0) return true;
		else return false;
	}
}

void BarProvider::MoveNext()
{
	if (_cFwdBkwd == 'F') ++_iPsn; else --_iPsn;

	// We expect that HasNextBar() is called before MoveNext()
	if (_cFwdBkwd == 'F' && _iPsn >= _iNumOfBars || _cFwdBkwd == 'B' && _iPsn < 0)
	{
		FreeVec();
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
}

//returns -1 if there is no more bars
const int BarProvider::PeekNextYMDi() const
{
	if (HasNextBar())
	{	
		if (_cFwdBkwd == 'F') return ((*_viYMD)[_iPsn+1]);
		else return ((*_viYMD)[_iPsn-1]);
	}
	else return -1;
}

const int BarProvider::PeekPrevYMDi()
{
	if (_cFwdBkwd == 'F' && _iPsn > 0) return (*_viYMD)[_iPsn-1];
	else if (_cFwdBkwd == 'B' && _iPsn < _iNumOfBars-1) return (*_viYMD)[_iPsn+1];
	else
	{
		FreeVec();
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
}

const double BarProvider::PeekPrevClose()
{
	if (_cFwdBkwd == 'F' && _iPsn > 0) return (*_vdClose)[_iPsn-1];
	else if (_cFwdBkwd == 'B' && _iPsn < _iNumOfBars-1) return (*_vdClose)[_iPsn+1];
	else
	{
		FreeVec();
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
}

const double * BarProvider::GetCloseArr(int iLen)
{
	if (iLen > 0)
	{
		if (_cFwdBkwd == 'F' && _iPsn >= iLen-1)
			return &((*_vdClose)[_iPsn-iLen+1]);
		else if (_cFwdBkwd == 'B')
			return &((*_vdClose)[_iPsn]);
	}
	else
	{
		FreeVec();
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
}


const double * BarProvider::GetReturnArr(int iLen)
{
	if (iLen > 0)
	{
		if (_cFwdBkwd == 'F' && _iPsn >= iLen-1)
			return &((*_vdReturn)[_iPsn-iLen+1]);
		else if (_cFwdBkwd == 'B')
			return &((*_vdReturn)[_iPsn]);
	}
	else
	{
		FreeVec();
		SException se;
		se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
		throw se;
	}
}




const int BarProvider::YMDi() const { return (*_viYMD)[_iPsn]; }
const int BarProvider::HMSi() const
{
	if (_bTimeFmt) return (*_viTime)[_iPsn];
	else return (*_viTime)[_iPsn] * 100;
}
const int BarProvider::HMi() const
{
	if (_bTimeFmt) return (*_viTime)[_iPsn] / 100; // Simply truncate the number of second
	else return (*_viTime)[_iPsn];
}
const double BarProvider::Open() const { return (*_vdOpen)[_iPsn]; }
const double BarProvider::High() const { return (*_vdHigh)[_iPsn]; }
const double BarProvider::Low() const { return (*_vdLow)[_iPsn]; }
const double BarProvider::Close() const { return (*_vdClose)[_iPsn]; }
const double BarProvider::Return() const { return (*_vdReturn)[_iPsn]; }
const int BarProvider::Volume() const { return (*_viVolume)[_iPsn]; }


#endif
