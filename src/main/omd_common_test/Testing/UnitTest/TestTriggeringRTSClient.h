/*
 * TestTriggeringRTSClient.h
 *
 *  Created on: May 12, 2014
 *      Author: sunny
 */

#ifndef TESTTRIGGERINGRTSCLIENT_H_
#define TESTTRIGGERINGRTSCLIENT_H_

#include "../UTest/UTest.h"
#include "ChannelAggregation/DataCompletenessInspector.h"
#include "../RTSTestingServer/RTSTestingServer.h"
#include "SystemConfig.h"
#include "SharedObjects.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>


class TestTriggeringRTSClient : public CppUnit::TestFixture
{
  public:
    TestTriggeringRTSClient();
    virtual ~TestTriggeringRTSClient();
    void RunTest();
    void setUp();
    void tearDown();

    static CppUnit::Test *suite();
};

#endif /* TESTTRIGGERINGRTSCLIENT_H_ */
