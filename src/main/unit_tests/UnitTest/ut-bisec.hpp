#include "UTest.h"
#include "Math/Bisection.h"

int TestBisec()
{
	UTest ut;

	Bisection b;
	double dM;

	dM = b.GetFirst(0,100,0.1);
	ut.Assert(dM == 50, __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(b.IsEnough(), __FILE__,__FUNCTION__,__LINE__);

	dM = b.GetNext(false);
	ut.Assert(dM == 75, __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(b.IsEnough(), __FILE__,__FUNCTION__,__LINE__);

	dM = b.GetNext(false);
	ut.Assert(dM == 87.5, __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(b.IsEnough(), __FILE__,__FUNCTION__,__LINE__);

	dM = b.GetNext(true);
	ut.Assert(dM == 81.25, __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(b.IsEnough(), __FILE__,__FUNCTION__,__LINE__);

	dM = b.GetNext(true);
	ut.Assert(dM == 78.125, __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(b.IsEnough(), __FILE__,__FUNCTION__,__LINE__);

	dM = b.GetNext(true);
	ut.Assert(dM == 76.5625, __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(b.IsEnough(), __FILE__,__FUNCTION__,__LINE__);

	dM = b.GetNext(false);
	ut.Assert(dM == 77.34375, __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(b.IsEnough(), __FILE__,__FUNCTION__,__LINE__);


	for (int i = 0; i < 2; ++i)
	{
		dM = b.GetNext(false);
		ut.AssertF(b.IsEnough(), __FILE__,__FUNCTION__,__LINE__);
	}

	dM = b.GetNext(false);
	ut.Assert(b.IsEnough(), __FILE__,__FUNCTION__,__LINE__);



	Bisection b2;
	try
	{
		dM = b2.GetNext(false);
		ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	} catch (std::exception &e) {}

	Bisection b3;
	try
	{
		dM = b3.GetFirst(0,100,0.1);
		dM = b3.GetFirst(0,100,0.1);
		ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	} catch (std::exception &e) {}




	ut.PrintResult();

	return 0;
}
