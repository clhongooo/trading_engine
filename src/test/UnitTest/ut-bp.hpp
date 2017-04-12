#include "UTest.h"
#include "Util/BarProvider.h"

int TestBP()
{
  UTest ut;



  // Forward No Header
  BarProvider bp("./UnitTest/test-plain.csv", "DOHLCV", 2, 'F', 'N', 'P');

  for (int k = 0; k < 3; k++)
  {
    int iCount = 1;
    bp.ResetPsn();
    while (bp.HasNextBar())
    {
      if (iCount == 1) ut.Assert(bp.PeekNextYMD() == YYYYMMDD(20120903), __FILE__,__FUNCTION__,__LINE__);
      bp.MoveNext();
      // cout << bp.GetYYYYMMDD().ToStrYYYYMMDD() << endl;
      // cout << bp.GetHHMMSS().ToStrHHMMSS() << endl;
      // cout << bp.Volume() << endl;
      // cout << bp.High() << endl;
      // cout << bp.Low() << endl;
      // cout << bp.Close() << endl;
      if (iCount == 1) ut.Assert(bp.Volume() == 5670, __FILE__,__FUNCTION__,__LINE__);
      if (iCount == 2) ut.Assert(bp.PeekPrevYMD() == YYYYMMDD(20120903), __FILE__,__FUNCTION__,__LINE__);
      if (iCount == 2) ut.Assert(bp.PeekPrevClose() == 19559.21, __FILE__,__FUNCTION__,__LINE__);
      if (iCount == 2) ut.Assert(bp.High() == 19572.31, __FILE__,__FUNCTION__,__LINE__);
      if (iCount == 3) ut.Assert(bp.Low() == 19116.84, __FILE__,__FUNCTION__,__LINE__);
      if (iCount == 4) ut.Assert(bp.Close() == 19209.30, __FILE__,__FUNCTION__,__LINE__);
      // if (iCount == 4) ut.Assert(abs(bp.Return()-19209.3/19145.07) < 0.0001, __FILE__,__FUNCTION__,__LINE__);
      if (iCount == 7) ut.Assert(bp.GetYYYYMMDD() == YYYYMMDD(20120911), __FILE__,__FUNCTION__,__LINE__);
      if (iCount == 8) ut.Assert(bp.GetYYYYMMDD() == YYYYMMDD(20120912), __FILE__,__FUNCTION__,__LINE__);
      if (iCount == 9) ut.Assert(bp.PeekPrevYMD() == YYYYMMDD(20120912), __FILE__,__FUNCTION__,__LINE__);
      if (iCount == 9)
      {
        vector<double> dTmp;
        ut.Assert(bp.GetCloseArr(5,dTmp), __FILE__,__FUNCTION__,__LINE__);
        ut.Assert(dTmp[0] == 19802.16, __FILE__,__FUNCTION__,__LINE__);
        ut.Assert(dTmp[1] == 19827.17, __FILE__,__FUNCTION__,__LINE__);
        ut.Assert(dTmp[2] == 19857.88, __FILE__,__FUNCTION__,__LINE__);
        ut.Assert(dTmp[3] == 20075.39, __FILE__,__FUNCTION__,__LINE__);
        ut.Assert(dTmp[4] == 20047.63, __FILE__,__FUNCTION__,__LINE__);
      }
      // if (iCount == 9)
      // {
      //   const double * dTmp = NULL;
      //   dTmp = bp.GetReturnArr(5);
      //   ut.Assert(abs(dTmp[3] - 1.0109533) < 0.0001, __FILE__,__FUNCTION__,__LINE__);
      //   ut.Assert(abs(dTmp[4] - 0.9986172) < 0.0001, __FILE__,__FUNCTION__,__LINE__);
      // }
      iCount++;
    }

    ut.Assert(bp.GetNumOfBars() == 57, __FILE__,__FUNCTION__,__LINE__);
  }


  try
  {
    bp.MoveNext();
    ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
  } catch (std::exception &e) {}



  //--------------------------------------------------
  // Test GetOHLCVInDateRange
  //--------------------------------------------------
  vector<YYYYMMDD> vYMD;
  vector<double> vO;
  vector<double> vH;
  vector<double> vL;
  vector<double> vC;
  vector<long> vV;
  bp.GetOHLCVInDateRange(YYYYMMDD(20120920),YYYYMMDD(20121015),vYMD,vO,vH,vL,vC,vV);
  ut.Assert(vO.size() == 16, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(vH.size() == 16, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(vL.size() == 16, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(vC.size() == 16, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(vV.size() == 16, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(vO[0]     == 20816.77, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(vC[1]     == 20734.94, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(vL.back() == 21076.95, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(vV.back() == 0, __FILE__,__FUNCTION__,__LINE__);

  bp.GetOHLCVInDateRange(YYYYMMDD(20140920),YYYYMMDD(20121015),vYMD,vO,vH,vL,vC,vV);
  ut.Assert(vO.size() == 0, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(vH.size() == 0, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(vL.size() == 0, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(vC.size() == 0, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(vV.size() == 0, __FILE__,__FUNCTION__,__LINE__);

  bp.GetOHLCVInDateRange(YYYYMMDD(20121015),YYYYMMDD(20121015),vYMD,vO,vH,vL,vC,vV);
  ut.Assert(vO.size() == 1, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(vH.size() == 1, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(vL.size() == 1, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(vC.size() == 1, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(vV.size() == 1, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(vC[0]     == 21148.25, __FILE__,__FUNCTION__,__LINE__);
  //--------------------------------------------------








  // Backward With Header (Yahoo)
  BarProvider bp2("./UnitTest/0001.HK.csv", "DOHLCV", 2, 'B', 'H', 'P');

  for (int k = 0; k < 3; k++)
  {
    bp2.ResetPsn();
    ut.Assert(bp2.GetNumOfBars() == 3516, __FILE__,__FUNCTION__,__LINE__);
    for (int i = 0; i < 2517; i++) bp2.MoveNext();
    ut.Assert(bp2.GetYYYYMMDD() == YYYYMMDD(20090904), __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp2.GetHHMMSS() == HHMMSS::BAD_TIME, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp2.Open() == 90.5, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp2.High() == 94.5, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp2.Low() == 89.7, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp2.Close() == 93.10, __FILE__,__FUNCTION__,__LINE__);
    // ut.Assert(abs(bp2.Return()-93.10/90.15) < 0.0001, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp2.Volume() == 7413600, __FILE__,__FUNCTION__,__LINE__);

    {
      vector<double> dTmp;
      ut.Assert(bp2.GetCloseArr(5,dTmp), __FILE__,__FUNCTION__,__LINE__);
      ut.Assert(dTmp[0] == 93.1, __FILE__,__FUNCTION__,__LINE__);
      ut.Assert(dTmp[1] == 90.15, __FILE__,__FUNCTION__,__LINE__);
      ut.Assert(dTmp[2] == 90.20, __FILE__,__FUNCTION__,__LINE__);
      ut.Assert(dTmp[3] == 91.70, __FILE__,__FUNCTION__,__LINE__);
      ut.Assert(dTmp[4] == 91.95, __FILE__,__FUNCTION__,__LINE__);
    }
    // {
    //   const double * dTmp = NULL;
    //   dTmp = bp2.GetReturnArr(5);
    //   ut.Assert(abs(dTmp[0] - 1.032723) < 0.0001, __FILE__,__FUNCTION__,__LINE__);
    //   ut.Assert(abs(dTmp[1] - 0.999445) < 0.0001, __FILE__,__FUNCTION__,__LINE__);
    // }

    for (int i = 0; i < 999; i++) bp2.MoveNext();
    ut.AssertF(bp2.HasNextBar(), __FILE__,__FUNCTION__,__LINE__);
  }

  try
  {
    bp2.MoveNext();
    ut.FailIfReached(__FILE__,__FUNCTION__,__LINE__);
  } catch (std::exception &e) {}




  // HSIF1M.csv (With Time)
  BarProvider bp3("./UnitTest/test-HSIF1M.csv", "DTOHLCV", 0, 'F', 'N', 'P');

  for (int k = 0; k < 3; k++)
  {
    ut.Assert(bp3.GetNumOfBars() == 8958, __FILE__,__FUNCTION__,__LINE__);
    for (int i = 0; i < 1000; i++) bp3.MoveNext();
    ut.Assert(bp3.GetYYYYMMDD() == YYYYMMDD(20130305), __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp3.GetHHMMSS() == HHMMSS(144645), __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp3.Open() == 22550, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp3.High() == 22555, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp3.Low() == 22544, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp3.Close() == 22549, __FILE__,__FUNCTION__,__LINE__);
    // ut.Assert(abs(bp3.Return()-(double)22549/22550) < 0.0001, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp3.Volume() == 231, __FILE__,__FUNCTION__,__LINE__);
    {
      vector<double> dTmp;
      ut.Assert(bp3.GetCloseArr(5,dTmp), __FILE__,__FUNCTION__,__LINE__);
      ut.Assert(dTmp[0] == 22576, __FILE__,__FUNCTION__,__LINE__);
      ut.Assert(dTmp[1] == 22574, __FILE__,__FUNCTION__,__LINE__);
      ut.Assert(dTmp[2] == 22569, __FILE__,__FUNCTION__,__LINE__);
      ut.Assert(dTmp[3] == 22550, __FILE__,__FUNCTION__,__LINE__);
      ut.Assert(dTmp[4] == 22549, __FILE__,__FUNCTION__,__LINE__);
    }
    // {
    //   const double * dTmp = NULL;
    //   dTmp = bp3.GetReturnArr(5);
    //   ut.Assert(abs(dTmp[4] - 0.999955) < 0.0001, __FILE__,__FUNCTION__,__LINE__);
    //   ut.Assert(abs(dTmp[3] - 0.999158) < 0.0001, __FILE__,__FUNCTION__,__LINE__);
    // }
    bp3.ResetPsn();
  }

  // HSIF1M.csv (With Time)
  BarProvider bp3a("./UnitTest/test-HSIF1M-hhmm.csv", "DTOHLCV", 0, 'F', 'N', 'P');

  for (int k = 0; k < 3; k++)
  {
    ut.Assert(bp3a.GetNumOfBars() == 8958, __FILE__,__FUNCTION__,__LINE__);
    for (int i = 0; i < 1000; i++) bp3a.MoveNext();
    ut.Assert(bp3a.GetYYYYMMDD() == YYYYMMDD(20130305), __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp3a.GetHHMMSS() == HHMMSS(144600), __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp3a.Open() == 22550, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp3a.High() == 22555, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp3a.Low() == 22544, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp3a.Close() == 22549, __FILE__,__FUNCTION__,__LINE__);
    // ut.Assert(abs(bp3a.Return()-(double)22549/22550) < 0.0001, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp3a.Volume() == 231, __FILE__,__FUNCTION__,__LINE__);
    {
      vector<double> dTmp;
      ut.Assert(bp3a.GetCloseArr(5,dTmp), __FILE__,__FUNCTION__,__LINE__);
      ut.Assert(dTmp[0] == 22576, __FILE__,__FUNCTION__,__LINE__);
      ut.Assert(dTmp[1] == 22574, __FILE__,__FUNCTION__,__LINE__);
      ut.Assert(dTmp[2] == 22569, __FILE__,__FUNCTION__,__LINE__);
      ut.Assert(dTmp[3] == 22550, __FILE__,__FUNCTION__,__LINE__);
      ut.Assert(dTmp[4] == 22549, __FILE__,__FUNCTION__,__LINE__);
    }
    // {
    //   const double * dTmp = NULL;
    //   dTmp = bp3a.GetReturnArr(5);
    //   ut.Assert(abs(dTmp[4] - 0.999955) < 0.0001, __FILE__,__FUNCTION__,__LINE__);
    //   ut.Assert(abs(dTmp[3] - 0.999158) < 0.0001, __FILE__,__FUNCTION__,__LINE__);
    // }
    bp3a.ResetPsn();
  }


  // GZipped file
  BarProvider bp4("./UnitTest/test-gzip.csv.gz", "DOHLCV", 2, 'B', 'H', 'G');

  for (int k = 0; k < 3; k++)
  {
    ut.Assert(bp4.GetNumOfBars() == 2447, __FILE__,__FUNCTION__,__LINE__);
    for (int i = 0; i < 1000; i++) { bp4.MoveNext(); ut.Assert(bp4.HasNextBar(), __FILE__,__FUNCTION__,__LINE__); }
    ut.Assert(bp4.GetYYYYMMDD() == YYYYMMDD(20071109), __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp4.GetHHMMSS() == HHMMSS::BAD_TIME, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp4.Open() == 44.5, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp4.High() == 45.55, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp4.Low() == 43.8, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp4.Close() == 44.45, __FILE__,__FUNCTION__,__LINE__);
    ut.Assert(bp4.Volume() == 111551400, __FILE__,__FUNCTION__,__LINE__);
    bp4.ResetPsn();
  }



  // Test sharing ability among multiple instances
  BarProvider bp10("./UnitTest/test-plain.csv", "DOHLCV", 2, 'F', 'N', 'P');
  BarProvider bp11(bp10);
  BarProvider bp12(bp10);


  bp10.ResetPsn();
  bp11.ResetPsn();
  bp12.ResetPsn();

  int iCount10 = 1;
  int iCount11 = 1;
  int iCount12 = 1;

  while (bp10.HasNextBar())
  {
    if (iCount10 == 1) ut.Assert(bp10.PeekNextYMD() == YYYYMMDD(20120903), __FILE__,__FUNCTION__,__LINE__);
    bp10.MoveNext();
    if (iCount10 == 1) ut.Assert(bp10.Volume() == 5670, __FILE__,__FUNCTION__,__LINE__);
    if (iCount10 == 2) ut.Assert(bp10.PeekPrevYMD() == YYYYMMDD(20120903), __FILE__,__FUNCTION__,__LINE__);
    if (iCount10 == 2) ut.Assert(bp10.PeekPrevClose() == 19559.21, __FILE__,__FUNCTION__,__LINE__);
    if (iCount10 == 2) ut.Assert(bp10.High() == 19572.31, __FILE__,__FUNCTION__,__LINE__);
    if (iCount10 == 3) ut.Assert(bp10.Low() == 19116.84, __FILE__,__FUNCTION__,__LINE__);
    if (iCount10 == 4) ut.Assert(bp10.Close() == 19209.30, __FILE__,__FUNCTION__,__LINE__);
    // if (iCount10 == 4) ut.Assert(abs(bp10.Return()-19209.3/19145.07) < 0.0001, __FILE__,__FUNCTION__,__LINE__);
    if (iCount10 == 7) ut.Assert(bp10.GetYYYYMMDD() == YYYYMMDD(20120911), __FILE__,__FUNCTION__,__LINE__);
    if (iCount10 == 8) ut.Assert(bp10.GetYYYYMMDD() == YYYYMMDD(20120912), __FILE__,__FUNCTION__,__LINE__);
    if (iCount10 == 9) ut.Assert(bp10.PeekPrevYMD() == YYYYMMDD(20120912), __FILE__,__FUNCTION__,__LINE__);
    iCount10++;

    if (iCount10 > 2)
    {
      if (iCount11 == 1) ut.Assert(bp11.PeekNextYMD() == YYYYMMDD(20120903), __FILE__,__FUNCTION__,__LINE__);
      bp11.MoveNext();
      if (iCount11 == 1) ut.Assert(bp11.Volume() == 5670, __FILE__,__FUNCTION__,__LINE__);
      if (iCount11 == 2) ut.Assert(bp11.PeekPrevYMD() == YYYYMMDD(20120903), __FILE__,__FUNCTION__,__LINE__);
      if (iCount11 == 2) ut.Assert(bp11.PeekPrevClose() == 19559.21, __FILE__,__FUNCTION__,__LINE__);
      if (iCount11 == 2) ut.Assert(bp11.High() == 19572.31, __FILE__,__FUNCTION__,__LINE__);
      if (iCount11 == 3) ut.Assert(bp11.Low() == 19116.84, __FILE__,__FUNCTION__,__LINE__);
      if (iCount11 == 4) ut.Assert(bp11.Close() == 19209.30, __FILE__,__FUNCTION__,__LINE__);
      // if (iCount11 == 4) ut.Assert(abs(bp11.Return()-19209.3/19145.07) < 0.0001, __FILE__,__FUNCTION__,__LINE__);
      if (iCount11 == 7) ut.Assert(bp11.GetYYYYMMDD() == YYYYMMDD(20120911), __FILE__,__FUNCTION__,__LINE__);
      if (iCount11 == 8) ut.Assert(bp11.GetYYYYMMDD() == YYYYMMDD(20120912), __FILE__,__FUNCTION__,__LINE__);
      if (iCount11 == 9) ut.Assert(bp11.PeekPrevYMD() == YYYYMMDD(20120912), __FILE__,__FUNCTION__,__LINE__);
      iCount11++;
    }

    if (iCount10 > 3)
    {
      if (iCount12 == 1) ut.Assert(bp12.PeekNextYMD() == YYYYMMDD(20120903), __FILE__,__FUNCTION__,__LINE__);
      bp12.MoveNext();
      if (iCount12 == 1) ut.Assert(bp12.Volume() == 5670, __FILE__,__FUNCTION__,__LINE__);
      if (iCount12 == 2) ut.Assert(bp12.PeekPrevYMD() == YYYYMMDD(20120903), __FILE__,__FUNCTION__,__LINE__);
      if (iCount12 == 2) ut.Assert(bp12.PeekPrevClose() == 19559.21, __FILE__,__FUNCTION__,__LINE__);
      if (iCount12 == 2) ut.Assert(bp12.High() == 19572.31, __FILE__,__FUNCTION__,__LINE__);
      if (iCount12 == 3) ut.Assert(bp12.Low() == 19116.84, __FILE__,__FUNCTION__,__LINE__);
      if (iCount12 == 4) ut.Assert(bp12.Close() == 19209.30, __FILE__,__FUNCTION__,__LINE__);
      // if (iCount12 == 4) ut.Assert(abs(bp12.Return()-19209.3/19145.07) < 0.0001, __FILE__,__FUNCTION__,__LINE__);
      if (iCount12 == 7) ut.Assert(bp12.GetYYYYMMDD() == YYYYMMDD(20120911), __FILE__,__FUNCTION__,__LINE__);
      if (iCount12 == 8) ut.Assert(bp12.GetYYYYMMDD() == YYYYMMDD(20120912), __FILE__,__FUNCTION__,__LINE__);
      if (iCount12 == 9) ut.Assert(bp12.PeekPrevYMD() == YYYYMMDD(20120912), __FILE__,__FUNCTION__,__LINE__);
      iCount12++;
    }
  }

  ut.Assert(bp10.GetNumOfBars() == 57, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(bp11.GetNumOfBars() == 57, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(bp12.GetNumOfBars() == 57, __FILE__,__FUNCTION__,__LINE__);


  //--------------------------------------------------
  BarProvider bp13("./UnitTest/test-plain.csv", "DOHLCV", 2, 'F', 'N', 'P');
  bp13.ResetPsn();

  bp13.AddBar(YYYYMMDD(20130102), 9394, 9395, 9396, 9397, 9398);
  bp13.AddBar(YYYYMMDD(20130103), 9395, 9396, 9397, 9398, 9399);
  bp13.AddBar(YYYYMMDD(20130103), 9396, 9397, 9398, 9399, 9400);


  ut.Assert(bp13.GetNumOfBars() == 60, __FILE__,__FUNCTION__,__LINE__);

  while (bp13.HasNextBar())
  {
    bp13.MoveNext();
  }

  ut.Assert(bp13.Open()   == 9396, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(bp13.High()   == 9397, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(bp13.Low()    == 9398, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(bp13.Close()  == 9399, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(bp13.Volume() == 9400, __FILE__,__FUNCTION__,__LINE__);

  ut.PrintResult();

  return 0;
}
