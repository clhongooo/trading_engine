#ifndef UNITTEST_TESTSFUNCTIONAL_H_
#define UNITTEST_TESTSFUNCTIONAL_H_

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "SFunctional.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestSFunctional : public CppUnit::TestFixture
{
  public:
    TestSFunctional();
    virtual ~TestSFunctional();
    void RunTest();
    void setUp();
    void tearDown();
    double GetIVByStrike(double);
    static CppUnit::Test *suite();
};

#endif /* UNITTEST_TESTSFUNCTIONAL_H_ */
