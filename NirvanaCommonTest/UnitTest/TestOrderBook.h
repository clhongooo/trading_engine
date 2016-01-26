/*
 * TestOrderBook.h
 *
 *  Created on: May 26, 2014
 *      Author: sunny
 */

#ifndef TESTORDERBOOK_H_
#define TESTORDERBOOK_H_

#include "OrderBook.h"

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

class TestOrderBook {
  public:
    TestOrderBook();
    virtual ~TestOrderBook();
    bool Check(int[10][3],int[10][3],int[10][3],int[10][3]);
    void RunTest();
    void setUp();
    void tearDown();
    static CppUnit::Test *suite();
};

#endif /* TESTORDERBOOK_H_ */

