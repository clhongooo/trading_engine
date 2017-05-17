#ifndef GLORYSKY_HPP
#define GLORYSKY_HPP

#include "PracticalSocket.h"
#include <string>

using namespace std;

const int RCVBUFSIZE = 1;


// Broker specific implementation
class GlorySky
{
	private:
		TCPSocket * _sockPrice;
		TCPSocket * _sockTicker;
		TCPSocket * _sockOrder;


		GlorySky() {}

	public:
		static const string SERVADDRESS;
		static const unsigned short PRICEPORT;
		static const unsigned short TICKERPORT;
		static const unsigned short ORDERPORT;

		void Connect();
		void Disconnect();
		void ReqLastPrice(const string &);
		void CreateOrder(const string &, const double, int);
		void AmendOrder(int, const double, int);
		void CancelOrder(int);
		void LoadDoneTrade();
		void LoadPosition();

		static GlorySky& Instance()
		{
			static GlorySky _instance;
			return _instance;
		}

};

const string GlorySky::SERVADDRESS = "127.0.0.1"; //Don't use localhost, will have problems
const unsigned short GlorySky::PRICEPORT = 8089;
const unsigned short GlorySky::TICKERPORT = 8090;
const unsigned short GlorySky::ORDERPORT = 8092;


void GlorySky::Connect()
{
	try {
		// Establish connection with the echo server
		_sockPrice = new TCPSocket(SERVADDRESS, PRICEPORT);

	} catch(SocketException &e) {
		cerr << e.what() << endl;
		{
			SException se;
			se.PrintMsg(__FILE__, __FUNCTION__, __LINE__, "Can't establish connection with GlorySky SP Trader.");
			throw se;
		}


	}
}



void GlorySky::ReqLastPrice(const string &sSym)
{

	string sSendToSvr = "4106,0,HHIU3\r\n";
	int iSendToSvrLen = sSendToSvr.length();

	// Send the string to the echo server
	_sockPrice->send(sSendToSvr.c_str(), iSendToSvrLen);
	cout << "Sent:" << endl;
	cout << sSendToSvr << endl;

	char caBuffer[RCVBUFSIZE + 1];    // + \0
	int bytesReceived = 0;              // Bytes read on each recv()

	cout << "Received:" << endl;

	while (true)
	{
		// Receive up to the buffer size bytes from the sender
		if ((bytesReceived = (_sockPrice->recv(caBuffer, RCVBUFSIZE))) <= 0) {
			cerr << "Unable to read";
			//exit(1);
		}
		caBuffer[bytesReceived] = '\0';        // Terminate the string!
		cout << caBuffer;                      // Print the echo buffer
		//if (caBuffer[0] == '\r' || caBuffer[0] == '\n') break;
	}
	cout << endl;

	// Destructor closes the socket

	return;
}


void GlorySky::CreateOrder(
		const string &sSym,
		const double dPrice,
		int iQty
		)
{
	return;
}



#endif
