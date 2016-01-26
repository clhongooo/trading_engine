#ifndef TESTTRDTICKANA_H_
#define TESTTRDTICKANA_H_

#include <cmath>
#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "TrdTickAna.h"
#include "TradeTick.h"
#include "SDateTime.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestTrdTickAna : public CppUnit::TestFixture
{
  public:
    TestTrdTickAna();
    virtual ~TestTrdTickAna();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif
