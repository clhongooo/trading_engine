#include "TestTickReturn.h"

TestTickReturn::TestTickReturn()
{
}

TestTickReturn::~TestTickReturn()
{
}

void TestTickReturn::RunTest()
{
  TickReturn tr(60);
  CPPUNIT_ASSERT(tr.Return () == -999);
  CPPUNIT_ASSERT(tr.ReturnR() == -999);

  tr.Add(YYYYMMDDHHMMSS(20150130, 91501), 23); CPPUNIT_ASSERT(abs(tr.Return() -   -999) < 0.1);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91511), 89); CPPUNIT_ASSERT(abs(tr.Return() -   -999) < 0.1);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91521), 26); CPPUNIT_ASSERT(abs(tr.Return() -   -999) < 0.1);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91531), 38); CPPUNIT_ASSERT(abs(tr.Return() -   -999) < 0.1);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91541), 89); CPPUNIT_ASSERT(abs(tr.Return() -   -999) < 0.1);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91551), 72); CPPUNIT_ASSERT(abs(tr.Return() -   -999) < 0.1);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91601), 75); CPPUNIT_ASSERT(abs(tr.Return() -   -999) < 0.1);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91602), 75); CPPUNIT_ASSERT(abs(tr.Return() - -0.157) < 0.1);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91611), 27); CPPUNIT_ASSERT(abs(tr.Return() - -0.696) < 0.1);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91621), 98);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91631), 99);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91641), 96);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91651), 30);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91701), 55);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91711), 8 );
  tr.Add(YYYYMMDDHHMMSS(20150130, 91721), 26);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91731), 7 );
  tr.Add(YYYYMMDDHHMMSS(20150130, 91741), 23);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91751), 38);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91801), 86);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91811), 8 );
  tr.Add(YYYYMMDDHHMMSS(20150130, 91821), 26);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91831), 93);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91841), 91);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91851), 69);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91901), 22);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91911), 72);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91921), 88);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91931), 22);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91941), 74);
  tr.Add(YYYYMMDDHHMMSS(20150130, 91951), 96);
  tr.Add(YYYYMMDDHHMMSS(20150130, 92001), 44);
  tr.Add(YYYYMMDDHHMMSS(20150130, 92011), 24);
  tr.Add(YYYYMMDDHHMMSS(20150130, 92021), 11);
  tr.Add(YYYYMMDDHHMMSS(20150130, 92031), 46);
  tr.Add(YYYYMMDDHHMMSS(20150130, 92041), 13);
  tr.Add(YYYYMMDDHHMMSS(20150130, 92051), 13); CPPUNIT_ASSERT(abs(tr.Return() - -0.86 ) < 0.1);
  tr.Add(YYYYMMDDHHMMSS(20150130, 92052), 13); CPPUNIT_ASSERT(abs(tr.Return() - -0.704) < 0.1);

  return;
}


void TestTickReturn::setUp()
{
}

void TestTickReturn::tearDown()
{
}

CppUnit::Test *TestTickReturn::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestTickReturn");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestTickReturn>(
        "TestTickReturn",
        &TestTickReturn::RunTest));

  return suiteOfTests;
}

