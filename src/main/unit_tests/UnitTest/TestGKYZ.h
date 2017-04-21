#ifndef TESTGKYZ_H_
#define TESTGKYZ_H_

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "GKYZ.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestGKYZ : public CppUnit::TestFixture
{
  public:
    TestGKYZ();
    virtual ~TestGKYZ();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif
