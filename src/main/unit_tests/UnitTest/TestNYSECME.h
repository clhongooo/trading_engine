#ifndef UNITTEST_TESTNYSECME_H_
#define UNITTEST_TESTNYSECME_H_

#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "NYSE.h"
#include "CME.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestNYSECME : public CppUnit::TestFixture
{
  public:
    TestNYSECME();
    virtual ~TestNYSECME();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif /* UNITTEST_TESTNYSECME_H_ */
