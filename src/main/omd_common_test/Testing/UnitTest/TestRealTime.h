/*
 * TestRealTime.h
 *
 *  Created on: May 30, 2014
 *      Author: sunny
 */

#ifndef TESTREALTIME_H_
#define TESTREALTIME_H_

#include "SharedObjects.h"
#include "SystemConfig.h"
#include "../UTest/UTest.h"
#include "DataProcessing/RealTimeProcessor.h"
#include "SDateTime.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include "ChannelAggregation/RTSClient.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestRealTime : public CppUnit::TestFixture
{
  public:
    TestRealTime();
    virtual ~TestRealTime();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif /* TESTREALTIME_H_ */
