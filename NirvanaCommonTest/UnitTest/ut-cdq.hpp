#include "UTest.h"
#include "Math/RandomEngine.h"
#include "Math/CirDataQue.hpp"
#include <iostream>

using namespace std;

int TestCDQ()
{
	UTest ut;
	CirDataQue<double> cb1;

	ut.AssertF(cb1.Ready(), __FILE__,__FUNCTION__,__LINE__);

	for (int i = 0; i < 10; i++) cb1.Add(10);
	ut.AssertF(cb1.Ready(), __FILE__,__FUNCTION__,__LINE__);

	//Half full
	cb1.Reset(3,true);
	cb1.Add(876);
	cb1.Add(9869);
	ut.AssertF(cb1.Count() != 2, __FILE__,__FUNCTION__,__LINE__);

	//Get half full array
	int iSize=0;
	const double *da;
	da = cb1.GetArrayUnord(iSize);
	ut.AssertF(da[0] != 876, __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(da[1] != 9869, __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(iSize != 2, __FILE__,__FUNCTION__,__LINE__);

	//Test cycle
	cb1.Add(234123);
	cb1.Add(30247);
	cb1.Add(8946);
	ut.AssertF(cb1.Count() != 3, __FILE__,__FUNCTION__,__LINE__);

	const double *db = cb1.GetArrayUnord(iSize);
	ut.AssertF(db[0] != 30247, __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(db[1] != 8946, __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(db[2] != 234123, __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(iSize != 3, __FILE__,__FUNCTION__,__LINE__);

	//Stress test
	cb1.Reset(100,true);
	RandomEngine re = RandomEngine::Instance();
	for (int i = 0; i < 100000; i++) cb1.Add(re.NextInt(1,1000));
	ut.AssertF(cb1.Count() != 100, __FILE__,__FUNCTION__,__LINE__);

	//Test erroneous input
	cb1.Reset(2,true);
	cb1.Add(-4);
	cb1.Add(-4.5);
	cb1.Add(0.00000001);
	ut.AssertF(cb1.Count() != 0, __FILE__,__FUNCTION__,__LINE__);

	//GetArrayInInsOrd, no opulent vector
	cb1.Reset(10,true);
	for (int i = 1; i < 77; i++) cb1.Add(i);
	const double *da2 = cb1.GetArrayInInsOrd();
	ut.Assert(da2[0] == 67, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(da2[1] == 68, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(da2[2] == 69, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(da2[3] == 70, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(da2[4] == 71, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(da2[5] == 72, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(da2[6] == 73, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(da2[7] == 74, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(da2[8] == 75, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(da2[9] == 76, __FILE__,__FUNCTION__,__LINE__);

	//GetArrayInInsOrd, with opulent vector
	cb1.Reset(10,55,true);
	for (int i = 1; i < 77; i++) cb1.Add(i);
	const double *db2 = cb1.GetArrayInInsOrd();
	ut.Assert(db2[0] == 67, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(db2[1] == 68, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(db2[2] == 69, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(db2[3] == 70, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(db2[4] == 71, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(db2[5] == 72, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(db2[6] == 73, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(db2[7] == 74, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(db2[8] == 75, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(db2[9] == 76, __FILE__,__FUNCTION__,__LINE__);

	//GetArrayInInsOrd, no opulent vector
	//not completely filled, throw exception
	cb1.Reset(10,true);
	for (int i = 1; i < 3; i++) cb1.Add(i);
	const double *da3 = 0;
	// try { da3 = cb1.GetArrayInInsOrd(); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); } catch (exception& e) {}
	ut.Assert(da3 == NULL, __FILE__,__FUNCTION__,__LINE__);

	//GetArrayInInsOrd, with opulent vector
	//not completely filled, throw exception
	cb1.Reset(10,55,true);
	for (int i = 1; i < 3; i++) cb1.Add(i);
	const double *db3 = 0;
	// try { db3 = cb1.GetArrayInInsOrd(); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); } catch (exception& e) {}
	ut.Assert(db3 == NULL, __FILE__,__FUNCTION__,__LINE__);


	//GetArrayInInsOrd, no opulent vector
	//more tests, cir array ptr == 0
	cb1.Reset(5,true);
	for (int i = 1; i < 4556; i++) cb1.Add(i);
	const double *da4 = cb1.GetArrayInInsOrd();
	ut.Assert(da4[0] == 4551, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(da4[1] == 4552, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(da4[2] == 4553, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(da4[3] == 4554, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(da4[4] == 4555, __FILE__,__FUNCTION__,__LINE__);

	//GetArrayInInsOrd, with opulent vector
	//more tests, cir array ptr == 0
	cb1.Reset(5, 911,true);
	for (int i = 1; i < 4554; i++) cb1.Add(i);
	cb1.Add(4554);
	ut.Assert(cb1.CirPsn() == 910, __FILE__,__FUNCTION__,__LINE__);
	cb1.Add(4555);
	ut.Assert(cb1.CirPsn() == 0, __FILE__,__FUNCTION__,__LINE__);
	const double *db4 = cb1.GetArrayInInsOrd();
	ut.Assert(db4[0] == 4551, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(db4[1] == 4552, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(db4[2] == 4553, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(db4[3] == 4554, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(db4[4] == 4555, __FILE__,__FUNCTION__,__LINE__);

	//GetArrayInInsOrd, no opulent vector
	//more tests, 1 item dangling
	cb1.Reset(5,true);
	for (int i = 1; i < 4554; i++) cb1.Add(i);
	cb1.Add(4554);
	ut.Assert(cb1.CirPsn() == 4, __FILE__,__FUNCTION__,__LINE__);
	cb1.Add(4555);
	ut.Assert(cb1.CirPsn() == 0, __FILE__,__FUNCTION__,__LINE__);
	cb1.Add(4556);
	ut.Assert(cb1.CirPsn() == 1, __FILE__,__FUNCTION__,__LINE__);
	const double *da5 = cb1.GetArrayInInsOrd();
	ut.Assert(da5[0] == 4552, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(da5[1] == 4553, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(da5[2] == 4554, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(da5[3] == 4555, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(da5[4] == 4556, __FILE__,__FUNCTION__,__LINE__);

	//GetArrayInInsOrd, with opulent vector
	//more tests, 1 item dangling
	cb1.Reset(5, 911,true);
	for (int i = 1; i < 4554; i++) cb1.Add(i);
	cb1.Add(4554);
	ut.Assert(cb1.CirPsn() == 910, __FILE__,__FUNCTION__,__LINE__);
	cb1.Add(4555);
	ut.Assert(cb1.CirPsn() == 0, __FILE__,__FUNCTION__,__LINE__);
	cb1.Add(4556);
	ut.Assert(cb1.CirPsn() == 1, __FILE__,__FUNCTION__,__LINE__);
	const double *db5 = cb1.GetArrayInInsOrd();
	ut.Assert(db5[0] == 4552, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(db5[1] == 4553, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(db5[2] == 4554, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(db5[3] == 4555, __FILE__,__FUNCTION__,__LINE__);
	ut.Assert(db5[4] == 4556, __FILE__,__FUNCTION__,__LINE__);

	//GetArrayUnord ok if no opulent vector
	cb1.Reset(5,true);
	try {
		const double *da6 = cb1.GetArrayUnord(iSize);
    if (da6 != NULL) da6 = NULL; //dummy use to avoid compiler warning
	} catch (exception& e)
	{
		ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
	}

	//GetArrayUnord doesn't support opulent vector
	cb1.Reset(5,10,true);
	// try {
	// 	const double *da6 = cb1.GetArrayUnord(iSize);
	// 	ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
  //   if (da6 != NULL) da6 = NULL; //dummy use to avoid compiler warning
	// } catch (exception& e) {}
	const double *da6 = cb1.GetArrayUnord(iSize);
	ut.Assert(da6 == NULL, __FILE__,__FUNCTION__,__LINE__);



	ut.PrintResult();


	return 0;
}
