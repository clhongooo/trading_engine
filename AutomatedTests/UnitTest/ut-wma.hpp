#include "UTest.h"
#include "Math/Wma.hpp"
#include <iostream>
#include <cmath>
#include <vector>

int TestWMA()
{
	UTest ut;
	Wma<double> w;
	vector<int> vParamVec;
	vParamVec.push_back(4);
	vParamVec.push_back(3);
	vParamVec.push_back(1);
	w.Reset(vParamVec,true);

	ut.AssertF(w.Ready(), __FILE__,__FUNCTION__,__LINE__);
	try
	{
		w.Value();
		ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	} catch (std::exception& ex) {}

	//should not enter ema at all
	w.Add(0.0000001);
	w.Add(0.0000001);
	w.Add(0.0000001);
	w.Add(0.0000001);
	w.Add(0.0000001);
	w.Add(0.0000001);
	ut.AssertF(w.Ready(), __FILE__,__FUNCTION__,__LINE__);


	w.Add(14963);
	ut.AssertF(w.Ready(), __FILE__,__FUNCTION__,__LINE__);
	w.Add(14637);
	ut.AssertF(w.Ready(), __FILE__,__FUNCTION__,__LINE__);
	w.Add(15182);
	ut.Assert(w.Ready(), __FILE__,__FUNCTION__,__LINE__);
	w.Add(15399);
	ut.Assert(w.Ready(), __FILE__,__FUNCTION__,__LINE__);
	w.Add(15358);
	ut.Assert(w.Ready(), __FILE__,__FUNCTION__,__LINE__);
	w.Add(15461);
	ut.Assert(w.Ready(), __FILE__,__FUNCTION__,__LINE__);

	w.Add(15429);
	ut.Assert(w.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(w.Value() - 15432) < 1, __FILE__,__FUNCTION__,__LINE__);

	w.Add(15244);
	ut.Assert(w.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(abs(w.Value() - 15340) < 1, __FILE__,__FUNCTION__,__LINE__);

	vParamVec.clear();
	vParamVec.push_back(6);
	vParamVec.push_back(4);
	vParamVec.push_back(3);
	vParamVec.push_back(1);
	w.Reset(vParamVec,true);
	ut.AssertF(w.Ready(), __FILE__,__FUNCTION__,__LINE__);

	try
	{
		vParamVec.clear();
		w.Reset(vParamVec,true);
		ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	} catch (std::exception& ex) {}



	ut.PrintResult();


	return 0;
}
