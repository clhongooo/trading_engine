#include <iostream>
#include <boost/lexical_cast.hpp>

#include "Cash/ToBBAT.h"
#include "Cash/EnsureIncTime.h"
#include "Cash/MarketFeedToOHLC.h"
#include "Cash/DailyOHLCToMarketFeed.h"
#include "Cash/MinuteOHLCToMarketFeed.h"
#include "Cash/MarketFeedMerger.h"
#include "Cash/MarketFeedCondensor.h"
// #include "Nirvana/HSIContFut.h"
#include "Nirvana/SubInterval.h"

using namespace std;
using namespace boost;

//--------------------------------------------------
// argv:
// 1:    Type
//       1:
//           From       091623,HSIV3,23294,1022,B,23293,2,23292,1,23291,5,23290,4,23289,2,A,23295,1,23296,2,23297,3,23298,6,23299,5
//                      091623,HSIV3,23292,1023,B,23291,2,23290,1,23289,5,23288,4,23287,2,A,23293,1,23294,2,23295,3,23296,6,23297,5
//                      091623,HSIV3,23294,1024,B,23293,2,23292,1,23291,5,23290,4,23289,2,A,23295,1,23296,2,23297,3,23298,6,23299,5
//                      091623,HSIV3,23294,1028,B,23293,2,23292,1,23291,5,23290,4,23289,2,A,23295,1,23296,2,23297,3,23298,6,23299,5
//                      091623,HSIV3,23294,1034,B,23293,2,23292,1,23291,5,23290,4,23289,2,A,23295,1,23296,2,23297,3,23298,6,23299,5
//           To
//                      HSI,91400       B: 23298 (2)    A: 23300 (1)    T: 23299 (16)
//                      HSI,91400       B: 23298 (2)    A: 23300 (1)    T: 23299 (1)
//                      HSI,91400       B: 23298 (2)    A: 23300 (1)    T: 23299 (1)
//                      HSI,91400       B: 23298 (2)    A: 23300 (1)    T: 23299 (47)
//                      HSI,91400       B: 23298 (2)    A: 23300 (1)    T: 23299 (2)
//                      HSI,91400       B: 23298 (2)    A: 23300 (1)    T: 23299 (1)
//                      HSI,91400       B: 23298 (2)    A: 23300 (1)    T: 23299 (76)
//                      HSI,91400       B: 23298 (2)    A: 23300 (1)    T: 23299 (2)
//                      HSI,91400       B: 23298 (2)    A: 23300 (1)    T: 23299 (2)
//
//           argv 2:   Input file
//           argv 3:   Output file
//           argv 4:   Symbol
//
//       2:
//           Ensure increasing time. Some of Cash's ebroker data have messed up time
//           Sample:
//                  091623,HSIV3,23294,1022,B,23293,2,23292,1,23291,5,23290,4,23289,2,A,23295,1,23296,2,23297,3,23298,6,23299,5
//                  091623,HSIV3,23292,1023,B,23291,2,23290,1,23289,5,23288,4,23287,2,A,23293,1,23294,2,23295,3,23296,6,23297,5
//                  091623,HSIV3,23294,1024,B,23293,2,23292,1,23291,5,23290,4,23289,2,A,23295,1,23296,2,23297,3,23298,6,23299,5
//                  091623,HSIV3,23294,1028,B,23293,2,23292,1,23291,5,23290,4,23289,2,A,23295,1,23296,2,23297,3,23298,6,23299,5
//                  091623,HSIV3,23294,1034,B,23293,2,23292,1,23291,5,23290,4,23289,2,A,23295,1,23296,2,23297,3,23298,6,23299,5
//
//           argv 2:   Input file
//           argv 3:   Output file
//
//       3:
//           From market feed to OHLC, considering trade price only
//           From
//                      094504,HSIJ0,21307,225
//                      094504,HSIJ0,21306,226
//                      094504,HSIJ0,21306,227
//                      094504,HSIJ0,21307,263
//                      094504,HSIJ0,21306,264
//                      094504,HSIJ0,21307,265
//                      094504,HSIJ0,21306,266
//                      094504,HSIJ0,21306,267
//                      094504,HSIJ0,21308,268
//                      094504,HSIJ0,21307,269
//                      094504,HSIJ0,21310,271
//           To
//                      09:45,Open,High,Low,Close,Volume
//
//           argv 2:   Input file
//           argv 3:   Output file
//           argv 4:   Volume in cumulative mode or discrete model, C / D
//
//       4:
//           Clean time-messed up data:
//           From
//                      094504,HSIJ0,21307,1
//                      094504,HSIJ0,21306,2
//                      094504,HSIJ0,21306,3
//                      094504,HSIJ0,21307,4
//                      094504,HSIJ0,21306,5
//                      094504,HSIJ0,21307,6
//                      094504,HSIJ0,21306,7
//                      094504,HSIJ0,21306,8
//                      094504,HSIJ0,21308,9
//                      094504,HSIJ0,21307,10
//           To
//                      094504,HSIJ0,21307,1
//                      094504,HSIJ0,21306,3
//                      094504,HSIJ0,21306,5
//                      094504,HSIJ0,21306,7
//                      094504,HSIJ0,21308,9
//
//           argv 2:   Input file
//           argv 3:   Output file
//           argv 4:   Interval (=2)
//
//       333 (deprecated):
//           Create HSI continuous futures from raw files
//
//           argv 2:   Source directory
//           argv 3:   Output file
//           argv 4:   From date   YYYYMMDD
//           argv 5:   To date     YYYYMMDD
//           argv 6:   Symbol      e.g. HI / HC
//
//       234:
//           Market feed merger
//           For the same timestamp, will always output all of file 1 before file 2
//
//           argv 2:   Input file1
//           argv 3:   Input file2
//           argv 4:   Output file
//
//       235:
//           Market feed condensor from tick to 5 second snapshots
//           argv 2:   Input file
//           argv 3:   Output file
//           argv 4:   Snapshot per N seconds
//
//       236:
//           From Daily OHLC Bar to market feed
//           argv 2:   Input file
//           argv 3:   Output file
//           argv 4:   Symbol
//           argv 5:   HHMMSS for Open price
//           argv 6:   HHMMSS for High price
//           argv 7:   HHMMSS for Low price
//           argv 8:   HHMMSS for Close price
//           argv 9:   BarProvider Column format
//
//       237:
//           From Minutely OHLC Bar to market feed
//           argv 2:   Input file
//           argv 3:   Output file
//           argv 4:   Symbol or the column num in the OHLC bar containing the symbol
//           argv 5:   Trade Only
//           argv 6:   The column num containing date
//           argv 7:   The column num containing time
//           argv 8:   The column num containing close price
//           argv 9:   Output Close price only
//
//--------------------------------------------------

