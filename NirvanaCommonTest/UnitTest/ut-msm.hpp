#include "UTest.h"
#include "Message/MktStatusMsg.h"
#include "Message/SockMsg.h"
#include "Util/CustType.h"
#include <iostream>
#include <cmath>

using namespace std;

int TestMSM()
{
	UTest ut;

	{
		SockMsg sm("1,99");
		MktStatusMsg msm(sm);
		ut.Assert(msm.GetSockMsgType() == MKTSTATUS, __FILE__,__FUNCTION__,__LINE__);
		ut.Assert(msm.GetMktStatus() == UNDEFMKTSTS, __FILE__,__FUNCTION__,__LINE__);
	}

	{
		MktStatusMsg msm2;
		msm2.SetSockMsgType(MKTSTATUS);
		msm2.SetMktStatus(AFT_CLOSE);
		ut.Assert(msm2.GetCsv() == "1,9", __FILE__,__FUNCTION__,__LINE__);

	}




	ut.PrintResult();

	return 0;
}
