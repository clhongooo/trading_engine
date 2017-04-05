#include "TestDistributions.h"

TestDistributions::TestDistributions() {
}

TestDistributions::~TestDistributions() {
}

void TestDistributions::RunTest()
{
  //--------------------------------------------------
  // Cauchy
  //--------------------------------------------------
  CauchyDistribution cd;
  cd.SetParam(0.1,2);
  CPPUNIT_ASSERT(abs(cd.F(-0.3)-0.437167) < 0.001);
  CPPUNIT_ASSERT(abs(cd.F_inv(cd.F(-0.3))+0.3) < 0.05);

  cd.SetParam(1,10);
  CPPUNIT_ASSERT(abs(cd.F(3)-0.562833) < 0.001);
  CPPUNIT_ASSERT(abs(cd.F_inv(cd.F(3))-3) < 0.05);


  //--------------------------------------------------
  // Gaussian
  //--------------------------------------------------
  GaussianDistribution gd;
  gd.SetParam(0,1);
  CPPUNIT_ASSERT(abs(gd.F(0)-0.5) < 0.001);
  CPPUNIT_ASSERT(abs(gd.F_inv(gd.F(0.5))-0.5) < 0.001);
  CPPUNIT_ASSERT(abs(gd.F(-1.96)-0.025) < 0.001);
  CPPUNIT_ASSERT(abs(gd.F(1.96)-0.975) < 0.001);

  gd.SetParam(-1,2);
  CPPUNIT_ASSERT(abs(gd.F(-1)-0.5) < 0.001);
  CPPUNIT_ASSERT(abs(gd.F_inv(gd.F(2.5))-2.5) < 0.001);


  //--------------------------------------------------
  // Gumbel Copula
  //--------------------------------------------------
  GumbelCopula gc;
  gc.SetParam(2.5);
  CPPUNIT_ASSERT(abs(gc.C(1,1) - 1) < 0.01);
  CPPUNIT_ASSERT(abs(gc.C(1,0.7) - 0.7) < 0.01);
  CPPUNIT_ASSERT(abs(gc.C(0.7,1) - 0.7) < 0.01);

  CPPUNIT_ASSERT(abs(gc.C(0,0) - 0) < 0.01);
  CPPUNIT_ASSERT(abs(gc.C(0,1) - 0) < 0.01);
  CPPUNIT_ASSERT(abs(gc.C(1,0) - 0) < 0.01);

  CPPUNIT_ASSERT(abs(gc.Prob_U_smlr_u_V_eq_v(1,0) - 1) < 0.01);
  CPPUNIT_ASSERT(abs(gc.Prob_U_smlr_u_V_eq_v(0,0) - 0) < 0.01);
  CPPUNIT_ASSERT(abs(gc.Prob_U_smlr_u_V_eq_v(1,1) - 1) < 0.01);
  CPPUNIT_ASSERT(abs(gc.Prob_U_smlr_u_V_eq_v(0,1) - 0) < 0.01);

  CPPUNIT_ASSERT(gc.Prob_U_smlr_u_V_eq_v(  0,0.5) < gc.Prob_U_smlr_u_V_eq_v(0.1,0.5));
  CPPUNIT_ASSERT(gc.Prob_U_smlr_u_V_eq_v(0.1,0.5) < gc.Prob_U_smlr_u_V_eq_v(0.2,0.5));
  CPPUNIT_ASSERT(gc.Prob_U_smlr_u_V_eq_v(0.2,0.5) < gc.Prob_U_smlr_u_V_eq_v(0.3,0.5));
  CPPUNIT_ASSERT(gc.Prob_U_smlr_u_V_eq_v(0.3,0.5) < gc.Prob_U_smlr_u_V_eq_v(0.4,0.5));
  CPPUNIT_ASSERT(gc.Prob_U_smlr_u_V_eq_v(0.4,0.5) < gc.Prob_U_smlr_u_V_eq_v(0.5,0.5));
  CPPUNIT_ASSERT(gc.Prob_U_smlr_u_V_eq_v(0.5,0.5) < gc.Prob_U_smlr_u_V_eq_v(0.6,0.5));
  CPPUNIT_ASSERT(gc.Prob_U_smlr_u_V_eq_v(0.6,0.5) < gc.Prob_U_smlr_u_V_eq_v(0.7,0.5));
  CPPUNIT_ASSERT(gc.Prob_U_smlr_u_V_eq_v(0.7,0.5) < gc.Prob_U_smlr_u_V_eq_v(0.8,0.5));
  CPPUNIT_ASSERT(gc.Prob_U_smlr_u_V_eq_v(0.8,0.5) < gc.Prob_U_smlr_u_V_eq_v(0.9,0.5));
  CPPUNIT_ASSERT(gc.Prob_U_smlr_u_V_eq_v(0.9,0.5) < gc.Prob_U_smlr_u_V_eq_v(  1,0.5));




  return;
}

void TestDistributions::setUp()
{
}

void TestDistributions::tearDown()
{
}

CppUnit::Test *TestDistributions::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestDistributions");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestDistributions>(
        "TestDistributions", 
        &TestDistributions::RunTest));

  return suiteOfTests;
}
