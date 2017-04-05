#ifndef TESTLEASTSQREGRESSION_H_
#define TESTLEASTSQREGRESSION_H_

#include <cmath>
#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "LeastSqRegression.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestLeastSqRegression : public CppUnit::TestFixture
{
  public:
    TestLeastSqRegression();
    virtual ~TestLeastSqRegression();
    double GetY_1(double);
    double GetY_2(double);
    double GetY_3(double);
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};
#endif /* TESTLEASTSQREGRESSION_H_ */