int main ( int argc, const char* argv[] )
{

  int iType = lexical_cast<unsigned short>(argv[1]);

  switch(iType)
  {
    case 1:
      {
        ToBBAT bbat;
        bbat.Convert(argv[2],argv[3],argv[4]);
        break;
      }
    case 2:
      {
        EnsureIncTime eit;
        eit.Convert(argv[2],argv[3]);
        break;
      }
    case 3:
      {
        MarketFeedToOHLC mftohlc;
        mftohlc.Convert(argv[2],argv[3],argv[4]);
        break;
      }
    case 4:
      {
        SubInterval sitvl;
        sitvl.Convert(argv[2],argv[3],argv[4]);
        break;
      }
    case 234:
      {
        MarketFeedMerger mfm;
        mfm.Merge(argv[2],argv[3],argv[4]);
        break;
      }
    case 235:
      {
        MarketFeedCondensor mfc;
        mfc.Condense(argv[2],argv[3],argv[4]);
        break;
      }
    case 236:
      {
        DailyOHLCToMarketFeed dohlcmf;
        dohlcmf.Convert(argv[2],argv[3],argv[4],
            argv[5],
            argv[6],
            argv[7],
            argv[8],
            argv[9]
            );
        break;
      }
    case 237:
      {
        MinuteOHLCToMarketFeed mohlcmf;
        mohlcmf.Convert(argv[2],argv[3],argv[4],argv[5],argv[6],argv[7],argv[8],argv[9],argv[10]);
        break;
      }
    // case 333:
    //   {
    //     HSIContFut hsicontfut;
    //     hsicontfut.Create(argv[2],argv[3],argv[4],argv[5],argv[6]);
    //     break;
    //   }
    default:
      {
        break;
      }
  }

return 0;
}
