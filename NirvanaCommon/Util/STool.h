#ifndef STOOL_HPP
#define STOOL_HPP

#include "PCH.h"
#include "Constants.h"
#include "Tokenizer.h"
#include "ValidityChk.h"
#include "SException.h"


using namespace std;

class STool
{
  public:
    static double Max(double dA, double dB);
    static double Min(double dA, double dB);
    static int Max(int iA, int iB);
    static int Max(int iA, int iB, int iC);
    static int Min(int iA, int iB);
    static int Min(int iA, int iB, int iC);
    static double IntPart1(double d);
    static double IntPart2(double d);
    static double Round(double d, unsigned u);
    static int Abs(int iA);
    static double RoundIntvl(double d, double dintvl);
    static string DecToBin(int iDec);
    static int BinToDec(const string &sBin);
    static string ToStr(int i);
    static string CharToStr(char c);
    static string ToStr(double d, unsigned u);
    static double ToDbl(const string &s);
    static double ToDbl(const string &s, unsigned u);
    // static int ToInt(const string &s);
    static string PadLeft(unsigned iOri, char cPad, unsigned iLen);
    static string PadLeft(const string &sOri, char cPad, unsigned iLen);
    static string PadLeftS(string sOri, char cPad, unsigned iLen);
    static string PadLeft(const char *, char cPad, unsigned iLen);
    static string PadRight(unsigned iOri, char cPad, unsigned iLen);
    static string PadRight(const string &sOri, char cPad, unsigned iLen);
    static string PadRightS(string sOri, char cPad, unsigned iLen);
    static string PadRight(const char *, char cPad, unsigned iLen);
    static void TrimRightB(string &s);
    static void TrimLeftB(string &s);
    static void TrimB(string &s);
    static string TrimRightS(string s);
    static string TrimLeftS(string s);
    static string TrimS(string s);
    static string& TrimRight(string &s);
    static string& TrimLeft(string &s);
    static string& Trim(string &s);
    static string ConstructStr(const string *s, int iSize, const string &sDelim);
    static string ToUpperCase(string & s);
    static string ToLowerCase(string & s);
    static bool IsNaN(double value);
    template<typename T1,typename T2>
      static bool ContainsKey(map<T1,T2> m,T1 k)
      {
        if (m.find(k) == m.end()) return false;
        else return true;
      }

    static void ReadFile(const string & sPath, deque<string> &);
    static void ReadFile(const char *sPath, deque<string> &);
    static void ReadCSVFile(const string & sPath, deque<vector<string> > &);
    static void ReadCSVFile(const char *sPath, deque<vector<string> > &);
    static bool ChkIfFileExists(const string &);
    static bool ChkIfFileExists(const char *);
    static bool IsStringAllLetters(const string & s);
    static double KellyCriterion(const double,const double);
    static bool IsValidPrice(const double);
    static bool IsNumber(const string &);
    static string GetNthItemFromCSV(const string &,const int);
    static int Sign(const long);
    static double Sign(const double);

};

#endif
