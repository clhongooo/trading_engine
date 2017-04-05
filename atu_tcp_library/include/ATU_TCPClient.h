#ifndef _ATU_TCPCLIENT_
#define _ATU_TCPCLIENT_
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <boost/function.hpp>
#include "ATU_ThreadSafeQueue.h"
#include "ATU_Logger.h"
#define ATU_TCP_QUEUE_SIZE 10000
#define ATU_TCP_CLIENT_COND_WAIT_MS 1 // 10000
#define ATU_TCP_CLIENT_RECONNECT_WAIT_MS 1
using boost::asio::ip::tcp;
using namespace std;
typedef boost::function<void(string str)> ATU_TCPReadCallBackFunc;
typedef boost::function<void()> ATU_TCPClientAfterConnectionCallBackFunc;
typedef boost::function<void(string str)> ATU_TCPLogCallBackFunc;

class ATU_TCPClient {
public:
	ATU_TCPClient();
	ATU_TCPClient(string server, string port);
	virtual ~ATU_TCPClient();
	void RegisterHandleMsgCallBack(ATU_TCPReadCallBackFunc* callback);
	void RegisterAfterConnectionCallBack(ATU_TCPClientAfterConnectionCallBackFunc* callback);
	void RegisterLogCallBack(ATU_TCPLogCallBackFunc* callback);
	
	void queuemsg(string str);
	
	virtual void detach();
	void stop(); // stop everything and exit thread
	bool isReady() {
		return m_isReady;
	}
	boost::asio::io_service m_io_service;
//	boost::lockfree::queue< int > m_lockfree_queue;
	virtual void init();
	virtual void register_notify_logfeed_call_back_func(ATU_logfeed_CallBackFunc *callback);
	virtual bool notify_logfeed(ATU_logfeed_struct *s);

protected:
//private:
	void connect();
	void sendmsg(string str);
	void handlemsg(string str);
	virtual void connect_and_recv();
	void process_send_queue();
	void handle_recv_queue();
	void addLog(const string & logSource,int logSeverity,const char *argsfmt,const char *argtype,...);

	tcp::socket *m_socket;
	string m_server;
	string m_port;
	boost::thread m_recv_msg_thread;
	boost::thread m_send_msg_thread;
	boost::thread m_handle_msg_thread;
	ATU_TCPReadCallBackFunc *m_tcp_handle_msg_call_back_func;
	ATU_TCPClientAfterConnectionCallBackFunc *m_tcp_after_connection_call_back_func;
	ATU_logfeed_CallBackFunc *m_logfeed_call_back_func;

	ATU_ThreadSafeQueue<string* > *m_sendQueue;
	ATU_ThreadSafeQueue<string* > *m_recvQueue;
	boost::condition_variable m_sendCond;
	boost::condition_variable m_recvCond;
	boost::mutex m_sendMutex;
	boost::mutex m_recvMutex;
	bool m_isReady;
	bool m_shutDown;

};
#endif //_ATU_TCPCLIENT_
