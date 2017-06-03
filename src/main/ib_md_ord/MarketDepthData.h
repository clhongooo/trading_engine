/*
 * MarketDepthData.h
 *
 *  Created on: Oct 22, 2014
 *      Author: jasonlin
 */

#ifndef MARKETDEPTHDATA_H_
#define MARKETDEPTHDATA_H_

#define INVALID_NUM 999999

#include <vector>
#include <utility>
#include "IBString.h"
#include "ATU_Abstract_MDI.h"
#include "SDateTime.h"

#define MARKET_DATA_DEPTH 5

using namespace std;

namespace atu {

class MarketDepthData {
public:
	IBString m_feedcode;
	double m_traded_price;
	double m_traded_volume;
	vector<pair<double, double> > m_bidsPriceVol;
	vector<pair<double, double> > m_asksPriceVol;

	MarketDepthData();
	MarketDepthData(IBString p_feedcode);

	bool update(int p_position, int p_operation, int p_side, double p_price, int p_size);

	bool checkValidity();

	void insertOrder(vector<pair<double, double> > &p_priceVol, int p_position, double p_price, int p_size);

	void updateOrder(vector<pair<double, double> > &p_priceVol, int p_position, double p_price, int p_size);

	void deleteOrder(vector<pair<double, double> > &p_priceVol, int p_position);

	void getMarketfeed(ATU_MDI_marketfeed_struct &) const;

};

}

#endif /* MARKETDEPTHDATA_H_ */
