#ifndef UNITTEST_TESTCPNLHIST_H_
#define UNITTEST_TESTCPNLHIST_H_

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "CPnLHist.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestCPnLHist : public CppUnit::TestFixture
{
  public:
    TestCPnLHist();
    virtual ~TestCPnLHist();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif /* UNITTEST_TESTCPNLHIST_H_ */
