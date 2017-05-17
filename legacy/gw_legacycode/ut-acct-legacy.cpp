#include "UTest.hpp"
#include "../smath/BlackScholes.hpp"
#include "../corelib/IParam.hpp"
#include "../corelib/STool.hpp"
#include "../corelib/HKFE.hpp"
#include "../corelib/Acct.hpp"
#include <iostream>
#include <string>

int main()
{
	UTest ut;

	Acct acct;

	//TradeFut
	acct.TradeFut(true, 1, 20101);
	ut.Assert(acct.FutPos() == 1,__LINE__);
	ut.Assert(acct.AggFutPx() == 20101+IParam::TRDG_FRICT_FUT_SLIP+IParam::TRDG_FRICT_CMSN,__LINE__);
	acct.TradeFut(false, 7, 20201);
	ut.Assert(acct.FutPos() == -6,__LINE__);
	acct.TradeFut(true, 6, 20001);
	acct.MarkToMarket(20302, 20130102, 0.14, 1);
	double dCPnL = 1300-IParam::TRDG_FRICT_FUT_SLIP*14-IParam::TRDG_FRICT_CMSN*14;
	ut.Assert(acct.CPnL() == dCPnL,__LINE__);

	ut.Assert(acct.FutPos() == 0,__LINE__);

	//ResetCPnL
	acct.ResetCPnL();
	ut.Assert(acct.CPnL() == 0,__LINE__);


	//TradeOpt
	acct.TradeOpt(false, "C-23400-2012-01", 20120103, 1, 22631, 0.15, "FUCK");
	acct.TradeOpt(false, "C-23400-2012-01", 20120103, 1, 22631, 0.15, "FUCK");
	ut.Assert(acct.AggCallPos() == -2,__LINE__);
	ut.Assert(acct.AggPutPos() == 0,__LINE__);
	ut.Assert(acct.AbsCallPos() == 2,__LINE__);
	ut.Assert(acct.AbsPutPos() == 0,__LINE__);

	acct.TradeOpt(true, "P-20400-2012-02", 20120103, 2, 22631, 0.16, "SHIT");
	ut.Assert(acct.AggCallPos() == -2,__LINE__);
	ut.Assert(acct.AggPutPos() == 2,__LINE__);
	ut.Assert(acct.AbsCallPos() == 2,__LINE__);
	ut.Assert(acct.AbsPutPos() == 2,__LINE__);

	//MarketToMarket
	//acct.MarkToMarket(22631, 20120103, 0.14, 1);
	//ut.Assert(acct.CPnL() > 0,__LINE__);

	//Expired options should be removed


	//Short option positions below a certain price should be removed
	acct.MarkToMarket(18631, 20120129, 0.13, 1);
	ut.Assert(acct.AggCallPos() == 0,__LINE__);
	ut.Assert(acct.AbsCallPos() == 0,__LINE__);
	ut.Assert(acct.AggPutPos() == 2,__LINE__);
	ut.Assert(acct.AbsPutPos() == 2,__LINE__);




	ut.PrintResult();


	return 0;
}
