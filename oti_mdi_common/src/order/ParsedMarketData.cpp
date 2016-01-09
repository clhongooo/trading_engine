#include "ParsedMarketData.h"
#include <cstdio>
#include <cstring>

ParsedMarketData::ParsedMarketData(){
#ifdef _ATUDEBUG_
//	cout << "creating PMD..." << endl;
#endif // End of _ATUDEBUG_
	m_id = "";
	m_feedcode = "";
	m_accSize = 0;
	m_lastTradedPrice = 999999;
	m_lastUpdateTime = 0;
	m_updated = false;
	m_trade_date = 0;
	m_lastUpdateTime_milliSecond = 0;

	for (int i = 0; i < 10; i++) {
		m_bid[i] = 999999;
		m_ask[i] = 999999;
		m_bid_size[i] = 999999;
		m_ask_size[i] = 999999;
	}
}

ParsedMarketData::~ParsedMarketData(){
#ifdef _ATUDEBUG_
//	cout << "destroying PMD..." << endl;
#endif // End of _ATUDEBUG_
}
/*ParsedMarketData::ParsedMarketData(const ParsedMarketData& pmd) {
#ifdef _ATUDEBUG_
	cout << "copying..." << endl;
#endif // End of _ATUDEBUG_
}
*/
void ParsedMarketData::printMarketData(ostream &os){
	char line[500];
	if (m_trade_date != 0) {
		sprintf(line, "%06d_%06d_%06d,%s,%0.3lf,%lld,B",
				m_trade_date,
				m_lastUpdateTime,
				m_lastUpdateTime_milliSecond,
				m_feedcode.c_str(),
				m_lastTradedPrice,
				m_accSize);
	} else {
		sprintf(line, "%06d,%s,%0.3lf,%lld,B",
				m_lastUpdateTime,
				m_feedcode.c_str(),
				m_lastTradedPrice,
				m_accSize);
	}
	for (int i = 0; i < 5; i++) {
		sprintf(line + strlen(line), ",%0.3lf,%lld", m_bid[i], m_bid_size[i]);
	}
	sprintf(line + strlen(line), ",A");
	for (int i = 0; i < 5; i++) {
		sprintf(line + strlen(line), ",%0.3lf,%lld", m_ask[i], m_ask_size[i]);
	}

	os << line << endl;
	os.flush();
}
void ParsedMarketData::printMarketDataWithFullTimeStampAndPerTrade(ostream &os,string timestamp,double tradesize){
	char line[500];
	sprintf(line, "%s,%s,%0.3lf,%.0lf,B",
			timestamp.c_str(),
			m_feedcode.c_str(),
			m_lastTradedPrice,
			tradesize);
	for (int i = 0; i < 5; i++) {
		sprintf(line + strlen(line), ",%0.3lf,%lld", m_bid[i], m_bid_size[i]);
	}
	sprintf(line + strlen(line), ",A");
	for (int i = 0; i < 5; i++) {
		sprintf(line + strlen(line), ",%0.3lf,%lld", m_ask[i], m_ask_size[i]);
	}

	os << line << endl;
	os.flush();
}

void ParsedMarketData::printFirstLevelOnly(ostream &os) {
	char line[500];
	sprintf(line,"%d,%8.3lf,%lld,%8.3lf,%lld",m_lastUpdateTime,m_bid[0],m_bid_size[0],m_ask[0],m_ask_size[0]);
	os << line << endl;
	os.flush();
}

