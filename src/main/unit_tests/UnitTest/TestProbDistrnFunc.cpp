#include "TestProbDistrnFunc.h"

TestProbDistrnFunc::TestProbDistrnFunc() {

}

TestProbDistrnFunc::~TestProbDistrnFunc() {
}

void TestProbDistrnFunc::RunTest()
{
  // {
  //   //--------------------------------------------------
  //   // Normal case
  //   //--------------------------------------------------
  //   ProbDistrnFunc pdf;
  //
  //   CPPUNIT_ASSERT(pdf.CheckValidity(201406151600) == false);
  //
  //   pdf.SetPr(201406151600,22000,22800,0.1);
  //   pdf.SetPr(201406151600,22800,23000,0.4);
  //   pdf.SetPr(201406151600,23000,23200,0.4);
  //   pdf.SetPr(201406151600,23200,24000,0.1);
  //
  //   CPPUNIT_ASSERT(pdf.CheckValidity(201406151600) == true);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,22800,23000) == 0.4);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,23200,23400) == 0.025);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,24000,24200) == 0);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,24200,26000) == 0);
  //
  //   pdf.Normalize(201406151600);
  //
  //   CPPUNIT_ASSERT(pdf.CheckValidity(201406151600) == true);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,22800,23000) == 0.4);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,23200,23400) == 0.025);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,24000,24200) == 0);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,24200,26000) == 0);
  //
  //
  //   //--------------------------------------------------
  //   // Local Reset
  //   //--------------------------------------------------
  //   pdf.Reset(201406151600);
  //
  //   CPPUNIT_ASSERT(pdf.CheckValidity(201406151600) == true);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,22800,23000) == 0);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,23200,23400) == 0);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,24000,24200) == 0);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,24200,26000) == 0);
  //
  //   pdf.Normalize(201406151600);
  //
  //   CPPUNIT_ASSERT(pdf.CheckValidity(201406151600) == true);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,22800,23000) == 0);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,23200,23400) == 0);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,24000,24200) == 0);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,24200,26000) == 0);
  //
  //   //--------------------------------------------------
  //   // Error prob
  //   //--------------------------------------------------
  //   pdf.SetPr(201406151600,22000,22800,0.2);
  //   pdf.SetPr(201406151600,22800,23000,0.8);
  //   pdf.SetPr(201406151600,23000,23200,0.8);
  //   pdf.SetPr(201406151600,23200,24000,0.2);
  //
  //   CPPUNIT_ASSERT(pdf.CheckValidity(201406151600) == false);
  //
  //   pdf.Normalize(201406151600);
  //
  //   CPPUNIT_ASSERT(pdf.CheckValidity(201406151600) == true);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,22800,23000) == 0.4);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,23200,23400) == 0.025);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,24000,24200) == 0);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,24200,26000) == 0);
  //
  // }
  //
  // {
  //   ProbDistrnFunc pdf;
  //
  //   //--------------------------------------------------
  //   // Test local and global Reset
  //   //--------------------------------------------------
  //   pdf.SetPr(201406151600,22000,22800,0.1);
  //   pdf.SetPr(201406151600,22800,23000,0.4);
  //   pdf.SetPr(201406151600,23000,23200,0.4);
  //   pdf.SetPr(201406151600,23200,24000,0.1);
  //
  //   pdf.SetPr(201506151600,22000,22800,0.1);
  //   pdf.SetPr(201506151600,22800,23000,0.4);
  //   pdf.SetPr(201506151600,23000,23200,0.4);
  //   pdf.SetPr(201506151600,23200,24000,0.1);
  //
  //   //--------------------------------------------------
  //   // Local Reset
  //   //--------------------------------------------------
  //   pdf.Reset(201506151600);
  //
  //   CPPUNIT_ASSERT(pdf.CheckValidity(201406151600) == true);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,22800,23000) == 0.4);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,23200,23400) == 0.025);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,24000,24200) == 0);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,24200,26000) == 0);
  //
  //   CPPUNIT_ASSERT(pdf.CheckValidity(201506151600) == false);
  //   CPPUNIT_ASSERT(pdf.GetPr(201506151600,22800,23000) == 0);
  //   CPPUNIT_ASSERT(pdf.GetPr(201506151600,23200,23400) == 0);
  //   CPPUNIT_ASSERT(pdf.GetPr(201506151600,24000,24200) == 0);
  //   CPPUNIT_ASSERT(pdf.GetPr(201506151600,24200,26000) == 0);
  //
  //   //--------------------------------------------------
  //   // Global Reset
  //   //--------------------------------------------------
  //   pdf.Reset();
  //
  //   CPPUNIT_ASSERT(pdf.CheckValidity(201406151600) == false);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,22800,23000) == 0);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,23200,23400) == 0);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,24000,24200) == 0);
  //   CPPUNIT_ASSERT(pdf.GetPr(201406151600,24200,26000) == 0);
  //
  //   CPPUNIT_ASSERT(pdf.CheckValidity(201506151600) == false);
  //   CPPUNIT_ASSERT(pdf.GetPr(201506151600,22800,23000) == 0);
  //   CPPUNIT_ASSERT(pdf.GetPr(201506151600,23200,23400) == 0);
  //   CPPUNIT_ASSERT(pdf.GetPr(201506151600,24000,24200) == 0);
  //   CPPUNIT_ASSERT(pdf.GetPr(201506151600,24200,26000) == 0);
  //
  // }
  //
  // {
  //   //--------------------------------------------------
  //   // Test ProbDistrnFuncBld
  //   //--------------------------------------------------
  //   ProbDistrnFuncBld pdfb;
  //
  //   //--------------------------------------------------
  //   // EraseCounts
  //   //--------------------------------------------------
  //   for (unsigned int i = 0; i < 100; ++i)
  //   {
  //     pdfb.AddCount(201406151600, 23213+i);
  //     pdfb.AddCount(201506151600, 23213-i);
  //   }
  //
  //   pdfb.EraseCounts(201406151600);
  //   CPPUNIT_ASSERT(pdfb.GetAllCounts(201406151600) == 0);
  //   CPPUNIT_ASSERT(pdfb.GetAllCounts(201506151600) == 100);
  //
  //   pdfb.EraseCounts(201506151600);
  //   CPPUNIT_ASSERT(pdfb.GetAllCounts(201406151600) == 0);
  //   CPPUNIT_ASSERT(pdfb.GetAllCounts(201506151600) == 0);
  //
  //   //--------------------------------------------------
  //   // EraseAllCounts
  //   //--------------------------------------------------
  //   for (unsigned int i = 0; i < 100; ++i)
  //   {
  //     pdfb.AddCount(201406151600, 23213+i);
  //     pdfb.AddCount(201506151600, 23213-i);
  //   }
  //
  //   pdfb.EraseAllCounts();
  //   CPPUNIT_ASSERT(pdfb.GetAllCounts(201406151600) == 0);
  //   CPPUNIT_ASSERT(pdfb.GetAllCounts(201506151600) == 0);
  //
  //   //--------------------------------------------------
  //   // GenerateDistrnFunc
  //   //--------------------------------------------------
  //   for (unsigned int i = 0; i < 100; ++i)
  //     pdfb.AddCount(201306151600, 23000);
  //
  //   for (unsigned int i = 0; i < 50; ++i)
  //     pdfb.AddCount(201306151600, 22000);
  //
  //   for (unsigned int i = 0; i < 20; ++i)
  //     pdfb.AddCount(201306151600, 24000);
  //
  //   CPPUNIT_ASSERT(!pdfb.GenerateDistrnFunc(ProbDistrnFuncBld::LINEAR, 201306151600)); // not set the bounds yet
  //
  //   pdfb.SetLowerBound(201306151600,21000);
  //   CPPUNIT_ASSERT(!pdfb.GenerateDistrnFunc(ProbDistrnFuncBld::LINEAR, 201306151600)); // not set the bounds yet
  //
  //   pdfb.SetUpperBound(201306151600,25000);
  //   CPPUNIT_ASSERT(pdfb.GenerateDistrnFunc(ProbDistrnFuncBld::LINEAR, 201306151600));
  //
  //   ProbDistrnFunc pdf;
  //   CPPUNIT_ASSERT(pdfb.GetProbDistrnFunc(201306151600,pdf));
  //
  //   //--------------------------------------------------
  //   // Test Saving / Loading data to / from file
  //   //--------------------------------------------------
  //   CPPUNIT_ASSERT(pdfb.SaveDistrnFuncToFile("test-pdf.csv",201306151600));
  //
  //   ProbDistrnFunc pdf2;
  //   CPPUNIT_ASSERT(pdfb.LoadDistrnFuncFrFile("test-pdf.csv",201206151600));
  //
  //   //--------------------------------------------------
  //   // Method: LINEAR
  //   // 21000-22000: 50/2       = 25 // area of triangle
  //   // 22000-23000: (50+100)/2 = 75 // area of trapezium
  //   // 23000-24000: (100+20)/2 = 60 // area of trapezium
  //   // 24000-25000: 20/2       = 10 // area of triangle
  //   // Total Area = 25+75+60+10 = 170
  //   //--------------------------------------------------
  //   CPPUNIT_ASSERT(abs(pdf2.GetPr(201206151600,19000,22000) - (double)25/170) < 0.001);
  //   CPPUNIT_ASSERT(abs(pdf2.GetPr(201206151600,20000,22000) - (double)25/170) < 0.001);
  //   CPPUNIT_ASSERT(abs(pdf2.GetPr(201206151600,21000,22000) - (double)25/170) < 0.001);
  //   CPPUNIT_ASSERT(abs(pdf2.GetPr(201206151600,22000,23000) - (double)75/170) < 0.001);
  //   CPPUNIT_ASSERT(abs(pdf2.GetPr(201206151600,23000,24000) - (double)60/170) < 0.001);
  //   CPPUNIT_ASSERT(abs(pdf2.GetPr(201206151600,24000,25000) - (double)10/170) < 0.001);
  //   CPPUNIT_ASSERT(abs(pdf2.GetPr(201206151600,24000,28000) - (double)10/170) < 0.001);
  //   CPPUNIT_ASSERT(abs(pdf2.GetPr(201206151600,21500,22500) - (double)(25+75)/2/170) < 0.001);
  //   CPPUNIT_ASSERT(abs(pdf2.GetPr(201206151600,21500,23000) - (double)(25+100)*1.5/2/170) < 0.001);
  //   CPPUNIT_ASSERT(abs(pdf2.GetPr(201206151600,20000,25000) - (double)1) < 0.001);
  //   CPPUNIT_ASSERT(abs(pdf2.GetPr(201206151600,23500,24500) - (double)(60+10)/2) < 0.001);
  //   CPPUNIT_ASSERT(abs(pdf2.GetPr(201206151600,23500,25000) - (double)60*1.5/2) < 0.001);
  //
  // }
  //
  //

  return;
}

void TestProbDistrnFunc::setUp()
{
}

void TestProbDistrnFunc::tearDown()
{
}

CppUnit::Test *TestProbDistrnFunc::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestProbDistrnFunc");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestProbDistrnFunc>(
        "TestProbDistrnFunc", 
        &TestProbDistrnFunc::RunTest));

  return suiteOfTests;
}
