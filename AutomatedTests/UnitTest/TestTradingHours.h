#ifndef UNITTEST_TESTTRADINGHOURS_H_
#define UNITTEST_TESTTRADINGHOURS_H_

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "SDateTime.h"
#include "TradingHours.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestTradingHours : public CppUnit::TestFixture
{
  public:
    TestTradingHours();
    virtual ~TestTradingHours();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif /* UNITTEST_TESTTRADINGHOURS_H_ */
