#ifndef UTEST_HPP
#define UTEST_HPP

#include <iostream>
#include <cstdlib>
#include "stdio.h"
#include "string.h"
#include <string>

using namespace std;

class UTest
{
  private:
    int _iPrbLine;
    char _sPrbFile[64];
    char _sPrbFunc[64];
    bool _bResult;
  public:
    UTest();
    void Assert(bool,const char*,const char*,int);
    void AssertF(bool,const char*,const char*,int);
    void FailIfReached(const char*,const char*,int);
    void PrintResult();
    bool ChkResult();
};

#endif
