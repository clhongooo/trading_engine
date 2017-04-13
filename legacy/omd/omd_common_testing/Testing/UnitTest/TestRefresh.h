/*
 * TestRefresh.h
 *
 *  Created on: May 22, 2014
 *      Author: sunny
 */

#ifndef TESTREFRESH_H_
#define TESTREFRESH_H_

#include "SharedObjects.h"
#include "SystemConfig.h"
#include "../RTSTestingServer/RTSTestingServer.h"
#include "ChannelAggregation/DataCompletenessInspector.h"
#include "../UTest/UTest.h"
#include "Util.h"
#include "DataProcessing/RefreshProcessor.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestRefresh : public CppUnit::TestFixture
{
  public:
    TestRefresh();
    virtual ~TestRefresh();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif /* TESTREFRESH_H_ */

