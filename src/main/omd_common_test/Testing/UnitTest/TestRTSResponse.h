/*
 * TestRTSResponse.h
 *
 *  Created on: Jun 22, 2014
 *      Author: sunny
 */

#ifndef TESTRTSRESPONSE_H_
#define TESTRTSRESPONSE_H_

#include "../UTest/UTest.h"
#include "ChannelAggregation/DataCompletenessInspector.h"
#include "..//RTSTestingServer/RTSTestingServer.h"
#include "SystemConfig.h"
#include "SharedObjects.h"
#include "SDateTime.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>


class TestRTSResponse : public CppUnit::TestFixture
{
  public:
    TestRTSResponse();
    virtual ~TestRTSResponse();

    void RunTest();
    void setUp();
    void tearDown();

    static CppUnit::Test *suite();
};

#endif /* TESTRTSRESPONSE_H_ */
