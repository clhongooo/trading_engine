#include "UTest.h"
#include "Util/SLowerBound.h"
#include <deque>
#include <iostream>

int TestSLB()
{
	UTest ut;
	bool b;
	deque<double> dq;

	dq.push_back(1);
	dq.push_back(3);
	dq.push_back(5);
	dq.push_back(7);
	dq.push_back(9);

	deque<double>::iterator it;


	b = SLowerBound<deque<double>::iterator, double >::ExactMatch(dq.begin(), dq.end(), 2, it);
	ut.AssertF(b, __FILE__,__FUNCTION__,__LINE__);

	b = SLowerBound<deque<double>::iterator, double >::ExactMatch(dq.begin(), dq.end(), 3, it);
	ut.AssertF(!b, __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(*it != 3, __FILE__,__FUNCTION__,__LINE__);

	b = SLowerBound<deque<double>::iterator, double >::ExactMatch(dq.begin(), dq.end(), 10, it);
	ut.AssertF(b, __FILE__,__FUNCTION__,__LINE__);



	b = SLowerBound<deque<double>::iterator, double >::JustLarger(dq.begin(), dq.end(), 0, it);
	ut.AssertF(!b, __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(*it != 1, __FILE__,__FUNCTION__,__LINE__);

	b = SLowerBound<deque<double>::iterator, double >::JustLarger(dq.begin(), dq.end(), 1, it);
	ut.AssertF(!b, __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(*it != 1, __FILE__,__FUNCTION__,__LINE__);

	b = SLowerBound<deque<double>::iterator, double >::JustLarger(dq.begin(), dq.end(), 2, it);
	ut.AssertF(!b, __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(*it != 3, __FILE__,__FUNCTION__,__LINE__);

	b = SLowerBound<deque<double>::iterator, double >::JustLarger(dq.begin(), dq.end(), 3, it);
	ut.AssertF(!b, __FILE__,__FUNCTION__,__LINE__);
	ut.AssertF(*it != 3, __FILE__,__FUNCTION__,__LINE__);

	b = SLowerBound<deque<double>::iterator, double >::JustLarger(dq.begin(), dq.end(), 10, it);
	ut.AssertF(b, __FILE__,__FUNCTION__,__LINE__);


	ut.PrintResult();


	return 0;
}
