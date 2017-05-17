#ifndef BROKERAPI_HPP
#define BROKERAPI_HPP

#include "PracticalSocket.h"
#include <string>

using namespace std;

// To provide a consistent and easy-to-use programming interface
// independent of the Brokers that are actually used
class BrokerAPI
{
	private:
		BrokerAPI() {}

	public:
		static enum BROKER = {
			IB,
			GLORYSKY
		};

		void Trade(const string &, const int, const int);

		static BrokerAPI& Instance()
		{
			static BrokerAPI _instance;
			return _instance;
		}

};


void BrokerAPI::Trade(
		const string &sSym,
		const int iSide,
		const int iQty)
{

	return;
}

#endif
