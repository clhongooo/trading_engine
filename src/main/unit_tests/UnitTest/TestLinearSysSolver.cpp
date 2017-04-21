#include "TestLinearSysSolver.h"

TestLinearSysSolver::TestLinearSysSolver() {

}

TestLinearSysSolver::~TestLinearSysSolver() {
}

void TestLinearSysSolver::RunTest()
{
  {
    LinearSysSolver lss(4);

    double a_data[] =
    { 0.18, 0.60, 0.57, 0.96,
      0.41, 0.24, 0.99, 0.58,
      0.14, 0.30, 0.97, 0.66,
      0.51, 0.13, 0.19, 0.85 };

    double b_data[] = { 1.0, 2.0, 3.0, 4.0 };

    vector<double> vMatrixA(a_data,a_data+sizeof(a_data)/sizeof(double));
    vector<double> vVectorB(b_data,b_data+sizeof(b_data)/sizeof(double));

    lss.SpecifyMatrixA(vMatrixA);
    lss.SpecifyVectorB(vVectorB);
    vector<double> rtnVec = lss.Solve();

    CPPUNIT_ASSERT(abs(rtnVec[0] - -4.05205) < 0.1);
    CPPUNIT_ASSERT(abs(rtnVec[1] - -12.6056) < 0.1);
    CPPUNIT_ASSERT(abs(rtnVec[2] -  1.66091) < 0.1);
    CPPUNIT_ASSERT(abs(rtnVec[3] -  8.69377) < 0.1);
  }
  {
    LinearSysSolver lss(2);

    double a_data[] =
    { 0.18, 0.60,
      0.36, 1.20 };

    double b_data[] = { 1.0, 2.0 };

    vector<double> vMatrixA(a_data,a_data+sizeof(a_data)/sizeof(double));
    vector<double> vVectorB(b_data,b_data+sizeof(b_data)/sizeof(double));

    lss.SpecifyMatrixA(vMatrixA);
    lss.SpecifyVectorB(vVectorB);
    vector<double> rtnVec = lss.Solve();

    CPPUNIT_ASSERT(rtnVec.size() == 0);
  }
  {
    LinearSysSolver lss(2);

    double a_data[] =
    { 0.18, 0.60,
      0.18, 0.60 };

    double b_data[] = { 1.0, 2.0 };

    vector<double> vMatrixA(a_data,a_data+sizeof(a_data)/sizeof(double));
    vector<double> vVectorB(b_data,b_data+sizeof(b_data)/sizeof(double));

    lss.SpecifyMatrixA(vMatrixA);
    lss.SpecifyVectorB(vVectorB);
    vector<double> rtnVec = lss.Solve();

    CPPUNIT_ASSERT(rtnVec.size() == 0);
  }

  return;
}

void TestLinearSysSolver::setUp()
{
}

void TestLinearSysSolver::tearDown()
{
}

CppUnit::Test *TestLinearSysSolver::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestLinearSysSolver");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestLinearSysSolver>(
        "TestLinearSysSolver", 
        &TestLinearSysSolver::RunTest));

  return suiteOfTests;
}
