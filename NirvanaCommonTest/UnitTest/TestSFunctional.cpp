#include <TestSFunctional.h>

TestSFunctional::TestSFunctional() {

}

TestSFunctional::~TestSFunctional() {
}

void TestSFunctional::RunTest()
{
  {
    SMap<string,string> sm;
    sm.Add("hello","world");
    sm.Add("hello1","world1");
    sm.Add("hello2","world2");
    sm.Add("hello3","world3");
    sm.Add("hello4","world4");
    sm.Add("hello5","world5");
    sm.Add("hello6","world6");
    sm.Add("hello7","world7");
    sm.Add("hello8","world8");
    sm.Add("hello9","world9");
    CPPUNIT_ASSERT(sm.Get("hello") == Option<string>("world"));
    CPPUNIT_ASSERT(sm.GetOrElse("hello","") == "world");
    CPPUNIT_ASSERT(sm.Get("haha") == Option<string>());
    CPPUNIT_ASSERT(sm.GetOrElse("haha","") == "");
  }
  {
    SMap<int,int> sm;
    sm.Add(1,1);
    sm.Add(2,2);
    sm.Add(3,3);
    sm.Add(4,4);
    sm.Add(5,5);
    sm.Add(6,6);
    sm.Add(7,7);
    sm.Add(8,8);
    sm.Add(9,9);
    CPPUNIT_ASSERT(sm.Get(7) == Option<int>(7));
    CPPUNIT_ASSERT(sm.GetOrElse(7,0) == 7);
    CPPUNIT_ASSERT(sm.Get(13) == Option<int>());
    CPPUNIT_ASSERT(sm.GetOrElse(13,0) == 0);
  }

  {
    SMapOfMap<string,string,int> smom;
    smom.Add("h1","w1",31);
    smom.Add("h2","w2",32);
    smom.Add("h3","w3",33);
    smom.Add("h4","w4",34);
    smom.Add("h5","w5",35);
    smom.Add("h6","w6",36);
    smom.Add("h7","w7",37);
    smom.Add("h8","w8",38);

    smom.Add("h9","w1",81);
    smom.Add("h9","w2",82);
    smom.Add("h9","w3",83);
    smom.Add("h9","w4",84);
    smom.Add("h9","w5",85);
    smom.Add("h9","w6",86);
    smom.Add("h9","w7",87);
    smom.Add("h9","w8",88);
    smom.Add("h9","w9",89);

    CPPUNIT_ASSERT(smom.Get("h9","w3") == Option<int>(83));
    CPPUNIT_ASSERT(smom.GetOrElse("h5","w5",0) == 35);
    CPPUNIT_ASSERT(smom.Get("hh","ww") == Option<int>());
    CPPUNIT_ASSERT(smom.GetOrElse("hh","ww",0) == 0);
  }

{
    SMapOfMapPersistVal<string,string,int> smom;
    smom.Add("h1","w1",31);
    smom.Add("h2","w2",32);
    smom.Add("h3","w3",33);
    smom.Add("h4","w4",34);
    smom.Add("h5","w5",35);
    smom.Add("h6","w6",36);
    smom.Add("h7","w7",37);
    smom.Add("h8","w8",38);

    smom.Add("h9","w1",81);
    smom.Add("h9","w2",82);
    smom.Add("h9","w3",83);
    smom.Add("h9","w4",84);
    smom.Add("h9","w5",85);
    smom.Add("h9","w6",86);
    smom.Add("h9","w7",87);
    smom.Add("h9","w8",88);
    smom.Add("h9","w9",89);

    smom.Add("h1","w1",41);
    smom.Add("h2","w2",42);
    smom.Add("h3","w3",43);
    smom.Add("h4","w4",44);
    smom.Add("h5","w5",45);
    smom.Add("h6","w6",46);
    smom.Add("h7","w7",47);
    smom.Add("h8","w8",48);

    smom.Add("h9","w1",41);
    smom.Add("h9","w2",42);
    smom.Add("h9","w3",43);
    smom.Add("h9","w4",44);
    smom.Add("h9","w5",45);
    smom.Add("h9","w6",46);
    smom.Add("h9","w7",47);
    smom.Add("h9","w8",48);
    smom.Add("h9","w9",49);

    CPPUNIT_ASSERT(smom.Get("h9","w3") == Option<int>(83));
    CPPUNIT_ASSERT(smom.GetOrElse("h5","w5",0) == 35);
    CPPUNIT_ASSERT(smom.Get("hh","ww") == Option<int>());
    CPPUNIT_ASSERT(smom.GetOrElse("hh","ww",0) == 0);
  }

  return;
}

void TestSFunctional::setUp()
{
}

void TestSFunctional::tearDown()
{
}

CppUnit::Test *TestSFunctional::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestSFunctional");

  suiteOfTests->addTest(new CppUnit::TestCaller<TestSFunctional>(
      "TestSFunctional", 
      &TestSFunctional::RunTest));

  return suiteOfTests;
}
