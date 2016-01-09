#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "PCH.h"
#include "Constants.h"

#define MAGIC_NUM_INT     7236467
#define MAGIC_NUM         7236467

#define UNDERLYING_VHSI "VHSI"
#define UNDERLYING_HSI  "HSI"
#define UNDERLYING_HHI  "HHI"
#define UNDERLYING_MHI  "MHI"
#define UNDERLYING_MCH  "MCH"
#define HSI_CONT_FUT_1  "HSIc1"
#define HHI_CONT_FUT_1  "HHIc1"
#define MHI_CONT_FUT_1  "MHIc1"
#define MCH_CONT_FUT_1  "MCHc1"

#define UNDERLYING_ES   "ES"
#define ES_CONT_FUT_1   "ESc1"
#define UNDERLYING_NQ   "NQ"
#define NQ_CONT_FUT_1   "NQc1"
#define UNDERLYING_YM   "YM"
#define YM_CONT_FUT_1   "YMc1"
#define UNDERLYING_TF   "TF"
#define TF_CONT_FUT_1   "TFc1"


#define ATU_INVALID_PRICE  999999
#define UNDEFINED_CORRELATION 999
#define NIR_EPSILON        0.000001

#define MAX_OTIMDI 10
#define HEALTHKEYLEN 10


#define ID_TECHINDUPDATER       "TIU"
#define ID_PROBDISTGENR         "PDG"
#define ID_DATAAGG              "DA"
#define ID_VOLSURFCALC          "VSC"
#define ID_ORDEREXEC            "OE"
#define ID_ORDEREXECCHKORD      "OEChkOrd"
#define ID_ORDEREXEPERSISTPOS   "OEPP"
#define ID_MTM                  "MTM"





using namespace std;
using namespace boost;

enum StrategyID {
  STY_NIR=1,
  STY_B1_HKF,
  STY_B2_USSTK,
  STY_B2_HK,
  STY_ETFR,
  STY_S11A,
  STY_A1,
  STY_A6,
  STY_R1,
  STY_R3,
  STY_R7,
  STY_R8,
  STY_R9,
  STY_TEST,
  STY_LAST=99};

enum EExchange {
  EX_HKFE=1,
  EX_HKSE,
  EX_NYSE,
  EX_CME,
  EX_UNSET,
};


string GetStrategyName(const StrategyID);
void GetStrategyNameCStr(const StrategyID, char [HEALTHKEYLEN+1]);





#endif
