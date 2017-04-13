/*
 * TestSwitchRTSSvr.h
 *
 *  Created on: May 13, 2014
 *      Author: sunny
 */

#ifndef TESTSWITCHRTSSVR_H_
#define TESTSWITCHRTSSVR_H_

#include "../UTest/UTest.h"
#include "..//RTSTestingServer/RTSTestingServer.h"
#include "ChannelAggregation/RTSClient.h"
#include "SystemConfig.h"
#include "SharedObjects.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestSwitchRTSSvr : public CppUnit::TestFixture
{
  public:
    TestSwitchRTSSvr();
    virtual ~TestSwitchRTSSvr();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif /* TESTSWITCHRTSSVR_H_ */

