#include "UTest.h"
#include "Message/BarMsg.h"
#include "Message/SockMsg.h"
#include "Util/CustType.h"
#include <iostream>
#include <cmath>

using namespace std;

int TestBM()
{
	UTest ut;

	{
		SockMsg sm("0,TestSym,2013-03-21,12:45,9.33,12.99,8.4,8.9,8768553");
		BarMsg bm(sm);
		ut.Assert(bm.GetSockMsgType() == BAR, __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(bm.Symbol() == "TestSym", __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(bm.GetYYYYMMDD() == YYYYMMDD(20130321), __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(bm.GetHHMMSS() == HHMMSS(1245), __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(bm.Open() == 9.33, __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(bm.High() == 12.99, __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(bm.Low() == 8.4, __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(bm.Close() == 8.9, __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(bm.Volume() == 8768553, __FILE__,__FUNCTION__,__LINE__);
	}


	{
		SockMsg sm("0,TestSym2,2013-03-21,0:0,9.33,12.99,8.4,8.9,0");
		BarMsg bm(sm);
		ut.Assert(bm.GetSockMsgType() == BAR, __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(bm.Symbol() == "TestSym2", __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(bm.GetYYYYMMDD() == YYYYMMDD(20130321), __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(bm.GetHHMMSS() == HHMMSS(0), __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(bm.Open() == 9.33, __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(bm.High() == 12.99, __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(bm.Low() == 8.4, __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(bm.Close() == 8.9, __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(bm.Volume() == 0, __FILE__,__FUNCTION__,__LINE__);
	}


	{
		BarMsg bm2;
		bm2.SetSockMsgType(BAR);
		bm2.SetSymbol("TestSym3");
		bm2.SetYYYYMMDD(YYYYMMDD(20140103));
		bm2.SetHHMMSS(HHMMSS(915));
		bm2.SetOpen(70.1);
		bm2.SetHigh(70.3);
		bm2.SetLow(66.7);
		bm2.SetClose(69.1);
		bm2.SetVolume(3423005);
		cout << bm2.GetCsv() << endl;
		ut.Assert(bm2.GetCsv() == "0,TestSym3,2014-01-03,09:15:00,70.1,70.3,66.7,69.1,3423005", __FILE__,__FUNCTION__,__LINE__);

	}




	ut.PrintResult();

	return 0;
}
