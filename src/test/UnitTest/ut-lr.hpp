#include "UTest.h"
#include "Math/LinRegr.h"
#include <iostream>
#include <cmath>

using namespace std;

int TestLR()
{
	UTest ut;
	LinRegr lr;
	lr.Reset(8,true);

	ut.AssertF(lr.Ready(), __FILE__,__FUNCTION__,__LINE__);
	try
	{
		lr.Value();
		ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	} catch (std::exception& e) {}

	lr.AddYX(166, 368);
	ut.AssertF(lr.Ready(), __FILE__,__FUNCTION__,__LINE__);
	lr.AddYX(811, 878);
	lr.AddYX(983, 662);
	lr.AddYX(534, 54);
	lr.AddYX(113, 117);
	lr.AddYX(171, 476);
	lr.AddYX(871, 622);
	ut.AssertF(lr.Ready(), __FILE__,__FUNCTION__,__LINE__);
	lr.AddYX(907, 448);
	ut.Assert(lr.Ready(), __FILE__,__FUNCTION__,__LINE__);


	ut.Assert(abs(lr.Value() - 0.809031) < 0.01, __FILE__,__FUNCTION__,__LINE__);




	lr.Reset(10,true);
	ut.AssertF(lr.Ready(), __FILE__,__FUNCTION__,__LINE__);


	ut.PrintResult();


	return 0;
}

