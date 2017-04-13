/*
 * TestOrderBookOMDDSP.cpp
 *
 *  Created on: Jul 15, 2014
 *      Author: sunny
 */

#include "TestOrderBookOMDDSP.h"

TestOrderBookOMDDSP::TestOrderBookOMDDSP() {
  // TODO Auto-generated constructor stub

}

TestOrderBookOMDDSP::~TestOrderBookOMDDSP() {
  // TODO Auto-generated destructor stub
}

bool TestOrderBookOMDDSP::Check(
    int iaBid[12][3],
    int iaAsk[12][3],
    int iaBidAns[12][3],
    int iaAskAns[12][3])
{
  bool bRtn = true;

  for (int i = 0; i < 12; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      if (iaBid[i][j] != iaBidAns[i][j]) bRtn = false;
      if (iaAsk[i][j] != iaAskAns[i][j]) bRtn = false;
    }
  }
  return bRtn;
}

void TestOrderBookOMDDSP::RunTest()
{

  int iaBid[12][3];
  int iaAsk[12][3];
  OrderBookOMDDSP osob;

  // -------------------------------------
  // LocatePriceLev: Empty queues
  // -------------------------------------
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9999) == -1);

  // -------------------------------------
  // Setting up the Initial Order Book (P.45/56)
  // -------------------------------------
  osob.Reset();
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9730, 1, 1, 700));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9720, 2, 1, 350));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9710, 3, 1, 150));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9700, 4, 1, 250));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9690, 5, 1, 100));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9680, 6, 1, 150));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9670, 7, 1,  50));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9660, 8, 1, 200));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9650, 9, 1, 100));

  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9760, 1, 1, 500));
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9770, 2, 1, 300));
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9780, 3, 1, 100));
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9790, 4, 1, 150));

  osob.Dump(OrderBook::BID,iaBid);
  osob.Dump(OrderBook::ASK,iaAsk);

  int iaBid1 [12][3] = {
    {0,  0,   0}
    ,{1,700,9730}
    ,{1,350,9720}
    ,{1,150,9710}
    ,{1,250,9700}
    ,{1,100,9690}
    ,{1,150,9680}
    ,{1, 50,9670}
    ,{1,200,9660}
    ,{1,100,9650}
    ,{0,  0,   0}
  };
  int iaAsk1 [12][3] = {
    {0,  0,   0}
    ,{1,500,9760}
    ,{1,300,9770}
    ,{1,100,9780}
    ,{1,150,9790}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
  };

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid1, iaAsk1));


  // -------------------------------------
  // LocatePriceLev:
  // -------------------------------------
  CPPUNIT_ASSERT(osob.LocatePriceLev(OrderBook::BID, 9710) == 3);


  // -------------------------------------
  // Example 1 P.46/56
  // -------------------------------------
  CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, 9770, 2, 1, 200));
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9850, 5, 1, 300));

  osob.Dump(OrderBook::BID,iaBid);
  osob.Dump(OrderBook::ASK,iaAsk);

  int iaBid2 [12][3] = {
    {0,  0,   0}
    ,{1,700,9730}
    ,{1,350,9720}
    ,{1,150,9710}
    ,{1,250,9700}
    ,{1,100,9690}
    ,{1,150,9680}
    ,{1, 50,9670}
    ,{1,200,9660}
    ,{1,100,9650}
    ,{0,  0,   0}
    ,{0,  0,   0}
  };
  int iaAsk2 [12][3] = {
    {0,  0,   0}
    ,{1,500,9760}
    ,{1,200,9770}
    ,{1,100,9780}
    ,{1,150,9790}
    ,{1,300,9850} // We won't store empty rows
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
  };

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid2, iaAsk2));


  // -------------------------------------
  // Example 2 P.46/56
  // -------------------------------------
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9740, 1, 1, 50));

  osob.Dump(OrderBook::BID,iaBid);
  osob.Dump(OrderBook::ASK,iaAsk);

  int iaBid3 [12][3] = {
    {0,  0,   0}
    ,{1, 50,9740}
    ,{1,700,9730}
    ,{1,350,9720}
    ,{1,150,9710}
    ,{1,250,9700}
    ,{1,100,9690}
    ,{1,150,9680}
    ,{1, 50,9670}
    ,{1,200,9660}
    ,{1,100,9650}
    ,{0,  0,   0}
  };
  int iaAsk3 [12][3] = {
    {0,  0,   0}
    ,{1,500,9760}
    ,{1,200,9770}
    ,{1,100,9780}
    ,{1,150,9790}
    ,{1,300,9850} // We won't store empty rows
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
  };

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid3, iaAsk3));


  // -------------------------------------
  // Example 3 P.47/56
  // -------------------------------------
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9750, 1, 1, 250));
  CPPUNIT_ASSERT(osob.Change(OrderBook::BID, 9660, 10, 1, 150));

  //osob.Dump(OrderBook::BID);
  //osob.Dump(OrderBook::ASK);

  osob.Dump(OrderBook::BID,iaBid);
  osob.Dump(OrderBook::ASK,iaAsk);

  int iaBid4 [12][3] = {
    {0,  0,   0}
    ,{1,250,9750}
    ,{1, 50,9740}
    ,{1,700,9730}
    ,{1,350,9720}
    ,{1,150,9710}
    ,{1,250,9700}
    ,{1,100,9690}
    ,{1,150,9680}
    ,{1, 50,9670}
    ,{1,150,9660}
    ,{0,  0,   0}
  };
  int iaAsk4 [12][3] = {
    {0,  0,   0}
    ,{1,500,9760}
    ,{1,200,9770}
    ,{1,100,9780}
    ,{1,150,9790}
    ,{1,300,9850} // We won't store empty rows
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
  };

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid4, iaAsk4));


  // -------------------------------------
  // Example 4 P.48/56
  // -------------------------------------
  CPPUNIT_ASSERT(osob.Delete(OrderBook::BID, 9750, 1, 1, 250));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9650, 10, 1, 100));

  osob.Dump(OrderBook::BID,iaBid);
  osob.Dump(OrderBook::ASK,iaAsk);

  int iaBid5 [12][3] = {
    {0,  0,   0}
    ,{1, 50,9740}
    ,{1,700,9730}
    ,{1,350,9720}
    ,{1,150,9710}
    ,{1,250,9700}
    ,{1,100,9690}
    ,{1,150,9680}
    ,{1, 50,9670}
    ,{1,150,9660}
    ,{1,100,9650}
    ,{0,  0,   0}
  };
  int iaAsk5 [12][3] = {
    {0,  0,   0}
    ,{1,500,9760}
    ,{1,200,9770}
    ,{1,100,9780}
    ,{1,150,9790}
    ,{1,300,9850} // We won't store empty rows
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
  };

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid5, iaAsk5));

  // -------------------------------------
  // Example 5 P.49/56 (Aggregated 11th level)
  // -------------------------------------
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, -1, 255, 1, 200));

  osob.Dump(OrderBook::BID,iaBid);
  osob.Dump(OrderBook::ASK,iaAsk);

  int iaBid6 [12][3] = {
    {0,  0,   0}
    ,{1, 50,9740}
    ,{1,700,9730}
    ,{1,350,9720}
    ,{1,150,9710}
    ,{1,250,9700}
    ,{1,100,9690}
    ,{1,150,9680}
    ,{1, 50,9670}
    ,{1,150,9660}
    ,{1,100,9650}
    ,{1,200,   0}
  };
  int iaAsk6 [12][3] = {
    {0,  0,   0}
    ,{1,500,9760}
    ,{1,200,9770}
    ,{1,100,9780}
    ,{1,150,9790}
    ,{1,300,9850} // We won't store empty rows
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
    ,{0,  0,   0}
  };

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid6, iaAsk6));

  // -------------------------------------
  // Example 6 Market Orders (P.50/56)
  // -------------------------------------
  osob.Reset();

  // Build initial order book
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9710, 1, 1, 7700));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9700, 2, 1, 6800));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9690, 3, 1, 2000));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9680, 4, 1,  200));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9650, 5, 1, 1000));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9640, 6, 1, 2500));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9620, 7, 1, 1000));
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9600, 8, 1, 1000));

  // Build initial order book
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9720, 1, 1, 8200));
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9730, 2, 1, 2000));
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9740, 3, 1, 1000));
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9750, 4, 1, 1500));
  CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9860, 5, 1, 8000));

  // Market Order
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, -1, 1, 1, 7900));

  osob.Dump(OrderBook::BID,iaBid);
  osob.Dump(OrderBook::ASK,iaAsk);

  int iaBid7 [12][3] = {
    {1,7900,   0}
    ,{1,7700,9710}
    ,{1,6800,9700}
    ,{1,2000,9690}
    ,{1, 200,9680}
    ,{1,1000,9650}
    ,{1,2500,9640}
    ,{1,1000,9620}
    ,{1,1000,9600}
    ,{0,   0,   0}
    ,{0,   0,   0}
    ,{0,   0,   0}
  };
  int iaAsk7 [12][3] = {
    {0,   0,   0}
    ,{1,8200,9720}
    ,{1,2000,9730}
    ,{1,1000,9740}
    ,{1,1500,9750}
    ,{1,8000,9860}
    ,{0,   0,   0}
    ,{0,   0,   0}
    ,{0,   0,   0}
    ,{0,   0,   0}
    ,{0,   0,   0}
    ,{0,   0,   0}
  };

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid7, iaAsk7));


  // -------------------------------------
  // Example 7 COP Merge (P.50/56)
  // -------------------------------------
  CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9720, 1, 1, 1000));

  osob.Dump(OrderBook::BID,iaBid);
  osob.Dump(OrderBook::ASK,iaAsk);

  int iaBid8 [12][3] = {
    {1,7900,   0}
    ,{1,1000,9720}
    ,{1,7700,9710}
    ,{1,6800,9700}
    ,{1,2000,9690}
    ,{1, 200,9680}
    ,{1,1000,9650}
    ,{1,2500,9640}
    ,{1,1000,9620}
    ,{1,1000,9600}
    ,{0,   0,   0}
    ,{0,   0,   0}
  };
  int iaAsk8 [12][3] = {
    {0,   0,   0}
    ,{1,8200,9720}
    ,{1,2000,9730}
    ,{1,1000,9740}
    ,{1,1500,9750}
    ,{1,8000,9860}
    ,{0,   0,   0}
    ,{0,   0,   0}
    ,{0,   0,   0}
    ,{0,   0,   0}
    ,{0,   0,   0}
    ,{0,   0,   0}
  };

  CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid8, iaAsk8));



  {
    // -------------------------------------
    // My Own Test Cases
    // Market Order
    // Focusing on tesing ASK
    // -------------------------------------
    osob.Reset();

    // Build initial order book
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9710, 1, 1, 7700));
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9700, 2, 1, 6800));
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9690, 3, 1, 2000));
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9680, 4, 1,  200));
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9650, 5, 1, 1000));
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9640, 6, 1, 2500));
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9620, 7, 1, 1000));
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9600, 8, 1, 1000));

    // Build initial order book
    CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9720, 1, 1, 8200));
    CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9730, 2, 1, 2000));
    CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9740, 3, 1, 1000));
    CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9750, 4, 1, 1500));
    CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9860, 5, 1, 8000));

    // Add Market Order
    CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, -1, 1, 2, 9394));

    osob.Dump(OrderBook::BID,iaBid);
    osob.Dump(OrderBook::ASK,iaAsk);

    int iaBid9 [12][3] = {
      {0,   0,   0}
      ,{1,7700,9710}
      ,{1,6800,9700}
      ,{1,2000,9690}
      ,{1, 200,9680}
      ,{1,1000,9650}
      ,{1,2500,9640}
      ,{1,1000,9620}
      ,{1,1000,9600}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };
    int iaAsk9 [12][3] = {
      {2,9394,   0}
      ,{1,8200,9720}
      ,{1,2000,9730}
      ,{1,1000,9740}
      ,{1,1500,9750}
      ,{1,8000,9860}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };

    CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid9, iaAsk9));

    // Add Market Order again
    CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, -1, 1, 3, 9413));

    osob.Dump(OrderBook::BID,iaBid);
    osob.Dump(OrderBook::ASK,iaAsk);

    int iaBid10 [12][3] = {
      {0,   0,   0}
      ,{1,7700,9710}
      ,{1,6800,9700}
      ,{1,2000,9690}
      ,{1, 200,9680}
      ,{1,1000,9650}
      ,{1,2500,9640}
      ,{1,1000,9620}
      ,{1,1000,9600}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };
    int iaAsk10 [12][3] = {
      {3,9413,   0}
      ,{1,8200,9720}
      ,{1,2000,9730}
      ,{1,1000,9740}
      ,{1,1500,9750}
      ,{1,8000,9860}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };

    CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid10, iaAsk10));

    // Change Market Order
    CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, -1, 1, 9, 1516));

    osob.Dump(OrderBook::BID,iaBid);
    osob.Dump(OrderBook::ASK,iaAsk);

    int iaBid11 [12][3] = {
      {0,   0,   0}
      ,{1,7700,9710}
      ,{1,6800,9700}
      ,{1,2000,9690}
      ,{1, 200,9680}
      ,{1,1000,9650}
      ,{1,2500,9640}
      ,{1,1000,9620}
      ,{1,1000,9600}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };
    int iaAsk11 [12][3] = {
      {9,1516,   0}
      ,{1,8200,9720}
      ,{1,2000,9730}
      ,{1,1000,9740}
      ,{1,1500,9750}
      ,{1,8000,9860}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };

    CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid11, iaAsk11));

    // Change Market Order again
    CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, -1, 1, 5, 1314));

    osob.Dump(OrderBook::BID,iaBid);
    osob.Dump(OrderBook::ASK,iaAsk);

    int iaBid12 [12][3] = {
      {0,   0,   0}
      ,{1,7700,9710}
      ,{1,6800,9700}
      ,{1,2000,9690}
      ,{1, 200,9680}
      ,{1,1000,9650}
      ,{1,2500,9640}
      ,{1,1000,9620}
      ,{1,1000,9600}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };
    int iaAsk12 [12][3] = {
      {5,1314,   0}
      ,{1,8200,9720}
      ,{1,2000,9730}
      ,{1,1000,9740}
      ,{1,1500,9750}
      ,{1,8000,9860}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };

    CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid12, iaAsk12));

    // Delete Market Order
    CPPUNIT_ASSERT(osob.Delete(OrderBook::ASK, -1, 1, 5, 1314));

    osob.Dump(OrderBook::BID,iaBid);
    osob.Dump(OrderBook::ASK,iaAsk);

    int iaBid13 [12][3] = {
      {0,   0,   0}
      ,{1,7700,9710}
      ,{1,6800,9700}
      ,{1,2000,9690}
      ,{1, 200,9680}
      ,{1,1000,9650}
      ,{1,2500,9640}
      ,{1,1000,9620}
      ,{1,1000,9600}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };
    int iaAsk13 [12][3] = {
      {0,   0,   0}
      ,{1,8200,9720}
      ,{1,2000,9730}
      ,{1,1000,9740}
      ,{1,1500,9750}
      ,{1,8000,9860}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };

    CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid13, iaAsk13));

    // Delete Market Order again
    CPPUNIT_ASSERT(osob.Delete(OrderBook::ASK, -1, 1, 0, 5354));

    osob.Dump(OrderBook::BID,iaBid);
    osob.Dump(OrderBook::ASK,iaAsk);

    int iaBid14 [12][3] = {
      {0,   0,   0}
      ,{1,7700,9710}
      ,{1,6800,9700}
      ,{1,2000,9690}
      ,{1, 200,9680}
      ,{1,1000,9650}
      ,{1,2500,9640}
      ,{1,1000,9620}
      ,{1,1000,9600}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };
    int iaAsk14 [12][3] = {
      {0,   0,   0}
      ,{1,8200,9720}
      ,{1,2000,9730}
      ,{1,1000,9740}
      ,{1,1500,9750}
      ,{1,8000,9860}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };

    CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid14, iaAsk14));

    // Add Market Order (with Change)
    CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, -1, 1, 4, 1919));

    osob.Dump(OrderBook::BID,iaBid);
    osob.Dump(OrderBook::ASK,iaAsk);

    int iaBid15 [12][3] = {
      {0,   0,   0}
      ,{1,7700,9710}
      ,{1,6800,9700}
      ,{1,2000,9690}
      ,{1, 200,9680}
      ,{1,1000,9650}
      ,{1,2500,9640}
      ,{1,1000,9620}
      ,{1,1000,9600}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };
    int iaAsk15 [12][3] = {
      {4,1919,   0}
      ,{1,8200,9720}
      ,{1,2000,9730}
      ,{1,1000,9740}
      ,{1,1500,9750}
      ,{1,8000,9860}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };

    CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid15, iaAsk15));
  }

  {
    // -------------------------------------
    // My Own Test Cases
    // Aggregated 11th level
    // Focusing on tesing ASK
    // -------------------------------------
    osob.Reset();

    // Build initial order book
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9710, 1, 1, 7700));
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9700, 2, 1, 6800));
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9690, 3, 1, 2000));
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9680, 4, 1,  200));
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9650, 5, 1, 1000));
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9640, 6, 1, 2500));
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9620, 7, 1, 1000));
    CPPUNIT_ASSERT(osob.Add(OrderBook::BID, 9600, 8, 1, 1000));

    // Build initial order book
    CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9720, 1, 1, 8200));
    CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9730, 2, 1, 2000));
    CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9740, 3, 1, 1000));
    CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9750, 4, 1, 1500));
    CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, 9860, 5, 1, 8000));

    // Add Aggregated 11th level
    CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, -1, 255, 2, 9394));

    osob.Dump(OrderBook::BID,iaBid);
    osob.Dump(OrderBook::ASK,iaAsk);

    int iaBid9 [12][3] = {
      {0,   0,   0}
      ,{1,7700,9710}
      ,{1,6800,9700}
      ,{1,2000,9690}
      ,{1, 200,9680}
      ,{1,1000,9650}
      ,{1,2500,9640}
      ,{1,1000,9620}
      ,{1,1000,9600}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };
    int iaAsk9 [12][3] = {
      {0,   0,   0}
      ,{1,8200,9720}
      ,{1,2000,9730}
      ,{1,1000,9740}
      ,{1,1500,9750}
      ,{1,8000,9860}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{2,9394,   0}
    };

    CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid9, iaAsk9));

    // Add Aggregated 11th level again
    CPPUNIT_ASSERT(osob.Add(OrderBook::ASK, -1, 255, 3, 9413));

    osob.Dump(OrderBook::BID,iaBid);
    osob.Dump(OrderBook::ASK,iaAsk);

    int iaBid10 [12][3] = {
      {0,   0,   0}
      ,{1,7700,9710}
      ,{1,6800,9700}
      ,{1,2000,9690}
      ,{1, 200,9680}
      ,{1,1000,9650}
      ,{1,2500,9640}
      ,{1,1000,9620}
      ,{1,1000,9600}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };
    int iaAsk10 [12][3] = {
      {0,   0,   0}
      ,{1,8200,9720}
      ,{1,2000,9730}
      ,{1,1000,9740}
      ,{1,1500,9750}
      ,{1,8000,9860}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{3,9413,   0}
    };

    CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid10, iaAsk10));

    // Change Aggregated 11th level
    CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, -1, 255, 9, 1516));

    osob.Dump(OrderBook::BID,iaBid);
    osob.Dump(OrderBook::ASK,iaAsk);

    int iaBid11 [12][3] = {
      {0,   0,   0}
      ,{1,7700,9710}
      ,{1,6800,9700}
      ,{1,2000,9690}
      ,{1, 200,9680}
      ,{1,1000,9650}
      ,{1,2500,9640}
      ,{1,1000,9620}
      ,{1,1000,9600}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };
    int iaAsk11 [12][3] = {
      {0,   0,   0}
      ,{1,8200,9720}
      ,{1,2000,9730}
      ,{1,1000,9740}
      ,{1,1500,9750}
      ,{1,8000,9860}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{9,1516,   0}
    };

    CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid11, iaAsk11));

    // Change Aggregated 11th level again
    CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, -1, 255, 5, 1314));

    osob.Dump(OrderBook::BID,iaBid);
    osob.Dump(OrderBook::ASK,iaAsk);

    int iaBid12 [12][3] = {
      {0,   0,   0}
      ,{1,7700,9710}
      ,{1,6800,9700}
      ,{1,2000,9690}
      ,{1, 200,9680}
      ,{1,1000,9650}
      ,{1,2500,9640}
      ,{1,1000,9620}
      ,{1,1000,9600}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };
    int iaAsk12 [12][3] = {
      {0,   0,   0}
      ,{1,8200,9720}
      ,{1,2000,9730}
      ,{1,1000,9740}
      ,{1,1500,9750}
      ,{1,8000,9860}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{5,1314,   0}
    };

    CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid12, iaAsk12));

    // Delete Aggregated 11th level
    CPPUNIT_ASSERT(osob.Delete(OrderBook::ASK, -1, 255, 5, 1314));

    osob.Dump(OrderBook::BID,iaBid);
    osob.Dump(OrderBook::ASK,iaAsk);

    int iaBid13 [12][3] = {
      {0,   0,   0}
      ,{1,7700,9710}
      ,{1,6800,9700}
      ,{1,2000,9690}
      ,{1, 200,9680}
      ,{1,1000,9650}
      ,{1,2500,9640}
      ,{1,1000,9620}
      ,{1,1000,9600}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };
    int iaAsk13 [12][3] = {
      {0,   0,   0}
      ,{1,8200,9720}
      ,{1,2000,9730}
      ,{1,1000,9740}
      ,{1,1500,9750}
      ,{1,8000,9860}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };

    CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid13, iaAsk13));

    // Delete Aggregated 11th level again
    CPPUNIT_ASSERT(osob.Delete(OrderBook::ASK, -1, 255, 0, 5354));

    osob.Dump(OrderBook::BID,iaBid);
    osob.Dump(OrderBook::ASK,iaAsk);

    int iaBid14 [12][3] = {
      {0,   0,   0}
      ,{1,7700,9710}
      ,{1,6800,9700}
      ,{1,2000,9690}
      ,{1, 200,9680}
      ,{1,1000,9650}
      ,{1,2500,9640}
      ,{1,1000,9620}
      ,{1,1000,9600}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };
    int iaAsk14 [12][3] = {
      {0,   0,   0}
      ,{1,8200,9720}
      ,{1,2000,9730}
      ,{1,1000,9740}
      ,{1,1500,9750}
      ,{1,8000,9860}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };

    CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid14, iaAsk14));

    // Add Aggregated 11th level (with Change)
    CPPUNIT_ASSERT(osob.Change(OrderBook::ASK, -1, 255, 4, 1919));

    osob.Dump(OrderBook::BID,iaBid);
    osob.Dump(OrderBook::ASK,iaAsk);

    int iaBid15 [12][3] = {
      {0,   0,   0}
      ,{1,7700,9710}
      ,{1,6800,9700}
      ,{1,2000,9690}
      ,{1, 200,9680}
      ,{1,1000,9650}
      ,{1,2500,9640}
      ,{1,1000,9620}
      ,{1,1000,9600}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
    };
    int iaAsk15 [12][3] = {
      {0,   0,   0}
      ,{1,8200,9720}
      ,{1,2000,9730}
      ,{1,1000,9740}
      ,{1,1500,9750}
      ,{1,8000,9860}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{0,   0,   0}
      ,{4,1919,   0}
    };

    CPPUNIT_ASSERT(Check(iaBid, iaAsk, iaBid15, iaAsk15));
  }



  return;
}

void TestOrderBookOMDDSP::setUp()
{
  return;
}


void TestOrderBookOMDDSP::tearDown()
{
  return;
}

CppUnit::Test *TestOrderBookOMDDSP::suite()
{
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("TestOrderBookOMDDSP");

  //--------------------------------------------------
  // Test triggering RTSClient (Config-RTS)
  //--------------------------------------------------
  suiteOfTests->addTest(new CppUnit::TestCaller<TestOrderBookOMDDSP>(
        "TestOrderBookOMDDSP", 
        &TestOrderBookOMDDSP::RunTest));

  return suiteOfTests;
}
