/*
 * ATU_IB_ClientSocket_OTI.h
 *
 *  Created on: Oct 6, 2014
 *      Author: jasonlin
 */

#ifndef ATU_IB_CLIENTSOCKET_OTI_H_
#define ATU_IB_CLIENTSOCKET_OTI_H_

#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <boost/thread.hpp>
#include "EClientSocketBase.h"

namespace atu {

class ATU_IB_ClientSocket_OTI : public EClientSocketBase
{
public:
	explicit ATU_IB_ClientSocket_OTI(EWrapper *p_ewrapper);
	~ATU_IB_ClientSocket_OTI();
	bool eConnect(const char *p_host, unsigned int p_port = 7496, int p_clientId = 0, bool p_extraAuth = false);
	void eDisconnect();
	bool isSocketOK() const;
	bool handleSocketError();
	int fd() const;
	void startHandlingSendRecvMsg();
	void placeOrderSync(OrderId id, const Contract &contract, const Order &order);
	void cancelOrderSync(OrderId id);
	void reqExecutionsSync(int reqId, const ExecutionFilter& filter);

private:
	void processSendMsg();
	void processRecvMsg();
	void onReceive();
	void onSend();
	void onError();
	void onConnect();
	void onClose();
	int send(const char *p_buf, size_t p_sz);
	int receive(char* p_buf, size_t p_sz);

private:
	int m_fd;

	boost::thread m_sendThread;
	boost::thread m_recvThread;
	boost::mutex m_sendMutex;
	boost::mutex m_recvMutex;
	boost::condition_variable m_sendCond;
	boost::condition_variable m_recvCond;
};

}

#endif /* ATU_IB_ClientSocket_OTI_H_ */
