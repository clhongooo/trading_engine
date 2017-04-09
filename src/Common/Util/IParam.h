#ifndef IPARAM_H
#define IPARAM_H

using namespace std;

class IParam
{
	public:
		static const char *HKFE_CALENDAR;
		static const char *DATA_FUT1;
		static const char *DATA_FUT2;
		static const char *DATA_OPT1;
		static const char *DATA_OPT2;
		static const char *VSMILE_1_SMTH;
		static const char *VSMILE_2_SMTH;
		static const char *VSMILE_1_RUGD;
		static const char *VSMILE_2_RUGD;

		static const char *FINSTATEMACH_PMF;
		static const char *FINSTATEMACH_PMF_HSILAST;
		static const char *FINSTATEMACH_PMF_JOINT;
		static const char *STOCKLIST_HSI;
		static const char *STOCKLIST_HSI_HIGH_COR;
		static const char *HSI_STK_CORREL;
		static const char *STOCKLIST_HK_INDICES;
		static const char *STOCKLIST_TESTING;
		static const char *DATA_DAILY_VHSI;
		static const char *DATA_DAILY_VHSI_OLD;
		static const char *DATA_DAILY_HSI;
		static const char *DATA_INTRADAY_HSIF_ACTIVE;
		static const char *DATA_INTRADAY_HSIF;
		static const char *DATA_INTRADAY_HSIF1Y;
		static const char *DATA_INTRADAY_HSIF1M;
		static const char *OPTMASTER_DATA;
		static const char *DATA_DAILY_HSI_OLD;
		static const char *DATA_YAHOO;
		static const char *DATA_ALL_STOCKS;

		static const int OPT_STRIKE_INTVL;
		static const int VSMILE_INTVL;
		static const double FSM_PMF_STK_INTVL;
		static const double FSM_PMF_STK_ABS_MAX;
		static const double FSM_PMF_HSI_INTVL;
		static const double FSM_PMF_HSI_ABS_MAX;
		/* static const int MAGIC_NUM_INT; */
		/* static const double MAGIC_NUM; */
		static const double TWO_DIMENSIONAL_DOUBLE;


		static const double R_F;
		static const double DIV_YIELD;

		static const double TRDG_FRICT_OPT_PCTG;
		static const double TRDG_FRICT_FUT_SLIP;
		static const double TRDG_FRICT_CMSN;
};

#endif
