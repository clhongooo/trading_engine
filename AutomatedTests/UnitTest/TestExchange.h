#ifndef UNITTEST_TESTEXCHANGE_H_
#define UNITTEST_TESTEXCHANGE_H_

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <cmath>
#include "Exchange.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestExchange : public CppUnit::TestFixture
{
  public:
    TestExchange();
    virtual ~TestExchange();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};


#endif /* UNITTEST_TESTEXCHANGE_H_ */
