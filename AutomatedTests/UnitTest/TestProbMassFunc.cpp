#include "TestProbMassFunc.h"

TestProbMassFunc::TestProbMassFunc() {

}

TestProbMassFunc::~TestProbMassFunc() {
}

void TestProbMassFunc::RunTest()
{

  {
    //--------------------------------------------------
    // Normal case
    //--------------------------------------------------
    ProbMassFunc pmf;

    CPPUNIT_ASSERT(pmf.CheckValidity(20140615160000) == false);

    pmf.SetPr(20140615160000,22000,0.1);
    pmf.SetPr(20140615160000,22800,0.4);
    pmf.SetPr(20140615160000,23000,0.4);
    pmf.SetPr(20140615160000,23200,0.1);

    CPPUNIT_ASSERT(pmf.CheckValidity(20140615160000) == true);

    CPPUNIT_ASSERT(pmf.GetPr(20140615160000,22000) == 0.1);
    CPPUNIT_ASSERT(pmf.GetPr(20140615160000,22800) == 0.4);
    CPPUNIT_ASSERT(pmf.GetPr(20140615160000,23000) == 0.4);
    CPPUNIT_ASSERT(pmf.GetPr(20140615160000,23200) == 0.1);


    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,21000,22000) - 0) < 0.1);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,22000,22400) - 0.1) < 0.1);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,22400,22800) - 0.32) < 0.1);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,22800,23000) - 0.28) < 0.1);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,23000,23100) - 0.2) < 0.1);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,23100,23200) - 0.1) < 0.1);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,23200,23600) - 0) < 0.1);

    pmf.Normalize(20140615160000);

    CPPUNIT_ASSERT(pmf.CheckValidity(20140615160000) == true);

    CPPUNIT_ASSERT(pmf.GetPr(20140615160000,22000) == 0.1);
    CPPUNIT_ASSERT(pmf.GetPr(20140615160000,22800) == 0.4);
    CPPUNIT_ASSERT(pmf.GetPr(20140615160000,23000) == 0.4);
    CPPUNIT_ASSERT(pmf.GetPr(20140615160000,23200) == 0.1);

    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,21000,22000) - 0) < 0.1);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,22000,22400) - 0.1) < 0.1);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,22400,22800) - 0.32) < 0.1);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,22800,23000) - 0.28) < 0.1);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,23000,23100) - 0.2) < 0.1);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,23100,23200) - 0.1) < 0.1);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,23200,23600) - 0) < 0.1);

    //--------------------------------------------------
    // Adjust SD
    //--------------------------------------------------
    CPPUNIT_ASSERT(abs(pmf.GetMean(20140615160000) - 22840)      < 0.05);
    pmf.AdjustToStdevNmlz(20140615160000,500);

    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,  20000                  ,(21473.01+22774.90)/2) - 0.1) < 0.005);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,  (21473.01+22774.90)/2  ,(22774.90+23100.37)/2) - 0.4) < 0.005);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,  (22774.90+23100.37)/2  ,(23100.37+23425.85)/2) - 0.4) < 0.005);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,  (23100.37+23425.85)/2  ,30000                ) - 0.1) < 0.005);

    CPPUNIT_ASSERT(abs(pmf.GetVariance(20140615160000) - 250000) < 0.05);
    CPPUNIT_ASSERT(abs(pmf.GetStdev(20140615160000) - 500)       < 0.05);
    CPPUNIT_ASSERT(abs(pmf.GetMean(20140615160000) - 22840)      < 0.05);

    //--------------------------------------------------
    // Local Reset
    //--------------------------------------------------
    pmf.Reset(20140615160000);

    CPPUNIT_ASSERT(pmf.CheckValidity(20140615160000) == false);
    CPPUNIT_ASSERT(pmf.GetPr(20140615160000,22800,23000) == -1);
    CPPUNIT_ASSERT(pmf.GetPr(20140615160000,23200,23400) == -1);
    CPPUNIT_ASSERT(pmf.GetPr(20140615160000,24000,24200) == -1);
    CPPUNIT_ASSERT(pmf.GetPr(20140615160000,24200,26000) == -1);

    pmf.Normalize(20140615160000);

    CPPUNIT_ASSERT(pmf.CheckValidity(20140615160000) == false);
    CPPUNIT_ASSERT(pmf.GetPr(20140615160000,22800,23000) == -1);
    CPPUNIT_ASSERT(pmf.GetPr(20140615160000,23200,23400) == -1);
    CPPUNIT_ASSERT(pmf.GetPr(20140615160000,24000,24200) == -1);
    CPPUNIT_ASSERT(pmf.GetPr(20140615160000,24200,26000) == -1);

    //--------------------------------------------------
    // Error prob
    //--------------------------------------------------
    pmf.SetPr(20140615160000,22000,0.2);
    pmf.SetPr(20140615160000,22800,0.8);
    pmf.SetPr(20140615160000,23000,0.8);
    pmf.SetPr(20140615160000,23200,0.2);

    CPPUNIT_ASSERT(pmf.CheckValidity(20140615160000) == false);

    pmf.Normalize(20140615160000);

    CPPUNIT_ASSERT(pmf.CheckValidity(20140615160000) == true);

    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,21000,22000) - 0) < 0.1);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,22000,22400) - 0.1) < 0.1);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,22400,22800) - 0.32) < 0.1);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,22800,23000) - 0.28) < 0.1);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,23000,23100) - 0.2) < 0.1);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,23100,23200) - 0.1) < 0.1);
    CPPUNIT_ASSERT(abs(pmf.GetPr(20140615160000,23200,23600) - 0) < 0.1);

    //--------------------------------------------------
    // Merging PMF
    //--------------------------------------------------
    ProbMassFunc pmf1;
    pmf1.SetPr(20140615160000,22000,0.5);
    pmf1.SetPr(20140615160000,23000,0.5);

    ProbMassFunc pmf2;
    pmf2.SetPr(20140615160000,21000,0.5);
    pmf2.SetPr(20140615160000,24000,0.5);

    pmf1.MergeWith(20140615160000,pmf2,20140615160000,1);

    CPPUNIT_ASSERT(abs(pmf1.GetPr(20140615160000,21000) - 0.25) < 0.1);
    CPPUNIT_ASSERT(abs(pmf1.GetPr(20140615160000,22000) - 0.25) < 0.1);
    CPPUNIT_ASSERT(abs(pmf1.GetPr(20140615160000,23000) - 0.25) < 0.1);
    CPPUNIT_ASSERT(abs(pmf1.GetPr(20140615160000,24000) - 0.25) < 0.1);

    ProbMassFunc pmf3;
    pmf3.SetPr(20140615160000,22000,0.5);
    pmf3.SetPr(20140615160000,23000,0.5);

    pmf1.MergeWith(20140615160000,pmf3,20140615160000,1);

    CPPUNIT_ASSERT(abs(pmf1.GetPr(20140615160000,21000) - 0.125) < 0.1);
    CPPUNIT_ASSERT(abs(pmf1.GetPr(20140615160000,22000) - 0.375) < 0.1);
    CPPUNIT_ASSERT(abs(pmf1.GetPr(20140615160000,23000) - 0.375) < 0.1);
    CPPUNIT_ASSERT(abs(pmf1.GetPr(20140615160000,24000) - 0.125) < 0.1);

    pmf1.Reset(20140615160000);
    pmf1.SetPr(20140615160000,22000,0.5);
    pmf1.SetPr(20140615160000,23000,0.5);

    pmf1.MergeWith(20140615160000,pmf2,20140615160000,0.5);

    CPPUNIT_ASSERT(abs(pmf1.GetPr(20140615160000,21000) - 0.16666) < 0.1);
    CPPUNIT_ASSERT(abs(pmf1.GetPr(20140615160000,22000) - 0.33333) < 0.1);
    CPPUNIT_ASSERT(abs(pmf1.GetPr(20140615160000,23000) - 0.33333) < 0.1);
    CPPUNIT_ASSERT(abs(pmf1.GetPr(20140615160000,24000) - 0.16666) < 0.1);

    //--------------------------------------------------
    // Import
    //--------------------------------------------------
    pmf1.Reset(20140615160000);

    pmf1.ImportButNotNmlz(20140615160000,pmf2,20140615160000,0.5);
    CPPUNIT_ASSERT(abs(pmf1.GetPr(20140615160000,21000) - 0.25) < 0.1);
    CPPUNIT_ASSERT(abs(pmf1.GetPr(20140615160000,24000) - 0.25) < 0.1);

    pmf1.ImportButNotNmlz(20140615160000,pmf2,20140615160000,0.1);
    CPPUNIT_ASSERT(abs(pmf1.GetPr(20140615160000,21000) - 0.3) < 0.1);
    CPPUNIT_ASSERT(abs(pmf1.GetPr(20140615160000,24000) - 0.3) < 0.1);

    pmf1.Normalize(20140615160000);
    CPPUNIT_ASSERT(abs(pmf1.GetPr(20140615160000,21000) - 0.5) < 0.1);
    CPPUNIT_ASSERT(abs(pmf1.GetPr(20140615160000,24000) - 0.5) < 0.1);

  }

  return;
}

void TestProbMassFunc::setUp()
{
}

void TestProbMassFunc::tearDown()
{
}

CppUnit::Test *TestProbMassFunc::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestProbMassFunc");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestProbMassFunc>(
        "TestProbMassFunc", 
        &TestProbMassFunc::RunTest));

  return suiteOfTests;
}
