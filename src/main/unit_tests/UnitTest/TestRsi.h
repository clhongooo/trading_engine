#ifndef TESTRSI_H_
#define TESTRSI_H_

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "Rsi.h"
#include "BarProvider.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestRsi : public CppUnit::TestFixture
{
  public:
    TestRsi();
    virtual ~TestRsi();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif /* TESTRSI_H_ */
