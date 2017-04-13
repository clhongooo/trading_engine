/*
 * ATU_IB_ClientSocket.cpp
 *
 *  Created on: Oct 6, 2014
 *      Author: jasonlin
 */

#include <string.h>
#include "ATU_IB_ClientSocket_MDI.h"
#include "TwsSocketClientErrors.h"

#include "Toolbox.h"

#define COND_WAIT_MS 1

namespace atu {

inline bool setSocketNonBlocking(int p_sockfd) {
	// get socket flags
	int flags = fcntl(p_sockfd, F_GETFL);
	if (flags == -1)
		return false;

	// set non-blocking mode
	return (fcntl(p_sockfd, F_SETFL, flags | O_NONBLOCK) == 0);
}

ATU_IB_ClientSocket_MDI::ATU_IB_ClientSocket_MDI(EWrapper *p_ewrapper) : EClientSocketBase(p_ewrapper)
{
	m_fd = -1;
}

ATU_IB_ClientSocket_MDI::~ATU_IB_ClientSocket_MDI()
{
	eDisconnect();
}

bool ATU_IB_ClientSocket_MDI::eConnect(const char* p_host, unsigned int p_port, int p_clientId, bool p_extraAuth)
{
	// reset errno
	errno = 0;

	// if already connected?
	if (m_fd >= 0) {
		errno = EISCONN;
		getWrapper()->error(NO_VALID_ID, ALREADY_CONNECTED.code(), ALREADY_CONNECTED.msg());
		return false;
	}

	// create socket
	m_fd = socket(AF_INET, SOCK_STREAM, 0);

	// if cannot create socket
	if (m_fd < 0) {
		getWrapper()->error(NO_VALID_ID, FAIL_CREATE_SOCK.code(), FAIL_CREATE_SOCK.msg());
		return false;
	}

	// start to connect to the server where TWS locates
	struct sockaddr_in sa;
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(p_port);
	sa.sin_addr.s_addr = inet_addr(p_host);

	// try to connect
	if ( connect(m_fd, (struct sockaddr *) &sa, sizeof(sa)) < 0) {
		// Close socket
		close(m_fd);
		m_fd = -1;
		getWrapper()->error(NO_VALID_ID, CONNECT_FAIL.code(), CONNECT_FAIL.msg());
		return false;
	}

	setClientId(p_clientId);
	setExtraAuth(p_extraAuth);

	onConnectBase();

	// keep checking messages from server until connect ack message is received and thus m_connected becomes true
	while (isSocketOK() && !isConnected()) {
		if (!checkMessages()) {
			// if checkMessage() returns false
			getWrapper()->error(NO_VALID_ID, CONNECT_FAIL.code(), CONNECT_FAIL.msg());
			return false;
		}
	}

	// set socket to non-blocking state
	if (!setSocketNonBlocking(m_fd)) {
		getWrapper()->error(NO_VALID_ID, CONNECT_FAIL.code(), CONNECT_FAIL.msg());
	}

	// successfully connected
	return true;
}

void ATU_IB_ClientSocket_MDI::eDisconnect()
{
	if (m_fd >= 0)
		close(m_fd);
	m_fd = -1;
	eDisconnectBase();
}

int ATU_IB_ClientSocket_MDI::fd() const
{
	return m_fd;
}

void ATU_IB_ClientSocket_MDI::startHandlingSendRecvMsg()
{
	m_sendThread = boost::thread(&ATU_IB_ClientSocket_MDI::processSendMsg, this);
	m_recvThread = boost::thread(&ATU_IB_ClientSocket_MDI::processRecvMsg, this);
}

void ATU_IB_ClientSocket_MDI::reqMktDataSync(TickerId tickerId, const Contract& contract,
								   const IBString& genericTicks, bool snapshot, const TagValueListSPtr& mktDataOptions)
{
	m_sendMutex.lock();
	reqMktData(tickerId, contract, genericTicks, snapshot, mktDataOptions);
	m_sendMutex.unlock();
	m_sendCond.notify_all();
}

void ATU_IB_ClientSocket_MDI::reqMktDepthSync( TickerId tickerId, const Contract &contract, int numRows, const TagValueListSPtr& mktDepthOptions) {
	m_sendMutex.lock();
	reqMktDepth(tickerId, contract, numRows, mktDepthOptions);
	m_sendMutex.unlock();
	m_sendCond.notify_all();
}

void ATU_IB_ClientSocket_MDI::processSendMsg()
{

	fd_set writeSet, errorSet;

	while (isSocketOK()) {

		boost::mutex::scoped_lock lock(m_sendMutex);

		if (!isOutBufferEmpty()) {

			FD_ZERO(&writeSet);
			errorSet = writeSet;

			FD_SET(fd(), &writeSet);
			FD_SET(fd(), &errorSet);

			// indicates which of the specified file descriptors is ready for reading, ready for writing, or has an error condition pending
			int ret = select(fd() + 1, (fd_set*) 0, &writeSet, &errorSet, NULL);

			// if timeout
			if (ret == 0) {
				cout << "MDI connection to TWS for sending messages is timeout...The server will stop in 5 seconds..." << endl;
				int i = 5;
				while (i > 0) {
					cout << i << endl;
					sleep(1);
					i--;
				}
				exit(1);
			}

			// if error
			if (ret < 0) {
				eDisconnect();
				return;
			}

			if (!isSocketOK())
				return;

			if (FD_ISSET(fd(), &errorSet))
				onError();

			if (!isSocketOK())
				return;

			if (FD_ISSET(fd(), &writeSet)) {
				onSend();
			}

		} else {
			m_sendCond.timed_wait(lock, boost::posix_time::milliseconds(COND_WAIT_MS));
		}

	}
}

void ATU_IB_ClientSocket_MDI::processRecvMsg()
{

	fd_set readSet, errorSet;

	while (isSocketOK()) {

		FD_ZERO(&readSet);
		errorSet = readSet;

		FD_SET(fd(), &readSet);

		FD_SET(fd(), &errorSet);

		// indicates which of the specified file descriptors is ready for reading, ready for writing, or has an error condition pending
		int ret = select(fd() + 1, &readSet, (fd_set*) 0, &errorSet, NULL);

		// if timeout
		if (ret == 0) {
			cout << "MDI connection to TWS for receiving messages is timeout...The server will stop in 5 seconds..." << endl;
			int i = 5;
			while (i > 0) {
				cout << i << endl;
				sleep(1);
				i--;
			}
			exit(1);
		}

		// if error
		if (ret < 0) {
			eDisconnect();
			return;
		}

		if (!isSocketOK())
			return;

		if (FD_ISSET(fd(), &errorSet))
			onError();

		if (!isSocketOK())
			return;

		if (FD_ISSET(fd(), &readSet))
			onReceive();
	}
}

void ATU_IB_ClientSocket_MDI::onSend()
{
	if (!handleSocketError())
		return;

	sendBufferedData();
}

void ATU_IB_ClientSocket_MDI::onReceive()
{
	if (!handleSocketError())
		return;

	m_recvMutex.lock();
	checkMessages();
	m_recvMutex.unlock();
	m_recvCond.notify_all();
}

void ATU_IB_ClientSocket_MDI::onError()
{
	handleSocketError();
}

void ATU_IB_ClientSocket_MDI::onConnect()
{
	if (!handleSocketError())
		return;

	onConnectBase();
}

void ATU_IB_ClientSocket_MDI::onClose()
{
	if (!handleSocketError())
		return;

	eDisconnect();
	getWrapper()->connectionClosed();
}

int ATU_IB_ClientSocket_MDI::send(const char *p_buf, size_t p_sz)
{
	if (p_sz < 0)
		return 0;

	int nResult = ::send(m_fd, p_buf, p_sz, 0);

	if (nResult == -1 && !handleSocketError())
		return -1;

	if (nResult <= 0)
		return 0;

	return nResult;
}

int ATU_IB_ClientSocket_MDI::receive(char *p_buf, size_t p_sz)
{
	if (p_sz < 0)
		return 0;

	int nResult = ::recv(m_fd, p_buf, p_sz, 0);

	if (nResult == -1 && !handleSocketError())
		return -1;

	if (nResult <= 0)
		return 0;

	return nResult;
}

bool ATU_IB_ClientSocket_MDI::isSocketOK() const
{
	return m_fd >= 0;
}

bool ATU_IB_ClientSocket_MDI::handleSocketError()
{
	switch (errno) {
		case 0:
			return true;
		case EISCONN:
			return true;
		case EWOULDBLOCK:
			return false;
		case ECONNREFUSED:
			getWrapper()->error(NO_VALID_ID, CONNECT_FAIL.code(), CONNECT_FAIL.msg());
			break;
		default:
			getWrapper()->error(NO_VALID_ID, SOCKET_EXCEPTION.code(), SOCKET_EXCEPTION.msg() + strerror(errno));
	}

	errno = 0;
	eDisconnect();
	return false;
}

}

