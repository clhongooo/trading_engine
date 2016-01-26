#ifndef TESTHKSE_H_
#define TESTHKSE_H_

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <cmath>
#include "HKSE.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestHKSE : public CppUnit::TestFixture
{
  public:
    TestHKSE();
    virtual ~TestHKSE();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif
