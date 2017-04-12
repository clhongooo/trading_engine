#ifndef TESTVOLSURF_H_
#define TESTVOLSURF_H_

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "VolSurf.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestVolSurf : public CppUnit::TestFixture
{
  public:
    TestVolSurf();
    virtual ~TestVolSurf();
    void RunTest();
    void setUp();
    void tearDown();
    double GetIVByStrike(double);
    static CppUnit::Test *suite();
};

#endif /* TESTVOLSURF_H_ */
