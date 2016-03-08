#include <TestSFunctional.h>

TestSFunctional::TestSFunctional() {

}

TestSFunctional::~TestSFunctional() {
}

void TestSFunctional::RunTest()
{

  {
    SMap<string,string> sm;
    sm.AddOrUpdate("hello0","world0");
    sm.AddOrUpdate("hello1","world1");
    sm.AddOrUpdate("hello2","world2");
    sm.AddOrUpdate("hello3","world3");
    sm.AddOrUpdate("hello4","world4");
    sm.AddOrUpdate("hello5","world5");
    sm.AddOrUpdate("hello6","world6");
    sm.AddOrUpdate("hello7","world7");
    sm.AddOrUpdate("hello8","world8");
    sm.AddOrUpdate("hello9","world9");
    CPPUNIT_ASSERT(sm.ToVector()[0].first == "hello0");
    CPPUNIT_ASSERT(sm.ToVector()[0].second == "world0");
    CPPUNIT_ASSERT(sm.ToVector()[1].first == "hello1");
    CPPUNIT_ASSERT(sm.ToVector()[1].second == "world1");
    CPPUNIT_ASSERT(sm.ToVector()[8].first == "hello8");
    CPPUNIT_ASSERT(sm.ToVector()[8].second == "world8");
    CPPUNIT_ASSERT(sm.ToVector()[9].first == "hello9");
    CPPUNIT_ASSERT(sm.ToVector()[9].second == "world9");
    CPPUNIT_ASSERT(sm.Contains("hello0"));
    CPPUNIT_ASSERT(sm.Contains("hello9"));
    CPPUNIT_ASSERT(!sm.Contains("hello"));
    CPPUNIT_ASSERT(!sm.Contains(""));
    CPPUNIT_ASSERT(sm.Get("hello0") == Option<string>("world0"));
    CPPUNIT_ASSERT(sm.GetOrElse("hello0","") == "world0");
    CPPUNIT_ASSERT(sm.Get("haha") == Option<string>());
    CPPUNIT_ASSERT(sm.GetOrElse("haha","") == "");

    CPPUNIT_ASSERT(sm.Contains("hello7"));
    sm.Remove("hello7");
    CPPUNIT_ASSERT(!sm.Contains("hello7"));
  }


  {
    vector<std::pair<string,int> > vIn;
    std::pair<string,int> p;

    for (int i = 0; i < 100; ++i)
    {
      p.first = boost::lexical_cast<string>(i);
      p.second = i;
      vIn.push_back(p);
    }

    SMap<string,int> sm;
    sm.FromVectorReplaceOrAdd(vIn);

    CPPUNIT_ASSERT(sm.Get("0") == Option<int>(0));
    CPPUNIT_ASSERT(sm.Get("20") == Option<int>(20));
    CPPUNIT_ASSERT(sm.Get("40") == Option<int>(40));
    CPPUNIT_ASSERT(sm.Get("130") == Option<int>());
    CPPUNIT_ASSERT(sm.Get("1300") == Option<int>());

    vector<std::pair<string,int> > vIn2;
    for (int i = 0; i < 100; ++i)
    {
      p.first = boost::lexical_cast<string>(i);
      p.second = i+10;
      vIn2.push_back(p);
    }

    sm.FromVectorTotalReplace(vIn2);

    CPPUNIT_ASSERT(sm.Get("0") == Option<int>(10));
    CPPUNIT_ASSERT(sm.Get("20") == Option<int>(30));
    CPPUNIT_ASSERT(sm.Get("40") == Option<int>(50));
    CPPUNIT_ASSERT(sm.Get("130") == Option<int>());
    CPPUNIT_ASSERT(sm.Get("1300") == Option<int>());

    sm.FromVectorTotalReplace(vector<std::pair<string,int> >());

    CPPUNIT_ASSERT(sm.Get("0") == Option<int>());
    CPPUNIT_ASSERT(sm.Get("20") == Option<int>());
    CPPUNIT_ASSERT(sm.Get("40") == Option<int>());
    CPPUNIT_ASSERT(sm.Get("130") == Option<int>());
    CPPUNIT_ASSERT(sm.Get("1300") == Option<int>());
  }


  {
    SMap<string,int> sm;
    sm.AddOrUpdate("hello0",0);
    sm.AddOrUpdate("hello1",1);
    sm.AddOrUpdate("hello2",2);
    sm.AddOrUpdate("hello3",3);
    sm.AddOrUpdate("hello4",4);
    sm.AddOrUpdate("hello5",5);
    sm.AddOrUpdate("hello6",6);
    sm.AddOrUpdate("hello7",7);
    sm.AddOrUpdate("hello8",8);
    sm.AddOrUpdate("hello9",9);
    CPPUNIT_ASSERT(sm.Get("hello0") == Option<int>(0));
    CPPUNIT_ASSERT(sm.GetOrElse("hello0",10) == 0);
    CPPUNIT_ASSERT(sm.Get("haha") == Option<int>());
    CPPUNIT_ASSERT(sm.GetOrElse("haha",10) == 10);
  }


  {
    SMap<int,int> sm;
    sm.AddOrUpdate(1,1);
    sm.AddOrUpdate(2,2);
    sm.AddOrUpdate(3,3);
    sm.AddOrUpdate(4,4);
    sm.AddOrUpdate(5,5);
    sm.AddOrUpdate(6,6);
    sm.AddOrUpdate(7,7);
    sm.AddOrUpdate(8,8);
    sm.AddOrUpdate(9,9);
    CPPUNIT_ASSERT(sm.Get(7) == Option<int>(7));
    CPPUNIT_ASSERT(sm.GetOrElse(7,0) == 7);
    CPPUNIT_ASSERT(sm.Get(13) == Option<int>());
    CPPUNIT_ASSERT(sm.GetOrElse(13,0) == 0);
  }

  {
    SMapOfMap<string,string,int> smom;
    smom.AddOrUpdate("h1","w1",31);
    smom.AddOrUpdate("h2","w2",32);
    smom.AddOrUpdate("h3","w3",33);
    smom.AddOrUpdate("h4","w4",34);
    smom.AddOrUpdate("h5","w5",35);
    smom.AddOrUpdate("h6","w6",36);
    smom.AddOrUpdate("h7","w7",37);
    smom.AddOrUpdate("h8","w8",38);

    smom.AddOrUpdate("h9","w1",81);
    smom.AddOrUpdate("h9","w2",82);
    smom.AddOrUpdate("h9","w3",83);
    smom.AddOrUpdate("h9","w4",84);
    smom.AddOrUpdate("h9","w5",85);
    smom.AddOrUpdate("h9","w6",86);
    smom.AddOrUpdate("h9","w7",87);
    smom.AddOrUpdate("h9","w8",88);
    smom.AddOrUpdate("h9","w9",89);

    CPPUNIT_ASSERT(smom.Get("h9","w3") == Option<int>(83));
    CPPUNIT_ASSERT(smom.GetOrElse("h5","w5",0) == 35);
    CPPUNIT_ASSERT(smom.Get("hh","ww") == Option<int>());
    CPPUNIT_ASSERT(smom.GetOrElse("hh","ww",0) == 0);
  }

  {
    vector<double> v;
    for (double i = 0; i < 100; ++i) v.push_back(i);
    cout << FSum(v) << endl;
    CPPUNIT_ASSERT(FSum(v) == 50*101);
  }

  {
    vector<int> v;
    for (int i = 0; i < 100; ++i) v.push_back(i);
    cout << FSum(v) << endl;
    CPPUNIT_ASSERT(FSum(v) == 50*101);
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
