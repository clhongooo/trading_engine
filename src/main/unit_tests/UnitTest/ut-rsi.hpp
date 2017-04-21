#include "UTest.h"
#include "Math/Wr.hpp"
#include <iostream>
#include <cmath>

using namespace std;

// Ans: 6 50.069
// Ans: 12 52.968


int TestWR()
{
	UTest ut;
	Wr<double> w;
	w.Reset(3,true);

	ut.AssertF(w.Ready(), __FILE__,__FUNCTION__,__LINE__);
	try
	{
		w.Value();
		ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	} catch (std::exception& e) {}

	w.Add(991, 322, 534);
	ut.AssertF(w.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(abs(w.Value() - -68.3109) > 0.1, __FILE__,__FUNCTION__,__LINE__);

	w.Add(784, 81, 784);
	ut.AssertF(w.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(abs(w.Value() - -22.7473) > 0.1, __FILE__,__FUNCTION__,__LINE__);

	w.Add(569, 90, 444);
	ut.AssertF(!w.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(abs(w.Value() - -60.1099) > 0.1, __FILE__,__FUNCTION__,__LINE__);

	w.Add(520, 82, 82);
	ut.AssertF(!w.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(abs(w.Value() - -99.8578) > 0.1, __FILE__,__FUNCTION__,__LINE__);

	w.Add(699, 291, 431);
	ut.AssertF(!w.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(abs(w.Value() - -43.436) > 0.1, __FILE__,__FUNCTION__,__LINE__);

	w.Add(910, 114, 114);
	ut.AssertF(!w.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(abs(w.Value() - -96.1353) > 0.1, __FILE__,__FUNCTION__,__LINE__);

	w.Add(967, 809, 967);
	ut.AssertF(!w.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(abs(w.Value() - 0) > 0.1, __FILE__,__FUNCTION__,__LINE__);

	w.Add(501, 129, 185);
	ut.AssertF(!w.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(abs(w.Value() - -91.6764) > 0.1, __FILE__,__FUNCTION__,__LINE__);

	w.Add(549, 242, 549);
	ut.AssertF(!w.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(abs(w.Value() - -49.8807) > 0.1, __FILE__,__FUNCTION__,__LINE__);





	w.Reset(10,true);
	ut.AssertF(w.Ready(), __FILE__,__FUNCTION__,__LINE__);


	ut.PrintResult();


	return 0;
}
