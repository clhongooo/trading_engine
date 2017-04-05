#include "UTest.h"

UTest::UTest()
{
  _bResult = true;
  _iPrbLine = 0;
}

void UTest::FailIfReached(const char* sPrbFile,const char* sPrbFunc, int iLine)
{
  if (_bResult)
  {
    _iPrbLine = iLine;
    memset(_sPrbFile,'\0',64);
    strncpy(_sPrbFile, sPrbFile, 63);
    memset(_sPrbFunc,'\0',64);
    strncpy(_sPrbFunc, sPrbFunc, 63);
  }
  _bResult = false;
}

void UTest::AssertF(bool bA, const char* sPrbFile,const char* sPrbFunc, int iLine)
{
  Assert(!bA, sPrbFile, sPrbFile, iLine);
}

void UTest::Assert(bool bA, const char* sPrbFile,const char* sPrbFunc, int iLine)
{
  if (_bResult && !bA)
  {
    _iPrbLine = iLine;
    memset(_sPrbFile,'\0',64);
    strncpy(_sPrbFile, sPrbFile, 63);
    memset(_sPrbFunc,'\0',64);
    strncpy(_sPrbFunc, sPrbFunc, 63);
  }

  if (!bA) _bResult = false;
}

void UTest::PrintResult()
{
  cout << endl;
  cout << "\t\t\t========================================" << endl;
  cout << "\t\t\tUnit Test Result:" << endl;
  cout << "\t\t\t" << ((_bResult)?"Pass":"Fail :`(");
  if (!_bResult)
    cout << " at File: " << string(_sPrbFile) << ", Func: " << string(_sPrbFunc) << ", Line: " << _iPrbLine << endl;
  else cout << endl;
  cout << "\t\t\t========================================" << endl;

}

bool UTest::ChkResult()
{
  return _bResult;
}
