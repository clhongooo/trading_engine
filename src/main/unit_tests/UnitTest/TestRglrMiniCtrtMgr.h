#ifndef UNITTEST_TESTRGLRMINICTRTMGR_H_
#define UNITTEST_TESTRGLRMINICTRTMGR_H_

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "RegularMiniCtrtManager.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestRglrMiniCtrtMgr : public CppUnit::TestFixture
{
  public:
    TestRglrMiniCtrtMgr();
    virtual ~TestRglrMiniCtrtMgr();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif /* UNITTEST_TESTRGLRMINICTRTMGR_H_ */
