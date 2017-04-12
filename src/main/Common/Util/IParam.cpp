#include "IParam.h"

using namespace std;

const char *IParam::HKFE_CALENDAR = "/home/sunny/Dropbox/nirvana/cfg/HKFE-Calendar.csv";
const char *IParam::DATA_FUT1 = "io/fut-1.csv";
const char *IParam::DATA_FUT2 = "io/fut-2.csv";
const char *IParam::DATA_OPT1 = "io/opt-1.csv";
const char *IParam::DATA_OPT2 = "io/opt-2.csv";
const char *IParam::VSMILE_1_SMTH = "io/vs-smth-1.csv";
const char *IParam::VSMILE_2_SMTH = "io/vs-smth-2.csv";
const char *IParam::VSMILE_1_RUGD = "io/vs-rugd-1.csv";
const char *IParam::VSMILE_2_RUGD = "io/vs-rugd-2.csv";

const char *IParam::FINSTATEMACH_PMF = "io/fsm-pmf/";
const char *IParam::FINSTATEMACH_PMF_HSILAST = "HSI-Latest.csv";
const char *IParam::FINSTATEMACH_PMF_JOINT = "io/fsm-pmf/Joint.csv";
const char *IParam::STOCKLIST_HSI= "cfg/hsi-constit.csv";
const char *IParam::STOCKLIST_HK_INDICES = "cfg/hk-idx-stk.csv";
const char *IParam::STOCKLIST_TESTING= "cfg/testing-stk.csv";
const char *IParam::STOCKLIST_HSI_HIGH_COR = "cfg/hsi-hh-cor.csv";
const char *IParam::HSI_STK_CORREL = "io/hsi-stk-correl.csv";
const char *IParam::DATA_YAHOO = "../dataENF/yahData/data/";
const char *IParam::DATA_ALL_STOCKS = "../dataENF/yahData/data/";
const char *IParam::DATA_DAILY_VHSI = "../dataRT/data-vhsi-new.csv";
const char *IParam::DATA_DAILY_HSI = "../dataRT/data-new.csv";
const char *IParam::DATA_INTRADAY_HSIF_ACTIVE = "../dataRT/IBRTFeed/out/HSIF-ACTIVE-60.csv";
const char *IParam::DATA_INTRADAY_HSIF = "../dataENF/assembledData/HSIF.csv";
const char *IParam::DATA_INTRADAY_HSIF1Y = "../dataENF/assembledData/HSIF1Y.csv";
const char *IParam::DATA_INTRADAY_HSIF1M = "../dataENF/assembledData/HSIF1M.csv";
const char *IParam::OPTMASTER_DATA = "../dataENF/hkexDlydOptPx/processed";

const int IParam::OPT_STRIKE_INTVL = 200;
const int IParam::VSMILE_INTVL = 10;
const double IParam::FSM_PMF_STK_INTVL = 0.002;
const double IParam::FSM_PMF_STK_ABS_MAX = 0.30;
const double IParam::FSM_PMF_HSI_INTVL = 0.001;
const double IParam::FSM_PMF_HSI_ABS_MAX = 0.10;

// const int IParam::MAGIC_NUM_INT = -3466;
// const double MAGIC_NUM = -3466.23556;
const double IParam::TWO_DIMENSIONAL_DOUBLE = 1000;

const double IParam::R_F = 0.01;
const double IParam::DIV_YIELD = 0.02;

const double IParam::TRDG_FRICT_OPT_PCTG = 0.02;
const double IParam::TRDG_FRICT_FUT_SLIP = 3;
const double IParam::TRDG_FRICT_CMSN = 1;
