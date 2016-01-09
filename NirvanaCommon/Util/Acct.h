#ifndef ACCT_HPP
#define ACCT_HPP

#include "PCH.h"
#include "Constants.h"
#include "SFunctional.h"
#include "STool.h"

class Acct
{
	public:
		Acct();

    //--------------------------------------------------
    // Get
    //--------------------------------------------------
		double CPnL();
		double GetCPnL();
		long Pos(const string &);
		double GetSignedNotional(const string &);
    const map<string,long> & GetPortfolio();
    set<string> GetSymbolsWithPos();
    bool CheckIfPosChgd();

    //--------------------------------------------------
    // Set
    //--------------------------------------------------
		void Reset();
    bool LoadFromFile(const string &);
    void SetCPnL(const double);
    void SetRecord(const string &,long,double);
		void Trade(const string &,long,double);
		void TradeWithTC(const string &,long,double,const double);
		void ClearPosChgFlag();

    //--------------------------------------------------
    // Output
    //--------------------------------------------------
    void WriteToFile(const string &);
    //--------------------------------------------------

    //--------------------------------------------------
    // MTM
    //--------------------------------------------------
    void ProvidePriceForMTM(const string &,const double);
    bool Get_MTM_CPnL(double &);

	private:
    double             m_dCPnL;
		map<string,double> m_AggPx;
		map<string,long>   m_Pos;
		map<string,double> m_PxForMTM;
    bool               m_PosChgd;

};
#endif
