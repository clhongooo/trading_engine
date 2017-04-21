#include "UTest.h"
#include "Math/HighLow.hpp"
#include <iostream>

int TestHL()
{
	UTest ut;

	HighLow<double> hl(3,true);

	try { hl.HighestHigh(); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); } catch (exception& e) {}
	try { hl.LowestHigh(); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); } catch (exception& e) {}
	try { hl.HighestLow(); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); } catch (exception& e) {}
	try { hl.LowestLow(); ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__); } catch (exception& e) {}
	ut.AssertF(hl.Ready(), __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(hl.Count() != 0, __FILE__,__FUNCTION__,__LINE__);


	hl.Add(80,68);
	ut.AssertF(hl.Ready(), __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.Count() != 1, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.HighestHigh() != 80, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.HighestLow() != 68, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.LowestLow() != 68, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.LowestHigh() != 80, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.Height() != 12, __FILE__,__FUNCTION__,__LINE__);;

	hl.Add(91,43);
	ut.AssertF(hl.Ready(), __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.Count() != 2, __FILE__,__FUNCTION__,__LINE__);;

	hl.Add(20,79);
	ut.AssertF(!hl.Ready(), __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.Count() != 3, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.HighestHigh() != 91, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.HighestLow() != 68, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.LowestLow() != 20, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.LowestHigh() != 79, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.Height() != 71, __FILE__,__FUNCTION__,__LINE__);;

	hl.Add(21,41);
	ut.AssertF(!hl.Ready(), __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.Count() != 3, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.HighestHigh() != 91, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.HighestLow() != 43, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.LowestLow() != 20, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.LowestHigh() != 41, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.Height() != 71, __FILE__,__FUNCTION__,__LINE__);;

	hl.Add(44,17);
	ut.AssertF(!hl.Ready(), __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.Count() != 3, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.HighestHigh() != 79, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.HighestLow() != 21, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.LowestLow() != 17, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.LowestHigh() != 41, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.Height() != 62, __FILE__,__FUNCTION__,__LINE__);;

	hl.Add(32,96);
	ut.AssertF(!hl.Ready(), __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.Count() != 3, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.HighestHigh() != 96, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.HighestLow() != 32, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.LowestLow() != 17, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.LowestHigh() != 41, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.Height() != 79, __FILE__,__FUNCTION__,__LINE__);;

	hl.Add(82,63);
	ut.AssertF(!hl.Ready(), __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.Count() != 3, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.HighestHigh() != 96, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.HighestLow() != 63, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.LowestLow() != 17, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.LowestHigh() != 44, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.Height() != 79, __FILE__,__FUNCTION__,__LINE__);;

	hl.Add(34,65);
	ut.AssertF(!hl.Ready(), __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.Count() != 3, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.HighestHigh() != 96, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.HighestLow() != 63, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.LowestLow() != 32, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.LowestHigh() != 65, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.Height() != 64, __FILE__,__FUNCTION__,__LINE__);;

	hl.Add(21,29);
	ut.AssertF(!hl.Ready(), __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.Count() != 3, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.HighestHigh() != 82, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.HighestLow() != 63, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.LowestLow() != 21, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.LowestHigh() != 29, __FILE__,__FUNCTION__,__LINE__);;
	ut.AssertF(hl.Height() != 61, __FILE__,__FUNCTION__,__LINE__);;

	hl.Reset(5,true);
	ut.AssertF(hl.Count() != 0, __FILE__,__FUNCTION__,__LINE__);;
	hl.Add(63,82);
	hl.Reset(10,true);
	ut.AssertF(hl.Count() != 0, __FILE__,__FUNCTION__,__LINE__);;


	ut.PrintResult();


	return 0;
}
