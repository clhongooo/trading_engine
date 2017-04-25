/*
 * TestSeqReset.h
 *
 *  Created on: Jun 5, 2014
 *      Author: sunny
 */

#ifndef TESTSEQRESET_H_
#define TESTSEQRESET_H_

#include "../UTest/UTest.h"
#include "BinaryTools.h"
#include "SystemConfig.h"
#include "SharedObjects.h"
#include "DataProcessing/PreProcessor.h"
#include "DataProcessing/RealTimeProcessor.h"
#include "DataProcessing/RefreshProcessor.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestSeqReset : public CppUnit::TestFixture
{
  public:
    TestSeqReset();
    virtual ~TestSeqReset();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif /* TESTSEQRESET_H_ */
