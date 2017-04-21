#include "TestLeastSqRegression.h"

TestLeastSqRegression::TestLeastSqRegression() {

}

TestLeastSqRegression::~TestLeastSqRegression() {
}

double TestLeastSqRegression::GetY_1(double dX)
{
  double a = 1;
  double b = 2;

  return
    (a * dX + b);
}
double TestLeastSqRegression::GetY_2(double dX)
{
  double a = 1;
  double b = -2;
  double c = 3;

  return
    (a * dX * dX +
     b * dX +
     c);
}
double TestLeastSqRegression::GetY_3(double dX)
{
  double a = 1;
  double b = -2;
  double c = 3;
  double d = -4;

  return
    (a * dX * dX * dX +
     b * dX * dX +
     c * dX +
     d);
}

void TestLeastSqRegression::RunTest()
{
  {
    LeastSqRegression lsr(3);

    lsr.AddPoint(-10,GetY_3(-10));
    lsr.AddPoint(-20,GetY_3(-20));
    lsr.AddPoint(-30,GetY_3(-30));
    lsr.AddPoint(-40,GetY_3(-40));
    lsr.AddPoint(-50,GetY_3(-50));
    lsr.AddPoint(-1,GetY_3(-1));
    lsr.AddPoint(-2,GetY_3(-2));
    lsr.AddPoint(-3,GetY_3(-3));
    lsr.AddPoint(-4,GetY_3(-4));
    lsr.AddPoint(-5,GetY_3(-5));
    lsr.AddPoint(0,GetY_3(0));
    lsr.AddPoint(1,GetY_3(1));
    lsr.AddPoint(2,GetY_3(2));
    lsr.AddPoint(3,GetY_3(3));
    lsr.AddPoint(4,GetY_3(4));
    lsr.AddPoint(5,GetY_3(5));
    lsr.AddPoint(10,GetY_3(10));
    lsr.AddPoint(20,GetY_3(20));
    lsr.AddPoint(30,GetY_3(30));
    lsr.AddPoint(40,GetY_3(40));
    lsr.AddPoint(50,GetY_3(50));

    vector<double> vSoln = lsr.Solve();

    for (unsigned int i = 0; i < vSoln.size(); ++i)
    {
      cout << i << ": " << vSoln[i] << endl;
    }

    CPPUNIT_ASSERT(vSoln.size() == 4);
    CPPUNIT_ASSERT(abs(vSoln[0] -  1  ) < 0.1);
    CPPUNIT_ASSERT(abs(vSoln[1] -  -2 ) < 0.1);
    CPPUNIT_ASSERT(abs(vSoln[2] -  3  ) < 0.1);
    CPPUNIT_ASSERT(abs(vSoln[3] -  -4 ) < 0.1);
  }
  {
    LeastSqRegression lsr(2);

    lsr.AddPoint(-10,GetY_2(-10));
    lsr.AddPoint(-20,GetY_2(-20));
    lsr.AddPoint(-30,GetY_2(-30));
    lsr.AddPoint(-40,GetY_2(-40));
    lsr.AddPoint(-50,GetY_2(-50));
    lsr.AddPoint(-1,GetY_2(-1));
    lsr.AddPoint(-2,GetY_2(-2));
    lsr.AddPoint(-3,GetY_2(-3));
    lsr.AddPoint(-4,GetY_2(-4));
    lsr.AddPoint(-5,GetY_2(-5));
    lsr.AddPoint(0,GetY_2(0));
    lsr.AddPoint(1,GetY_2(1));
    lsr.AddPoint(2,GetY_2(2));
    lsr.AddPoint(3,GetY_2(3));
    lsr.AddPoint(4,GetY_2(4));
    lsr.AddPoint(5,GetY_2(5));
    lsr.AddPoint(10,GetY_2(10));
    lsr.AddPoint(20,GetY_2(20));
    lsr.AddPoint(30,GetY_2(30));
    lsr.AddPoint(40,GetY_2(40));
    lsr.AddPoint(50,GetY_2(50));

    vector<double> vSoln = lsr.Solve();

    for (unsigned int i = 0; i < vSoln.size(); ++i)
    {
      cout << i << ": " << vSoln[i] << endl;
    }

    CPPUNIT_ASSERT(vSoln.size() == 3);
    CPPUNIT_ASSERT(abs(vSoln[0] -  1  ) < 0.1);
    CPPUNIT_ASSERT(abs(vSoln[1] -  -2 ) < 0.1);
    CPPUNIT_ASSERT(abs(vSoln[2] -  3  ) < 0.1);
  }

  {
    LeastSqRegression lsr(1);

    lsr.AddPoint(-10,GetY_1(-10));
    lsr.AddPoint(-20,GetY_1(-20));
    lsr.AddPoint(-30,GetY_1(-30));
    lsr.AddPoint(-40,GetY_1(-40));
    lsr.AddPoint(-50,GetY_1(-50));
    lsr.AddPoint(-1,GetY_1(-1));
    lsr.AddPoint(-2,GetY_1(-2));
    lsr.AddPoint(-3,GetY_1(-3));
    lsr.AddPoint(-4,GetY_1(-4));
    lsr.AddPoint(-5,GetY_1(-5));
    lsr.AddPoint(0,GetY_1(0));
    lsr.AddPoint(1,GetY_1(1));
    lsr.AddPoint(2,GetY_1(2));
    lsr.AddPoint(3,GetY_1(3));
    lsr.AddPoint(4,GetY_1(4));
    lsr.AddPoint(5,GetY_1(5));
    lsr.AddPoint(10,GetY_1(10));
    lsr.AddPoint(20,GetY_1(20));
    lsr.AddPoint(30,GetY_1(30));
    lsr.AddPoint(40,GetY_1(40));
    lsr.AddPoint(50,GetY_1(50));

    vector<double> vSoln = lsr.Solve();

    for (unsigned int i = 0; i < vSoln.size(); ++i)
    {
      cout << i << ": " << vSoln[i] << endl;
    }

    CPPUNIT_ASSERT(vSoln.size() == 2);
    CPPUNIT_ASSERT(abs(vSoln[0] -  1  ) < 0.1);
    CPPUNIT_ASSERT(abs(vSoln[1] -  2  ) < 0.1);
  }

  return;
}

void TestLeastSqRegression::setUp()
{
}

void TestLeastSqRegression::tearDown()
{
}

CppUnit::Test *TestLeastSqRegression::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestLeastSqRegression");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestLeastSqRegression>(
        "TestLeastSqRegression", 
        &TestLeastSqRegression::RunTest));

  return suiteOfTests;
}

