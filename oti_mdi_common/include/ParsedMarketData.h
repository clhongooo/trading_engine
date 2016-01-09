#ifndef _PARSEDMARKETDATA_H_
#define _PARSEDMARKETDATA_H_
#include <string>
#include <iostream>
using namespace std;
class ParsedMarketData {
public:
	string m_id;
	string m_feedcode;
	int m_lastUpdateTime;
	bool m_updated;		// boolean to record if this data has been updated
	long long m_accSize;
	double m_bid[10];
	long long m_bid_size[10];
	double m_ask[10];
	long long m_ask_size[10];
	double m_lastTradedPrice;
	int m_trade_date;
	int m_lastUpdateTime_milliSecond;

	ParsedMarketData();
	//ParsedMarketData(int, int);
	//ParsedMarketData(const ParsedMarketData& pmd);
	virtual ~ParsedMarketData();
	void printMarketData(ostream&);
	void printMarketDataWithFullTimeStampAndPerTrade(ostream &os,string timestamp,double tradesize);
	void printFirstLevelOnly(ostream&);
};
#endif // End of _PARSEDMARKETDATA__H_
