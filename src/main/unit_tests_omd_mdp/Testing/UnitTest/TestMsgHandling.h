/*
 * TestMsgHandling.h
 *
 *  Created on: May 23, 2014
 *      Author: sunny
 */

#ifndef TESTMSGHANDLING_H_
#define TESTMSGHANDLING_H_

#include "SharedObjects.h"
#include "SystemConfig.h"
#include "DataProcessing/RealTimeProcessor.h"
#include "DataProcessing/RefreshProcessor.h"
#include "../UTest/UTest.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestMsgHandling : public CppUnit::TestFixture
{
  public:
    TestMsgHandling();
    virtual ~TestMsgHandling();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif /* TESTMSGHANDLING_H_ */
