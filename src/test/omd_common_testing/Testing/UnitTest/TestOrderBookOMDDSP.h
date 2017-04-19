/*
 * TestOrderBookOMDDSP.h
 *
 *  Created on: Jul 15, 2014
 *      Author: sunny
 */

#ifndef TESTORDERBOOKOMDDSP_H_
#define TESTORDERBOOKOMDDSP_H_

#include "Container/OrderBook.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestOrderBookOMDDSP {
  public:
    TestOrderBookOMDDSP();
    virtual ~TestOrderBookOMDDSP();
    bool Check(int[10][3],int[10][3],int[10][3],int[10][3]);
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif /* TESTORDERBOOKOMDDSP_H_ */
