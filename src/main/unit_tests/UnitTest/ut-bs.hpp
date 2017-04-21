#include "UTest.h"
#include "Math/BlackScholes.h"
#include <iostream>
#include <cmath>

using namespace std;

int TestBS()
{
  UTest ut;
  double p, p2, iv;

  double tau = (double)1/12;
  double dFutPx = 0;

  //--------------------------------------------------
  // Underlying
  //--------------------------------------------------
  p = BlackScholes::EuropeanCallPrice(20000,21000,0.01,0.03,tau,0.2);
  cout << p << endl;
  ut.AssertF(abs(p - 124.38) > 1, __FILE__,__FUNCTION__,__LINE__);

  p = BlackScholes::EuropeanPutPrice(20000,19000,0.01,0.03,tau,0.2);
  cout << p << endl;
  ut.AssertF(abs(p - 121.42) > 1, __FILE__,__FUNCTION__,__LINE__);

  //--------------------------------------------------
  // Futures price
  //--------------------------------------------------
  p = BlackScholes::EuropeanCallPrice(20000,21000,0.01,0,tau,0.2);
  dFutPx = 20000*pow(2.71828,0.01*tau);
  p2 = BlackScholes::EuropeanCallPriceFut(dFutPx,21000,0.01,tau,0.2);
  cout << p << ", " << p2<< endl;
  ut.AssertF(abs(p - p2) > 0.01, __FILE__,__FUNCTION__,__LINE__);

  p = BlackScholes::EuropeanPutPrice(20000,19000,0.01,0,tau,0.2);
  dFutPx = 20000*pow(2.71828,0.01*tau);
  p2 = BlackScholes::EuropeanPutPriceFut(dFutPx,19000,0.01,tau,0.2);
  cout << p << ", " << p2<< endl;
  ut.AssertF(abs(p - p2) > 0.01, __FILE__,__FUNCTION__,__LINE__);

  //--------------------------------------------------
  // test algorithm speed
  //--------------------------------------------------
  //for (long i = 0; i < 900000; ++i)
  {
    //--------------------------------------------------
    // Underlying
    //--------------------------------------------------
    iv = BlackScholes::ImpldVolatility(true,20000,21000,0.01,0.03,tau,124.38);
    cout << iv << endl;
    ut.AssertF(abs(iv - 0.2) > 0.01, __FILE__,__FUNCTION__,__LINE__);

    iv = BlackScholes::ImpldVolatility(false,20000,19000,0.01,0.03,tau,121.42);
    cout << iv << endl;
    ut.AssertF(abs(iv - 0.2) > 0.01, __FILE__,__FUNCTION__,__LINE__);


    iv = BlackScholes::ImpldVolatility(true,20000,21000,0.01,0.03,tau,124.38,0.21);
    cout << iv << endl;
    ut.AssertF(abs(iv - 0.2) > 0.01, __FILE__,__FUNCTION__,__LINE__);

    iv = BlackScholes::ImpldVolatility(false,20000,19000,0.01,0.03,tau,121.42,0.21);
    cout << iv << endl;
    ut.AssertF(abs(iv - 0.2) > 0.01, __FILE__,__FUNCTION__,__LINE__);

    //--------------------------------------------------
    // Futures price
    //--------------------------------------------------
    dFutPx = 20000*pow(2.71828,0.01*tau);

    iv = BlackScholes::ImpldVolatilityFut(true,dFutPx,21000,0.01,tau,134.621);
    cout << iv << endl;
    ut.AssertF(abs(iv - 0.2) > 0.01, __FILE__,__FUNCTION__,__LINE__);

    iv = BlackScholes::ImpldVolatilityFut(false,dFutPx,19000,0.01,tau,112.365);
    cout << iv << endl;
    ut.AssertF(abs(iv - 0.2) > 0.01, __FILE__,__FUNCTION__,__LINE__);

    iv = BlackScholes::ImpldVolatilityFut(true,dFutPx,21000,0.01,tau,134.621,0.21);
    cout << iv << endl;
    ut.AssertF(abs(iv - 0.2) > 0.01, __FILE__,__FUNCTION__,__LINE__);

    iv = BlackScholes::ImpldVolatilityFut(false,dFutPx,19000,0.01,tau,112.365,0.21);
    cout << iv << endl;
    ut.AssertF(abs(iv - 0.2) > 0.01, __FILE__,__FUNCTION__,__LINE__);

  }

  p = BlackScholes::CallDelta(20000,21000,0.01,0.03,tau,0.2);
  cout << "Call Delta: " << p << endl;

  p = BlackScholes::PutDelta(20000,19000,0.01,0.03,tau,0.2);
  cout << "Put Delta: " << p << endl;

  p = BlackScholes::Gamma(20000,21000,0.01,0.03,tau,0.2);
  cout << "Call Gamma: " << p << endl;
  p = BlackScholes::Gamma(20000,19000,0.01,0.03,tau,0.2);
  cout << "Put Gamma: " << p << endl;


  ut.PrintResult();


  return 0;
}
