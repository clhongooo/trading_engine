#include "UTest.h"
#include "Message/BarMsg.h"
#include "Util/CustType.h"
#include <iostream>
#include <cmath>

using namespace std;

int TestSM()
{
	UTest ut;

	SockMsg sm;
	ut.Assert(sm.GetSockMsgType() == UNDEFSOCKMSG, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(sm.GetCsv() == "", __FILE__,__FUNCTION__,__LINE__);

	sm.SetCsv("0,TestSym,2013-03-21,12:45,9.33,12.99,8.4,8.9,8768553");
	ut.Assert(sm.GetSockMsgType() == BAR, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(sm.GetCsv() == "0,TestSym,2013-03-21,12:45,9.33,12.99,8.4,8.9,8768553", __FILE__,__FUNCTION__,__LINE__);


	SockMsg sm1("0,TestSym,2013-03-21,12:45,9.33,12.99,8.4,8.9,8768553");
	ut.Assert(sm.GetSockMsgType() == BAR, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(sm.GetCsv() == "0,TestSym,2013-03-21,12:45,9.33,12.99,8.4,8.9,8768553", __FILE__,__FUNCTION__,__LINE__);


	ut.PrintResult();

	return 0;
}
