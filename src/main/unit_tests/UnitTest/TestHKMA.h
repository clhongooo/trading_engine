#ifndef TESTHKMA_H
#define TESTHKMA_H

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <cmath>
#include "HKMA.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestHKMA : public CppUnit::TestFixture
{
  public:
    TestHKMA();
    virtual ~TestHKMA();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif
