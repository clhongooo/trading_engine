#include "UTest.h"
#include "Util/HKFE.h"
#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>

int TestHKFE()
{
  UTest ut;

  shared_ptr<HKFE> m_HKFE = HKFE::Instance();
  m_HKFE->LoadCalendar("/home/qy/Dropbox/nirvana/conf/HKFE_Calendar.csv");

  ut.Assert(m_HKFE->FutCode(2005,1).compare("F05") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->FutCode(2000,10).compare("V00") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->FutCode(2012,5).compare("K12") == 0,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetSymbol('F',2012,1).compare("F-2012-01") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetSymbol('C',24000,2013,9).compare("C-24000-2013-09") == 0,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetSymbol('P',8000,1990,12).compare("P-08000-1990-12") == 0,__FILE__,__FUNCTION__,__LINE__);


  ut.Assert(m_HKFE->GetStrike("C-21400-2012-10") == 21400,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetStrike("C-09000-2012-10") == 9000,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetStrike("C-38000-2012-10") == 38000,__FILE__,__FUNCTION__,__LINE__);


  // ut.Assert(m_HKFE->VerbatimYMi("C-18600-2011-02") == 201102,__FILE__,__FUNCTION__,__LINE__);
  // ut.Assert(m_HKFE->VerbatimYMi("P-28600-2015-11") == 201511,__FILE__,__FUNCTION__,__LINE__);
  // ut.Assert(m_HKFE->VerbatimYMi("F-2009-08") == 200908,__FILE__,__FUNCTION__,__LINE__);
  // ut.Assert(m_HKFE->VerbatimYMi("F-2002-12") == 200212,__FILE__,__FUNCTION__,__LINE__);


  ut.Assert(m_HKFE->IsCall("C-21400-2012-10"),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsPut("C-21400-2012-10"),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsFut("C-21400-2012-10"),__FILE__,__FUNCTION__,__LINE__);

  ut.AssertF(m_HKFE->IsCall("P-21400-2012-10"),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsPut("P-21400-2012-10"),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsFut("P-21400-2012-10"),__FILE__,__FUNCTION__,__LINE__);

  ut.AssertF(m_HKFE->IsCall("F-21400-2012-10"),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsPut("F-21400-2012-10"),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsFut("F-21400-2012-10"),__FILE__,__FUNCTION__,__LINE__);




  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121005),1) == 17,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121012),1) == 12,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121019),1) == 7,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121026),1) == 3,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121029),1) == 2,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121030),1) == 1,__FILE__,__FUNCTION__,__LINE__);


  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121005),0) == -1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121012),0) == -1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121019),0) == -1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121026),0) == -1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121029),0) == -1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121030),0) == -1,__FILE__,__FUNCTION__,__LINE__);


  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121005),-1) == -1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121012),-1) == -1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121019),-1) == -1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121026),-1) == -1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121029),-1) == -1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121030),-1) == -1,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121005),2) == 17+22,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121012),2) == 12+22,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121019),2) == 7+22,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121026),2) == 3+22,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121029),2) == 2+22,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121030),2) == 1+22,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121031),1) == 0+22,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetDTM(YYYYMMDD(20121101),1) ==-1+22,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20121005,130000),1) - (double)17/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20121012,130000),1) - (double)12/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20121019,130000),1) - (double)7/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20121026,130000),1) - (double)3/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20121029,130000),1) - (double)2/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20121030,130000),1) - (double)1/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20121005,130000),2) - (double)(17+22)/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20121012,130000),2) - (double)(12+22)/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20121019,130000),2) - (double)(7+22)/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20121026,130000),2) - (double)(3+22)/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20121029,130000),2) - (double)(2+22)/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20121030,130000),2) - (double)(1+22)/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20121031,130000),1) - (double)(0+22)/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20121101,130000),1) - (double)(-1+22)/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20130205,130000),1) - (double)14/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20130214,130000),1) - (double)10/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20130219,130000),1) - (double)7/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20130226,130000),1) - (double)2/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20130227,130000),1) - (double)1/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20130205,130000),2) - (double)(14+20)/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20130214,130000),2) - (double)(10+20)/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20130219,130000),2) - (double)(7+20)/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20130226,130000),2) - (double)(2+20)/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(abs(m_HKFE->GetTTM(YYYYMMDDHHMMSS(20130227,130000),2) - (double)(1+20)/252) < 0.01,__FILE__,__FUNCTION__,__LINE__);





  int iWhichFM=0;
  ut.Assert(m_HKFE->WhichFrontMth(20120905,201210,iWhichFM),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(iWhichFM == 2,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->WhichFrontMth(20121005,201210,iWhichFM),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(iWhichFM == 1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->WhichFrontMth(20121030,201210,iWhichFM),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(iWhichFM == 1,__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->WhichFrontMth(20121031,201210,iWhichFM),__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetExpy("F-2012-10",1)       == YYYYMMDD(20121030),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetExpy("C-23000-2012-10",1) == YYYYMMDD(20121030),__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetExpy(YMD(2012,10, 5),1)   == YYYYMMDD(20121030),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetExpy(YMD(2012, 9,28),1)   == YYYYMMDD(20121030),__FILE__,__FUNCTION__,__LINE__);



  ut.Assert(m_HKFE->GetExpy(YMD(2012,10,2),-3) == YYYYMMDD(20120628),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetExpy(YMD(2012,10,2),-2) == YYYYMMDD(20120730),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetExpy(YMD(2012,10,2),-1) == YYYYMMDD(20120830),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetExpy(YMD(2012,10,2),0 ) == YYYYMMDD(20120927),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetExpy(YMD(2012,10,2),1 ) == YYYYMMDD(20121030),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetExpy(YMD(2012,10,2),2 ) == YYYYMMDD(20121129),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetExpy(YMD(2012,10,2),3 ) == YYYYMMDD(20121228),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetExpy(YMD(2012,10,2),4 ) == YYYYMMDD(20130130),__FILE__,__FUNCTION__,__LINE__);


  YMD ymd;
  ymd.Set(2012,10,3); m_HKFE->NextTrdgDate(ymd,-1)   ; ut.Assert(ymd == YYYYMMDD(20120928), __FILE__,__FUNCTION__,__LINE__);
  ymd.Set(2012,10,3); m_HKFE->NextTrdgDate(ymd,0)    ; ut.Assert(ymd == YYYYMMDD(20121003), __FILE__,__FUNCTION__,__LINE__);
  ymd.Set(2012,10,3); m_HKFE->NextTrdgDate(ymd,1)    ; ut.Assert(ymd == YYYYMMDD(20121004), __FILE__,__FUNCTION__,__LINE__);
  ymd.Set(2012,10,3); m_HKFE->NextTrdgDate(ymd,2)    ; ut.Assert(ymd == YYYYMMDD(20121005), __FILE__,__FUNCTION__,__LINE__);
  ymd.Set(2012,10,3); m_HKFE->NextTrdgDate(ymd,20000); ut.Assert(ymd == YYYYMMDD::BAD_DATE,  __FILE__,__FUNCTION__,__LINE__);


  HMS hms;
  ymd.Set(2013,5,3); hms.Set( 9, 5,0); m_HKFE->NextTrdgMin(ymd,hms); ut.Assert(ymd == YMD(2013,5,3) && hms == HMS( 9,15,0),  __FILE__,__FUNCTION__,__LINE__);
  ymd.Set(2013,5,3); hms.Set( 9,42,0); m_HKFE->NextTrdgMin(ymd,hms); ut.Assert(ymd == YMD(2013,5,3) && hms == HMS( 9,43,0),  __FILE__,__FUNCTION__,__LINE__);
  ymd.Set(2013,5,3); hms.Set(11,30,0); m_HKFE->NextTrdgMin(ymd,hms); ut.Assert(ymd == YMD(2013,5,3) && hms == HMS(11,31,0),  __FILE__,__FUNCTION__,__LINE__);
  ymd.Set(2013,5,3); hms.Set(11,59,0); m_HKFE->NextTrdgMin(ymd,hms); ut.Assert(ymd == YMD(2013,5,3) && hms == HMS(12, 0,0),  __FILE__,__FUNCTION__,__LINE__);
  ymd.Set(2013,5,3); hms.Set(12, 0,0); m_HKFE->NextTrdgMin(ymd,hms); ut.Assert(ymd == YMD(2013,5,3) && hms == HMS(13, 0,0),  __FILE__,__FUNCTION__,__LINE__);
  ymd.Set(2013,5,3); hms.Set(12,30,0); m_HKFE->NextTrdgMin(ymd,hms); ut.Assert(ymd == YMD(2013,5,3) && hms == HMS(13, 0,0),  __FILE__,__FUNCTION__,__LINE__);
  ymd.Set(2013,5,3); hms.Set(13, 0,0); m_HKFE->NextTrdgMin(ymd,hms); ut.Assert(ymd == YMD(2013,5,3) && hms == HMS(13, 1,0),  __FILE__,__FUNCTION__,__LINE__);
  ymd.Set(2013,5,3); hms.Set(16,14,0); m_HKFE->NextTrdgMin(ymd,hms); ut.Assert(ymd == YMD(2013,5,3) && hms == HMS(16,15,0),  __FILE__,__FUNCTION__,__LINE__);
  ymd.Set(2013,5,3); hms.Set(16,15,0); m_HKFE->NextTrdgMin(ymd,hms); ut.Assert(ymd == YMD(2013,5,6) && hms == HMS( 9,15,0), __FILE__,__FUNCTION__,__LINE__);






  ut.AssertF(m_HKFE->IsTradingHour(YMD(2013, 4, 8),HMS( 9,14,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2013, 4, 8),HMS( 9,15,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2013, 4, 8),HMS( 9,26,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2013, 4, 8),HMS(11,36,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2013, 4, 8),HMS(11,54,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2013, 4, 8),HMS(11,56,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2013, 4, 8),HMS(12, 1,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2013, 4, 8),HMS(12, 5,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2013, 4, 8),HMS(12,59,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2013, 4, 8),HMS(13, 1,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2013, 4, 8),HMS(13,15,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2013, 4, 8),HMS(14,29,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2013, 4, 8),HMS(16, 9,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2013, 4, 8),HMS(16,15,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2013, 4, 8),HMS(16,16,0)),__FILE__,__FUNCTION__,__LINE__);

  ut.AssertF(m_HKFE->IsTradingHour(YMD(2012, 3, 5),HMS( 9,14,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2012, 3, 5),HMS( 9,15,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2012, 3, 5),HMS( 9,26,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2012, 3, 5),HMS(11,36,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2012, 3, 5),HMS(11,54,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2012, 3, 5),HMS(11,56,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2012, 3, 5),HMS(12, 1,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2012, 3, 5),HMS(12, 5,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2012, 3, 5),HMS(12,59,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2012, 3, 5),HMS(13, 1,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2012, 3, 5),HMS(13,15,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2012, 3, 5),HMS(14,29,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2012, 3, 5),HMS(16, 9,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2012, 3, 5),HMS(16,15,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2012, 3, 5),HMS(16,16,0)),__FILE__,__FUNCTION__,__LINE__);

  ut.AssertF(m_HKFE->IsTradingHour(YMD(2011, 3, 7),HMS( 9,14,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2011, 3, 7),HMS( 9,15,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2011, 3, 7),HMS( 9,26,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2011, 3, 7),HMS(11,36,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2011, 3, 7),HMS(11,54,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2011, 3, 7),HMS(11,56,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2011, 3, 7),HMS(12, 1,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2011, 3, 7),HMS(12, 5,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2011, 3, 7),HMS(12,59,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2011, 3, 7),HMS(13, 1,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2011, 3, 7),HMS(13,15,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2011, 3, 7),HMS(14,29,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2011, 3, 7),HMS(16, 9,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2011, 3, 7),HMS(16,15,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2011, 3, 7),HMS(16,16,0)),__FILE__,__FUNCTION__,__LINE__);

  ut.AssertF(m_HKFE->IsTradingHour(YMD(2005,10,27),HMS( 9,14,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2005,10,27),HMS( 9,15,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2005,10,27),HMS( 9,26,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2005,10,27),HMS(11,36,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2005,10,27),HMS(11,54,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2005,10,27),HMS(11,56,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2005,10,27),HMS(12, 1,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2005,10,27),HMS(12, 5,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2005,10,27),HMS(12,59,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2005,10,27),HMS(13, 1,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2005,10,27),HMS(13,15,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2005,10,27),HMS(14,29,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2005,10,27),HMS(16, 9,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert (m_HKFE->IsTradingHour(YMD(2005,10,27),HMS(16,15,0)),__FILE__,__FUNCTION__,__LINE__);
  ut.AssertF(m_HKFE->IsTradingHour(YMD(2005,10,27),HMS(16,16,0)),__FILE__,__FUNCTION__,__LINE__);



  ut.Assert(m_HKFE->GetTrdgSess(YMD(2013, 4, 8),HMS( 9,14,0)) == 7,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2013, 4, 8),HMS( 9,15,0)) == 1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2013, 4, 8),HMS(11,59,0)) == 1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2013, 4, 8),HMS(12, 0,0)) == 1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2013, 4, 8),HMS(12, 1,0)) == 8,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2013, 4, 8),HMS(12, 5,0)) == 8,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2013, 4, 8),HMS(12,59,0)) == 8,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2013, 4, 8),HMS(13, 0,0)) == 2,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2013, 4, 8),HMS(16,15,0)) == 2,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2013, 4, 8),HMS(16,16,0)) == 9,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetTrdgSess(YMD(2012, 3, 5),HMS( 9,14,0)) == 7,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2012, 3, 5),HMS( 9,15,0)) == 1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2012, 3, 5),HMS(12, 0,0)) == 1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2012, 3, 5),HMS(12, 1,0)) == 8,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2012, 3, 5),HMS(12,59,0)) == 8,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2012, 3, 5),HMS(13, 0,0)) == 2,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2012, 3, 5),HMS(16,15,0)) == 2,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2012, 3, 5),HMS(16,16,0)) == 9,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetTrdgSess(YMD(2011, 3, 7),HMS( 9,14,0)) == 7,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2011, 3, 7),HMS( 9,15,0)) == 1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2011, 3, 7),HMS(12, 0,0)) == 1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2011, 3, 7),HMS(12, 1,0)) == 8,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2011, 3, 7),HMS(13,29,0)) == 8,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2011, 3, 7),HMS(13,30,0)) == 2,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2011, 3, 7),HMS(16,15,0)) == 2,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2011, 3, 7),HMS(16,16,0)) == 9,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetTrdgSess(YMD(2005,10,27),HMS( 9,44,0)) == 7,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2005,10,27),HMS( 9,45,0)) == 1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2005,10,27),HMS(12,30,0)) == 1,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2005,10,27),HMS(12,31,0)) == 8,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2005,10,27),HMS(14,29,0)) == 8,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2005,10,27),HMS(14,30,0)) == 2,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2005,10,27),HMS(16,15,0)) == 2,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetTrdgSess(YMD(2005,10,27),HMS(16,16,0)) == 9,__FILE__,__FUNCTION__,__LINE__);


  m_HKFE->GetMrngOpen(YMD(2013, 4, 8),hms) ; ut.Assert(hms == HMS( 9,15,0),__FILE__,__FUNCTION__,__LINE__);
  m_HKFE->GetMrngClose(YMD(2013, 4, 8),hms); ut.Assert(hms == HMS(12, 0,0),__FILE__,__FUNCTION__,__LINE__);
  m_HKFE->GetAftnOpen(YMD(2013, 4, 8),hms) ; ut.Assert(hms == HMS(13, 0,0),__FILE__,__FUNCTION__,__LINE__);
  m_HKFE->GetAftnClose(YMD(2013, 4, 8),hms); ut.Assert(hms == HMS(16,15,0),__FILE__,__FUNCTION__,__LINE__);

  m_HKFE->GetMrngOpen(YMD(2012, 3, 5),hms) ; ut.Assert(hms == HMS( 9,15,0),__FILE__,__FUNCTION__,__LINE__);
  m_HKFE->GetMrngClose(YMD(2012, 3, 5),hms); ut.Assert(hms == HMS(12, 0,0),__FILE__,__FUNCTION__,__LINE__);
  m_HKFE->GetAftnOpen(YMD(2012, 3, 5),hms) ; ut.Assert(hms == HMS(13, 0,0),__FILE__,__FUNCTION__,__LINE__);
  m_HKFE->GetAftnClose(YMD(2012, 3, 5),hms); ut.Assert(hms == HMS(16,15,0),__FILE__,__FUNCTION__,__LINE__);

  m_HKFE->GetMrngOpen(YMD(2011, 3, 7),hms) ; ut.Assert(hms == HMS( 9,15,0),__FILE__,__FUNCTION__,__LINE__);
  m_HKFE->GetMrngClose(YMD(2011, 3, 7),hms); ut.Assert(hms == HMS(12, 0,0),__FILE__,__FUNCTION__,__LINE__);
  m_HKFE->GetAftnOpen(YMD(2011, 3, 7),hms) ; ut.Assert(hms == HMS(13,30,0),__FILE__,__FUNCTION__,__LINE__);
  m_HKFE->GetAftnClose(YMD(2011, 3, 7),hms); ut.Assert(hms == HMS(16,15,0),__FILE__,__FUNCTION__,__LINE__);

  m_HKFE->GetMrngOpen(YMD(2005,10,27),hms) ; ut.Assert(hms == HMS( 9,45,0),__FILE__,__FUNCTION__,__LINE__);
  m_HKFE->GetMrngClose(YMD(2005,10,27),hms); ut.Assert(hms == HMS(12,30,0),__FILE__,__FUNCTION__,__LINE__);
  m_HKFE->GetAftnOpen(YMD(2005,10,27),hms) ; ut.Assert(hms == HMS(14,30,0),__FILE__,__FUNCTION__,__LINE__);
  m_HKFE->GetAftnClose(YMD(2005,10,27),hms); ut.Assert(hms == HMS(16,15,0),__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->IsExpyDay(YYYYMMDD(20140528)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsExpyDay(YYYYMMDD(20140529)) == true, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsExpyDay(YYYYMMDD(20140530)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsExpyDay(YYYYMMDD(20110102)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsExpyDay(YYYYMMDD(20110103)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsExpyDay(YYYYMMDD(20140326)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsExpyDay(YYYYMMDD(20140328)) == true, __FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->IsTrdgDay(YYYYMMDD(20140528)) == true, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsTrdgDay(YYYYMMDD(20140529)) == true, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsTrdgDay(YYYYMMDD(20140530)) == true, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsTrdgDay(YYYYMMDD(20110102)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsTrdgDay(YYYYMMDD(20110103)) == true, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsTrdgDay(YYYYMMDD(20140326)) == true, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsTrdgDay(YYYYMMDD(20140328)) == true, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsTrdgDay(YYYYMMDD(20140531)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsTrdgDay(YYYYMMDD(20140601)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsTrdgDay(YYYYMMDD(20110101)) == false,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->IsHalfDay(YYYYMMDD(20141124)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsHalfDay(YYYYMMDD(20141125)) == true,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsHalfDay(YYYYMMDD(20141126)) == false,__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->IsExpyDay(YMD(2014, 5,28)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsExpyDay(YMD(2014, 5,29)) == true, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsExpyDay(YMD(2014, 5,30)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsExpyDay(YMD(2011, 1, 2)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsExpyDay(YMD(2011, 1, 3)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsExpyDay(YMD(2014, 3,26)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsExpyDay(YMD(2014, 3,28)) == true, __FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->IsTrdgDay(YMD(2014, 5,28)) == true, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsTrdgDay(YMD(2014, 5,29)) == true, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsTrdgDay(YMD(2014, 5,30)) == true, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsTrdgDay(YMD(2011, 1, 2)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsTrdgDay(YMD(2011, 1, 3)) == true, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsTrdgDay(YMD(2014, 3,26)) == true, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsTrdgDay(YMD(2014, 3,28)) == true, __FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsTrdgDay(YMD(2014, 5,31)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsTrdgDay(YMD(2014, 6, 1)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsTrdgDay(YMD(2011, 1, 1)) == false,__FILE__,__FUNCTION__,__LINE__);


  ut.Assert(m_HKFE->IsHalfDay(YMD(2014,12,21)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsHalfDay(YMD(2014,12,22)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsHalfDay(YMD(2014,12,23)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsHalfDay(YMD(2014,12,24)) == true,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsHalfDay(YMD(2014,12,25)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsHalfDay(YMD(2014,12,26)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsHalfDay(YMD(2014,12,27)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsHalfDay(YMD(2014,12,28)) == false,__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->IsHalfDay(YMD(2014,12,31)) == true,__FILE__,__FUNCTION__,__LINE__);



  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::FUTURES,YYYYMMDD(20110103),1) == "F1",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::FUTURES,YYYYMMDD(20110103),2) == "G1",__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::FUTURES,YYYYMMDD(20140102),1) == "F4",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::FUTURES,YYYYMMDD(20140102),2) == "G4",__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::FUTURES,YYYYMMDD(20140128),1) == "F4",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::FUTURES,YYYYMMDD(20140128),2) == "G4",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::FUTURES,YYYYMMDD(20140129),1) == "F4",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::FUTURES,YYYYMMDD(20140129),2) == "G4",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::FUTURES,YYYYMMDD(20140130),1) == "G4",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::FUTURES,YYYYMMDD(20140130),2) == "H4",__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20110103),1) == "F11",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20110103),2) == "G11",__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20140102),1) == "F14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20140102),2) == "G14",__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20140128),1) == "F14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20140128),2) == "G14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20140129),1) == "F14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20140129),2) == "G14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20140130),1) == "G14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20140130),2) == "H14",__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20131120),1) == "X13",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20131120),2) == "Z13",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20131128),1) == "X13",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20131128),2) == "Z13",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20131129),1) == "Z13",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20131129),2) == "F14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20131202),1) == "Z13",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20131202),2) == "F14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20131212),1) == "Z13",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20131212),2) == "F14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20131230),1) == "Z13",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20131230),2) == "F14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20131231),1) == "F14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20131231),2) == "G14",__FILE__,__FUNCTION__,__LINE__);



  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20081222),1) == "Z08",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20081222),2) == "F09",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20081231),1) == "F09",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::FUTURES,YYYYMMDD(20081231),2) == "G09",__FILE__,__FUNCTION__,__LINE__);


  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::CALL,YYYYMMDD(20110103),1) == "A1",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::CALL,YYYYMMDD(20110103),2) == "B1",__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::CALL,YYYYMMDD(20140102),1) == "A4",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::CALL,YYYYMMDD(20140102),2) == "B4",__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::CALL,YYYYMMDD(20140128),1) == "A4",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::CALL,YYYYMMDD(20140128),2) == "B4",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::CALL,YYYYMMDD(20140129),1) == "A4",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::CALL,YYYYMMDD(20140129),2) == "B4",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::CALL,YYYYMMDD(20140130),1) == "B4",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::CALL,YYYYMMDD(20140130),2) == "C4",__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::CALL,YYYYMMDD(20110103),1) == "A11",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::CALL,YYYYMMDD(20110103),2) == "B11",__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::CALL,YYYYMMDD(20140102),1) == "A14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::CALL,YYYYMMDD(20140102),2) == "B14",__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::CALL,YYYYMMDD(20140128),1) == "A14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::CALL,YYYYMMDD(20140128),2) == "B14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::CALL,YYYYMMDD(20140129),1) == "A14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::CALL,YYYYMMDD(20140129),2) == "B14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::CALL,YYYYMMDD(20140130),1) == "B14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::CALL,YYYYMMDD(20140130),2) == "C14",__FILE__,__FUNCTION__,__LINE__);


  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::PUT ,YYYYMMDD(20110103),1) == "M1",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::PUT ,YYYYMMDD(20110103),2) == "N1",__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::PUT ,YYYYMMDD(20140102),1) == "M4",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::PUT ,YYYYMMDD(20140102),2) == "N4",__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::PUT ,YYYYMMDD(20140128),1) == "M4",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::PUT ,YYYYMMDD(20140128),2) == "N4",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::PUT ,YYYYMMDD(20140129),1) == "M4",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::PUT ,YYYYMMDD(20140129),2) == "N4",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::PUT ,YYYYMMDD(20140130),1) == "N4",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1(HKFE::PUT ,YYYYMMDD(20140130),2) == "O4",__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::PUT ,YYYYMMDD(20110103),1) == "M11",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::PUT ,YYYYMMDD(20110103),2) == "N11",__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::PUT ,YYYYMMDD(20140102),1) == "M14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::PUT ,YYYYMMDD(20140102),2) == "N14",__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::PUT ,YYYYMMDD(20140128),1) == "M14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::PUT ,YYYYMMDD(20140128),2) == "N14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::PUT ,YYYYMMDD(20140129),1) == "M14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::PUT ,YYYYMMDD(20140129),2) == "N14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::PUT ,YYYYMMDD(20140130),1) == "N14",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode2(HKFE::PUT ,YYYYMMDD(20140130),2) == "O14",__FILE__,__FUNCTION__,__LINE__);

  ut.Assert(m_HKFE->GetCtrtCode1_WithDTM(HKFE::FUTURES,YYYYMMDD(20121101),10) == "X2",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1_WithDTM(HKFE::FUTURES,YYYYMMDD(20121101),20) == "X2",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1_WithDTM(HKFE::FUTURES,YYYYMMDD(20121101),21) == "X2",__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetCtrtCode1_WithDTM(HKFE::FUTURES,YYYYMMDD(20121101),22) == "Z2",__FILE__,__FUNCTION__,__LINE__);


  ut.Assert(m_HKFE->GetExpyFmtMxx(HKFE::FUTURES,"X12") == YYYYMMDD(20121129),__FILE__,__FUNCTION__,__LINE__);
  ut.Assert(m_HKFE->GetExpyFmtMxx(HKFE::FUTURES,"Z12") == YYYYMMDD(20121228),__FILE__,__FUNCTION__,__LINE__);


  ut.PrintResult();


  return 0;
}
