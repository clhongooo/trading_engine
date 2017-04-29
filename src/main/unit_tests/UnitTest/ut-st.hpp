#include "UTest.h"
#include <boost/shared_ptr.hpp>
#include "STool.h"
#include <iostream>
#include <string>
#include <vector>
#include <deque>

int TestST()
{
  UTest ut;

  ut.Assert(STool::Min(0.1,0.2) == 0.1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Min(-0.1,-0.2) == -0.2,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Min(123,234) == 123,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Min(-123,-234) == -234,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(STool::Max(0.1,0.2) == 0.2,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Max(-0.1,-0.2) == -0.1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Max(123,234) == 234,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Max(-123,-234) == -123,__FILE__,__FUNCTION__,__LINE__);


  ut.Assert(STool::Min(10,21) == 10,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Min(-101,-108) == -108,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Min(123,234) == 123,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Min(-123,-234) == -234,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(STool::Max(10,21) == 21,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Max(-101,-108) == -101,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Max(123,234) == 234,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Max(-123,-234) == -123,__FILE__,__FUNCTION__,__LINE__);




  ut.Assert(STool::Sign(0.0) == 1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Sign(3.0) == 1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Sign(-33.0) == -1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Sign(2.34567) == 1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Sign(-2.34567) == -1,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(STool::Sign((int)0) == 1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Sign((int)3) == 1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Sign((int)-33) == -1,__FILE__,__FUNCTION__,__LINE__);


  ut.Assert(STool::Round(2.34567,2) == 2.35,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Round(2.499999,2) == 2.5,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Round(2.999999,2) == 3,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(STool::Round(-2.34567,2) == -2.35,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Round(-2.499999,2) == -2.5,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Round(-2.999999,2) == -3,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(STool::Abs(-2) == 2,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Abs(-20) == 20,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Abs(30) == 30,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::Abs(-99) == 99,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(STool::RoundIntvl(3.446742,0.05) == 3.45,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::RoundIntvl(3.446742,0.5) == 3.5,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::RoundIntvl(3.446742,0.002) == 3.446,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::RoundIntvl(-3.446742,0.05) == -3.45,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(STool::DecToBin(56).compare("111000") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::BinToDec("11010") == 26,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(STool::IntPart1(2.34567) == 2,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::IntPart1(2.9999) == 2,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::IntPart1(2.0000001) == 2,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::IntPart1(-2.34567) == -2,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::IntPart1(-2.9999) == -2,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::IntPart1(-2.0000001) == -2,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::IntPart2(2.34567) == 3,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::IntPart2(2.9999) == 3,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::IntPart2(2.0000001) == 3,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::IntPart2(-2.34567) == -3,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::IntPart2(-2.9999) == -3,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::IntPart2(-2.0000001) == -3,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(STool::ToStr(109).compare("109") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::ToStr(3.141592653,3).compare("3.142") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::ToStr(3.141592653,4).compare("3.1416") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::CharToStr('f').compare("f") == 0,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(STool::ToDbl("56.234") == 56.234,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::ToDbl("-56.235") == -56.235,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::ToDbl("56.234",2) == 56.23,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::ToDbl("-56.235",2) == -56.24,__FILE__,__FUNCTION__,__LINE__);

  // ut.Assert(STool::ToInt("56.234") == 56,__FILE__,__FUNCTION__,__LINE__);
  // ut.Assert(STool::ToInt("-56.235") == -56,__FILE__,__FUNCTION__,__LINE__);


  string s;
  char sc[100];
  s = "s";           ut.Assert(STool::PadLeft (s,'0',5).compare("0000s") == 0,__FILE__,__FUNCTION__,__LINE__);
  s = "ssss";        ut.Assert(STool::PadLeft (s,'0',3).compare("ssss") == 0,__FILE__,__FUNCTION__,__LINE__);
  s = "ssss";        ut.Assert(STool::PadLeft (s,'0',10).compare("000000ssss") == 0,__FILE__,__FUNCTION__,__LINE__);
  strcpy(sc,"ssss"); ut.Assert(STool::PadLeft (sc,'0',10).compare("000000ssss") == 0,__FILE__,__FUNCTION__,__LINE__);
  strcpy(sc,"ssss"); ut.Assert(STool::PadRight(sc,'0',10).compare("ssss000000") == 0,__FILE__,__FUNCTION__,__LINE__);
  s = "ssss";        ut.Assert(STool::PadRight(s,'0',10).compare("ssss000000") == 0,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(STool::PadLeftS("s",'0',5).compare("0000s") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::PadLeftS("ssss",'0',3).compare("ssss") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::PadLeftS("ssss",'0',10).compare("000000ssss") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::PadRightS("ssss",'0',10).compare("ssss000000") == 0,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(STool::PadLeft(1,'0',5).compare("00001") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::PadLeft(1111,'0',3).compare("1111") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::PadLeft(1111,'0',10).compare("0000001111") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::PadRight(1111,'0',10).compare("1111000000") == 0,__FILE__,__FUNCTION__,__LINE__);

  string sTest;

  sTest = "  qwer  ";
  STool::TrimInPlace(sTest);
  ut.Assert(sTest == "qwer",__FILE__,__FUNCTION__,__LINE__);

  sTest = "  qwer  ";
  STool::TrimLeftInPlace(sTest);
  ut.Assert(sTest == "qwer  ",__FILE__,__FUNCTION__,__LINE__);

  sTest = "  qwer  ";
  STool::TrimRightInPlace(sTest);
  ut.Assert(sTest == "  qwer",__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(STool::Trim("  qwer  ") == "qwer",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::TrimRight("  qwer  ") == "  qwer",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::TrimLeft("  qwer  ") == "qwer  ",__FILE__,__FUNCTION__,__LINE__);

  sTest = "  qwer  "; ut.Assert(STool::Trim(sTest) == "qwer",__FILE__,__FUNCTION__,__LINE__);
  sTest = "  qwer  "; ut.Assert(STool::TrimRight(sTest) == "  qwer",__FILE__,__FUNCTION__,__LINE__);
  sTest = "  qwer  "; ut.Assert(STool::TrimLeft(sTest) == "qwer  ",__FILE__,__FUNCTION__,__LINE__);
  sTest = "00000941"; ut.Assert(STool::TrimLeft(sTest, "0") == "941",__FILE__,__FUNCTION__,__LINE__);
  sTest = "00094100"; ut.Assert(STool::TrimLeft(sTest, "0") == "94100",__FILE__,__FUNCTION__,__LINE__);
  sTest = "000941  "; ut.Assert(STool::TrimLeft(sTest, "0") == "941  ",__FILE__,__FUNCTION__,__LINE__);
  sTest = "   941  "; ut.Assert(STool::TrimLeft(sTest, "0") == "   941  ",__FILE__,__FUNCTION__,__LINE__);


  string s1[] = {"2010"};
  string s2[] = {"2010","10"};
  string s3[] = {"2010","10","18"};
  string s4[] = {"2010","10","18","4"};
  string s5[] = {"2010","10","18","4","54"};
  string s6[] = {"2010","10","18","4","54","16"};

  ut.Assert(STool::ConstructStr(s1,1,"-").compare("2010") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::ConstructStr(s2,2,"-").compare("2010-10") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::ConstructStr(s3,3,"-").compare("2010-10-18") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::ConstructStr(s4,4,"-").compare("2010-10-18-4") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::ConstructStr(s5,5,"-").compare("2010-10-18-4-54") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::ConstructStr(s6,6,"-").compare("2010-10-18-4-54-16") == 0,__FILE__,__FUNCTION__,__LINE__);



  //map
  map<int,double> m1;
  m1[123] = 56.78;
  ut.Assert(STool::ContainsKey(m1,123),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(STool::ContainsKey(m1,122),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(STool::ContainsKey(m1,124),__FILE__,__FUNCTION__,__LINE__);

  //read file
  deque<string> dqS; STool::ReadFile("./UnitTest/test.txt",dqS);
  ut.Assert(dqS.size() == 5,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(dqS[1].compare("Line two.") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(dqS[2].compare("Line three.") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(dqS[3].compare("Line three.") == 0,__FILE__,__FUNCTION__,__LINE__);

  //IsStringAllLetters
  ut.Assert(STool::IsStringAllLetters("C"),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::IsStringAllLetters("SPY"),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::IsStringAllLetters("IBKR"),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(STool::IsStringAllLetters("HSIV5"),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(STool::IsStringAllLetters("1"),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(STool::IsStringAllLetters("939"),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(STool::IsStringAllLetters("69394"),__FILE__,__FUNCTION__,__LINE__);


  ut.Assert(!STool::ChkIfFileExists("fuckyou"),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::ChkIfFileExists("./UnitTest/test.txt"),__FILE__,__FUNCTION__,__LINE__);



  ut.Assert(abs(STool::KellyCriterion(0.52,2)-0.28)<0.001,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(STool::KellyCriterion(0.3,3)-0.0666)<0.001,__FILE__,__FUNCTION__,__LINE__);

  // ut.Assert(STool::IsSymContFut("HSIc1"),__FILE__,__FUNCTION__,__LINE__);
  // ut.Assert(STool::IsSymContFut("HSIC1"),__FILE__,__FUNCTION__,__LINE__);
  // ut.Assert(STool::IsSymContFut("ESc1"),__FILE__,__FUNCTION__,__LINE__);
  // ut.Assert(!STool::IsSymContFut("ESM5"),__FILE__,__FUNCTION__,__LINE__);
  // ut.Assert(!STool::IsSymContFut("HHIF5"),__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(STool::ToUpperCase("haha") == "HAHA",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(STool::ToLowerCase("HEHE") == "hehe",__FILE__,__FUNCTION__,__LINE__);

  {
    string s("Notice,that,unlike,member,find_first_of,,whenever,more,than,one,character,is,being,searched,for,,it,is,not,enough,that,just,one,of,these,characters,match,,but,the,entire,sequence,must,match");
    ut.Assert(STool::GetNthItemFromCSV(s,1) == "Notice",__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(STool::GetNthItemFromCSV(s,5) == "find_first_of",__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(STool::GetNthItemFromCSV(s,6) == "",__FILE__,__FUNCTION__,__LINE__);
    ut.Assert(STool::GetNthItemFromCSV(s,7) == "whenever",__FILE__,__FUNCTION__,__LINE__);
  }

  ut.PrintResult();

  return 0;
}
