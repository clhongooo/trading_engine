#include "TestGKYZ.h"

TestGKYZ::TestGKYZ() {

}

TestGKYZ::~TestGKYZ() {
}

void TestGKYZ::RunTest()
{
  GKYZ gkyz(20);


  gkyz.Add(14026.21,14522.39,13920.11,14441.43);
  gkyz.Add(14470.49,14632.07,14403.38,14559.24);
  gkyz.Add(14602.25,14701.98,14507.5,14573.21);
  gkyz.Add(14914.54,15178.27,14914.54,15098.95);
  gkyz.Add(14982.92,15032.27,14839.38,15011.52);
  gkyz.Add(15008.8,15281.81,14968.64,15189.33);
  gkyz.Add(15274.07,15438.89,15193.49,15408.54);
  gkyz.Add(15498.45,15548.26,15319.84,15329.6);
  gkyz.Add(15332.68,15637.78,15280.96,15621.73);
  gkyz.Add(15543.85,15597.63,15441.58,15496.99);
  gkyz.Add(15290.6,15290.6,14914.05,14975.53);
  gkyz.Add(14901.43,15039.46,14766.85,15024.53);
  gkyz.Add(15074.36,15192.22,15014.77,15188.04);
  gkyz.Add(15048.51,15048.51,14839.49,14930.72);
  gkyz.Add(14775.97,14775.97,14476.79,14659.32);
  gkyz.Add(14730.51,14851.86,14707.75,14738.21);
  gkyz.Add(14811.95,14941.23,14727.45,14748.36);
  gkyz.Add(14785.15,14900.14,14737.95,14796.55);
  gkyz.Add(14864.04,15154.92,14785.01,15095.53);
  gkyz.Add(15089.85,15089.85,14865.66,14869.94);
  gkyz.Add(14751.33,14751.33,14512.71,14589.58);
  gkyz.Add(15060.6,15312.61,15060.6,15235.03);
  gkyz.Add(15354.16,15494.56,15275.9,15447.61);
  gkyz.Add(15328.12,15451.72,15200.52,15436.53);
  gkyz.Add(15438.3,15555.97,15336.62,15500.59);
  gkyz.Add(15395.97,15539.3,15327.7,15435.79);
  gkyz.Add(15441.3,15481.76,15060.81,15090.77);
  gkyz.Add(15157.37,15330.15,15157.37,15295.42);
  gkyz.Add(15288.81,15388.02,15146.65,15293.76);
  gkyz.Add(15291.99,15430.95,15241.9,15363.15);
  gkyz.Add(15409.38,15451.39,15154.02,15261.48);
  gkyz.Add(15292.84,15553.51,15292.84,15528.75);
  gkyz.Add(15682.35,15964.93,15682.35,15933.55);
  gkyz.Add(15967.66,16106.16,15906.58,16099.27);
  gkyz.Add(16053.22,16099.06,15940.26,16044.21);
  gkyz.Add(16176.1,16255.11,15980.51,16099.82);      cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  20.60) < 0.01);
  gkyz.Add(16145.28,16167.95,15877.23,15893.07);     cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  20.46) < 0.01);
  gkyz.Add(15916.42,16106.71,15905.51,16102.35);     cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  20.34) < 0.01);
  gkyz.Add(16055.64,16193.17,15913.07,16163.99);     cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  20.06) < 0.01);
  gkyz.Add(16165.43,16274.67,16056.2,16071.29);      cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  20.22) < 0.01);
  gkyz.Add(16013.46,16013.46,15753.97,15830.84);     cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  20.05) < 0.01);
  gkyz.Add(15864.47,15936.48,15819.64,15913.24);     cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  16.35) < 0.01);
  gkyz.Add(15909.02,16060.32,15871.38,16049.47);     cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  15.93) < 0.01);
  gkyz.Add(15987.57,15987.57,15794.44,15909.4);      cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  15.54) < 0.01);
  gkyz.Add(15882.93,15899.24,15782.88,15873.28);     cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  15.28) < 0.01);
  gkyz.Add(15807.64,15807.64,15586.46,15693.11);     cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  15.11) < 0.01);
  gkyz.Add(15801.69,15915.06,15792.76,15842.72);     cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  14.68) < 0.01);
  gkyz.Add(15821.79,16021.22,15750.96,15860.42);     cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  15.07) < 0.01);
  gkyz.Add(15865.47,15870.98,15714.77,15756.37);     cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  14.67) < 0.01);
  gkyz.Add(15777.4,15813.15,15544.62,15630.31);      cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  14.87) < 0.01);
  gkyz.Add(15521.43,15573.74,15458.43,15490.96);     cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  14.50) < 0.01);
  gkyz.Add(15548.46,15613.44,15437.51,15527.36);     cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  14.58) < 0.01);
  gkyz.Add(15484.35,15484.35,15298.75,15351.51);     cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  14.11) < 0.01);
  gkyz.Add(15202.16,15202.16,15039.08,15098.64);     cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  14.47) < 0.01);
  gkyz.Add(15135.3,15302.7,15042.81,15280.56);       cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  14.72) < 0.01);
  gkyz.Add(15278.15,15304.66,15177.8,15230.22);      cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  13.97) < 0.01);
  gkyz.Add(14933.59,15059.17,14771.09,14834.73);     cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  15.98) < 0.01);
  gkyz.Add(14702.96,14842.69,14573.54,14787.87);     cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  16.77) < 0.01);
  gkyz.Add(14696.47,14696.47,14330.57,14360.56);     cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  16.80) < 0.01);
  gkyz.Add(14285.3,14285.3,13951.43,13966.43);       cout << gkyz.Value() << endl;     CPPUNIT_ASSERT(abs(gkyz.Value() * 100  -  16.92) < 0.01);

  return;
}

void TestGKYZ::setUp()
{
}

void TestGKYZ::tearDown()
{
}

CppUnit::Test *TestGKYZ::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestGKYZ");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestGKYZ>(
        "TestGKYZ", 
        &TestGKYZ::RunTest));

  return suiteOfTests;
}
