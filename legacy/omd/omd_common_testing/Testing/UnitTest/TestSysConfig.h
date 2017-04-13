/*
 * TestSysConfig.h
 *
 *  Created on: May 12, 2014
 *      Author: sunny
 */

#ifndef TESTSYSCONFIG_H_
#define TESTSYSCONFIG_H_

#include "SharedObjects.h"
#include "SystemConfig.h"
#include "DataProcessing/RealTimeProcessor.h"
#include "DataProcessing/RefreshProcessor.h"
#include "../UTest/UTest.h"
#include "Util.h"
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestSysConfig {
  public:
    TestSysConfig();
    virtual ~TestSysConfig();
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif /* TESTSYSCONFIG_H_ */
