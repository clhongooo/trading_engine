/*
 * MarketDepthData.cpp
 *
 *  Created on: Oct 22, 2014
 *      Author: jasonlin
 */

#include "MarketDepthData.h"
#include "Toolbox.h"

namespace atu {

MarketDepthData::MarketDepthData() {

}

MarketDepthData::MarketDepthData(IBString p_feedcode) :
		 m_feedcode(p_feedcode)
		,m_traded_price(INVALID_NUM)
		,m_traded_volume(0)
		,m_bidsPriceVol(MARKET_DATA_DEPTH)
		,m_asksPriceVol(MARKET_DATA_DEPTH)
	{
		for (int i = 0; i < MARKET_DATA_DEPTH; i++) {
			m_bidsPriceVol[i].first = INVALID_NUM;
			m_bidsPriceVol[i].second = INVALID_NUM;
			m_asksPriceVol[i].first = INVALID_NUM;
			m_asksPriceVol[i].second = INVALID_NUM;
		}
	}

bool MarketDepthData::update(int p_position, int p_operation, int p_side, double p_price, int p_size)
{
	vector<pair<double, double> > &priceVol = p_side == 0 ? m_asksPriceVol : m_bidsPriceVol;

	if (p_operation == 0) {
		insertOrder(priceVol, p_position, p_price, p_size);
	} else if (p_operation == 1) {
		updateOrder(priceVol, p_position, p_price, p_size);
	} else {
		deleteOrder(priceVol, p_position);
	}

	return checkValidity();
}

bool MarketDepthData::checkValidity() {
	// Only one operation might not complete a valid feed. e.g. 5|4|3|2|1 -> update level 4 as 1 -> 5|4|3|1|1 -> delete level 5 -> 5|4|3|1
	// If invalid after checking, will not output
	bool lastBid = false;
	bool lastAsk = false;
	int i = 0;
	for (; i < MARKET_DATA_DEPTH - 1; i++) {
		if (m_bidsPriceVol[i].first == INVALID_NUM) {
			lastBid = true;
		} else {
			if (lastBid) {
				return false;
			} else if (m_bidsPriceVol[i + 1].first != INVALID_NUM && m_bidsPriceVol[i].first <= m_bidsPriceVol[i + 1].first) {
				return false;
			}
		}

		if (m_asksPriceVol[i].first == INVALID_NUM) {
			lastAsk = true;
		} else {
			if (lastAsk) {
				return false;
			} else if (m_asksPriceVol[i + 1].first != INVALID_NUM && m_asksPriceVol[i].first >= m_asksPriceVol[i + 1].first) {
				return false;
			}
		}
	}
	if (lastBid && m_bidsPriceVol[i].first != INVALID_NUM) {
		return false;
	}
	if (lastAsk && m_asksPriceVol[i].first != INVALID_NUM) {
		return false;
	}
	return true;
}

void MarketDepthData::insertOrder(vector<pair<double, double> > &p_priceVol, int p_position, double p_price, int p_size) {
	updateOrder(p_priceVol, p_position, p_price, p_size);
}

void MarketDepthData::updateOrder(vector<pair<double, double> > &p_priceVol, int p_position, double p_price, int p_size) {
	p_priceVol[p_position].first = p_price;
	p_priceVol[p_position].second = p_size;
}

void MarketDepthData::deleteOrder(vector<pair<double, double> > &p_priceVol, int p_position) {
	updateOrder(p_priceVol, p_position, INVALID_NUM, INVALID_NUM);
}

ATU_MDI_marketfeed_struct MarketDepthData::getMarketfeed() {
	ATU_MDI_marketfeed_struct mf;
	mf.m_timestamp = Toolbox::timeStamp_full();
	mf.m_feedcode = m_feedcode;
	mf.m_traded_price = m_traded_price;
	mf.m_traded_volume = m_traded_volume;
	mf.m_bid_price_1 = m_bidsPriceVol[0].first;
	mf.m_bid_volume_1 = m_bidsPriceVol[0].second;
	mf.m_bid_price_2 = m_bidsPriceVol[1].first;
	mf.m_bid_volume_2 = m_bidsPriceVol[1].second;
	mf.m_bid_price_3 = m_bidsPriceVol[2].first;
	mf.m_bid_volume_3 = m_bidsPriceVol[2].second;
	mf.m_bid_price_4 = m_bidsPriceVol[3].first;
	mf.m_bid_volume_4 = m_bidsPriceVol[3].second;
	mf.m_bid_price_5 = m_bidsPriceVol[4].first;
	mf.m_bid_volume_5 = m_bidsPriceVol[4].second;
	mf.m_ask_price_1 = m_asksPriceVol[0].first;
	mf.m_ask_volume_1 = m_asksPriceVol[0].second;
	mf.m_ask_price_2 = m_asksPriceVol[1].first;
	mf.m_ask_volume_2 = m_asksPriceVol[1].second;
	mf.m_ask_price_3 = m_asksPriceVol[2].first;
	mf.m_ask_volume_3 = m_asksPriceVol[2].second;
	mf.m_ask_price_4 = m_asksPriceVol[3].first;
	mf.m_ask_volume_4 = m_asksPriceVol[3].second;
	mf.m_ask_price_5 = m_asksPriceVol[4].first;
	mf.m_ask_volume_5 = m_asksPriceVol[4].second;
	return mf;
}

}


