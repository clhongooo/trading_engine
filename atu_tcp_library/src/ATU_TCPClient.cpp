#include "ATU_TCPClient.h"
#include <iostream>
using namespace std;
ATU_TCPClient::ATU_TCPClient() : 
		m_server(""),
		m_port(""),
		m_socket(NULL),
		m_tcp_handle_msg_call_back_func(NULL), 
		m_isReady(false),
		m_shutDown(false),
		m_logfeed_call_back_func(NULL) {
  //--------------------------------------------------
  // Sunny added - [start]
  //--------------------------------------------------
  cout << "ATU TCP Client: SVN revision no. (ATU TCP Client): "   << string(__SVN_REV__)       << endl;
  cout << "ATU TCP Client: SVN revision no. (Entire workspace): " << string(__SVN_WKSPC_REV__) << endl;
  //--------------------------------------------------
  // Sunny added - [end]
  //--------------------------------------------------
	init();
}
ATU_TCPClient::ATU_TCPClient(string server, string port) : 
		m_server(server),
		m_port(port),
		m_socket(NULL),
		m_tcp_handle_msg_call_back_func(NULL), 
		m_tcp_after_connection_call_back_func(NULL), 
		m_isReady(false),
		m_shutDown(false),
		m_logfeed_call_back_func(NULL) {
	init();
}
void ATU_TCPClient::init() {
  //--------------------------------------------------
  // Sunny added - [start]
  //--------------------------------------------------
  if (m_sendQueue == NULL)
  //--------------------------------------------------
  // Sunny added - [end]
  //--------------------------------------------------
	m_sendQueue=new ATU_ThreadSafeQueue< string* >(ATU_TCP_QUEUE_SIZE);
  //--------------------------------------------------
  // Sunny added - [start]
  //--------------------------------------------------
  if (m_recvQueue == NULL)
  //--------------------------------------------------
  // Sunny added - [end]
  //--------------------------------------------------
	m_recvQueue=new ATU_ThreadSafeQueue< string* >(ATU_TCP_QUEUE_SIZE);
}
ATU_TCPClient::~ATU_TCPClient()
{
	stop();
	if (m_socket!=NULL) {
		m_isReady=false;
		delete m_socket;
		m_socket=NULL;
	}
	if (m_tcp_handle_msg_call_back_func!=NULL) {
		m_isReady=false;
		delete m_tcp_handle_msg_call_back_func;
		m_tcp_handle_msg_call_back_func=NULL;
	}
	string *strptr;
	if (m_recvQueue!=NULL) {
		while (!m_recvQueue->empty())
		{
			m_recvQueue->pop(strptr);
			delete strptr;
		}
		delete m_recvQueue;
		m_recvQueue=NULL;
	}
	if (m_sendQueue!=NULL) {
		while (!m_sendQueue->empty())
		{
			m_sendQueue->pop(strptr);
			delete strptr;
		}
		delete m_sendQueue;
		m_sendQueue=NULL;
	}

}
void ATU_TCPClient::RegisterHandleMsgCallBack(ATU_TCPReadCallBackFunc* callback) {
	m_tcp_handle_msg_call_back_func=callback;
}
void ATU_TCPClient::RegisterAfterConnectionCallBack(ATU_TCPClientAfterConnectionCallBackFunc* callback) {
	m_tcp_after_connection_call_back_func=callback;
}

void ATU_TCPClient::queuemsg(string str){
	
	if (!m_shutDown) {
		string *strptr=new string(str);

    //--------------------------------------------------
    // Sunny added - [start]
    //--------------------------------------------------
    if (m_sendQueue == NULL)
      m_sendQueue=new ATU_ThreadSafeQueue< string* >(ATU_TCP_QUEUE_SIZE);
    //--------------------------------------------------
    // Sunny added - [end]
    //--------------------------------------------------

		m_sendQueue->push(strptr);
		m_sendCond.notify_all();
//		boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
//		m_sendCond.notify_all();
	}
}
void ATU_TCPClient::sendmsg(string str) {

	if (m_socket!=NULL && m_isReady) {
		try {
			boost::asio::streambuf request;
			ostream request_stream(&request);
			request_stream << str;

//			addLog(__LOGSOURCE__,"Sending ["+str+"]",ATU_logfeed_struct::INFO);
			addLog(__LOGSOURCE__,ATU_logfeed_struct::INFO,"Sending [%s]","s",str.c_str());
			boost::asio::write(*m_socket, request);
			return;
		}
		catch(exception &e) {
			m_isReady=false;
//			addLog(__LOGSOURCE__,"ATU_TCPClient::sendmsg exception in sending msg",ATU_logfeed_struct::ALERT);
			addLog(__LOGSOURCE__,ATU_logfeed_struct::ALERT,"ATU_TCPClient::sendmsg exception in sending msg","");
			return;
		}		
	}
	return;
}
void ATU_TCPClient::handlemsg(string str) {
	if (m_tcp_handle_msg_call_back_func!=NULL) {
		(*m_tcp_handle_msg_call_back_func)(str);
	}
}
void ATU_TCPClient::detach() {
	m_recv_msg_thread=boost::thread(boost::bind(&ATU_TCPClient::connect_and_recv,this));
	m_send_msg_thread=boost::thread(boost::bind(&ATU_TCPClient::process_send_queue,this));
	m_handle_msg_thread=boost::thread(boost::bind(&ATU_TCPClient::handle_recv_queue,this));

}
void ATU_TCPClient::stop() {
	m_isReady=false;
	m_shutDown=true;
	m_recvCond.notify_all();
	m_sendCond.notify_all();	
	if (m_socket!=NULL) {
//		addLog(__LOGSOURCE__,"Close Socket...",ATU_logfeed_struct::INFO);
		addLog(__LOGSOURCE__,ATU_logfeed_struct::INFO,"Close Socket...","");

		boost::system::error_code ec;
		m_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
		m_socket->close();
		delete m_socket;
		m_socket=NULL;
	}
	m_recv_msg_thread.join();
	m_send_msg_thread.join();
	m_handle_msg_thread.join();

}
void ATU_TCPClient::handle_recv_queue() {
	while(!m_shutDown) {
		string *strptr;
		if (!m_recvQueue->empty()) {
			m_recvQueue->pop(strptr);
			handlemsg(*strptr);
			delete strptr;
		} else {
			boost::mutex::scoped_lock lock(m_recvMutex);
			m_recvCond.timed_wait(lock,boost::posix_time::milliseconds(ATU_TCP_CLIENT_COND_WAIT_MS) );
		}
	}
}
void ATU_TCPClient::process_send_queue()
{
//	addLog(__LOGSOURCE__,"",ATU_logfeed_struct::DEBUG);
	addLog(__LOGSOURCE__,ATU_logfeed_struct::DEBUG,"","");
	while(!m_shutDown) {
		string *strptr;
    //--------------------------------------------------
    // Sunny modified - [start]
    //--------------------------------------------------
		if (m_sendQueue != NULL && !m_sendQueue->empty() && m_isReady) {
    //--------------------------------------------------
    // Sunny modified - [end]
    //--------------------------------------------------
			m_sendQueue->pop(strptr);
			sendmsg(*strptr);
			delete strptr;
		} else {
			boost::mutex::scoped_lock lock(m_sendMutex);
			m_sendCond.timed_wait(lock,boost::posix_time::milliseconds(ATU_TCP_CLIENT_COND_WAIT_MS) );
		}
	}
}
void ATU_TCPClient::connect_and_recv()
{
	boost::asio::streambuf response;
	int result=0;
	while(m_socket==NULL && !m_shutDown) {
		connect();
		if (m_isReady) {
			if (m_tcp_after_connection_call_back_func!=NULL) {
				(*m_tcp_after_connection_call_back_func)();
			}
		}
		while(m_socket!=NULL && !m_shutDown) {
			if (m_socket!=NULL) {
				try {
//					boost::this_thread::sleep(boost::posix_time::milliseconds(ATU_TCP_CLIENT_RECONNECT_WAIT_MS));
//					addLog(__LOGSOURCE__,"Waiting for data...",ATU_logfeed_struct::INFO); //<< endl;
					addLog(__LOGSOURCE__,ATU_logfeed_struct::INFO,"Waiting for data...","");
					m_isReady=true;					
					result=boost::asio::read_until(*m_socket, response, "\n");
				} 
				catch(exception &e) {
					m_isReady=false;
//					addLog(__LOGSOURCE__,"Abnormal disconnected...",ATU_logfeed_struct::ALERT);
					addLog(__LOGSOURCE__,ATU_logfeed_struct::ALERT,"Abnormal disconnected...","");
					delete m_socket;
					m_socket=NULL;
//					addLog(__LOGSOURCE__,"socket set to NULL",ATU_logfeed_struct::ALERT);
					addLog(__LOGSOURCE__,ATU_logfeed_struct::ALERT,"socket set to NULL","");
				}
			}
			if (result>0 && m_socket!=NULL) {
					string tmpstr;
					//std::istream(&response) >> tmpstr;
					istream response_stream(&response);
					getline(response_stream, tmpstr);
					string *strptr=new string(tmpstr);
					addLog(__LOGSOURCE__,ATU_logfeed_struct::INFO,"Receiving [%s]","s",tmpstr.c_str());

					m_recvQueue->push(strptr);
					m_recvCond.notify_all();
				//}
			} else {
//				addLog(__LOGSOURCE__,"Socket Removed...",ATU_logfeed_struct::INFO);
				addLog(__LOGSOURCE__,ATU_logfeed_struct::INFO,"Socket Removed...","");
				m_socket=NULL;
				delete m_socket;
				m_isReady=false;
			}
		}
		boost::this_thread::sleep(boost::posix_time::milliseconds(ATU_TCP_CLIENT_RECONNECT_WAIT_MS));
	}	
}
void ATU_TCPClient::connect()
{
	m_isReady=false;
	//m_io_service.reset();
	tcp::resolver resolver(m_io_service);
    tcp::resolver::query query(m_server, m_port);
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;
	
    // Try each endpoint until we successfully establish a connection.
	/*
	int32_t timeout = 2;
    setsockopt(m_socket->native_handle(), SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    setsockopt(m_socket->native_handle(), SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
	*/

    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end) {
      if (m_socket != NULL) {
		m_socket->close();
		delete m_socket;
	  }	  
	  m_socket=new tcp::socket(m_io_service);
	  ostringstream cooo;
	  cooo << "Connecting to " << m_server << " at port " << m_port;
//	  addLog(__LOGSOURCE__,cooo.str(),ATU_logfeed_struct::INFO);
	  addLog(__LOGSOURCE__,ATU_logfeed_struct::INFO,(char*)(cooo.str().c_str()),"");
	  //boost::asio::connect(m_socket,endpoint_iterator);
	  
      m_socket->connect(*endpoint_iterator++, error);
		
    }
    if (error) {
		m_isReady=false;
//      throw boost::system::system_error(error);
		ostringstream cooo;
		cooo << "Cannot connect to " << m_server << " at port " << m_port;
//		addLog(__LOGSOURCE__,cooo.str(),ATU_logfeed_struct::ERROR);
		addLog(__LOGSOURCE__,ATU_logfeed_struct::ERROR,(char*)(cooo.str().c_str()),"");
		boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
		if (m_socket != NULL) {
			m_socket->close();
			delete m_socket;
		}	  
		
		m_socket=NULL;

	} else {
		ostringstream cooo;
		cooo << "Connected to " << m_server << " at port " << m_port;
//		addLog(__LOGSOURCE__,cooo.str(),ATU_logfeed_struct::INFO);
		addLog(__LOGSOURCE__,ATU_logfeed_struct::INFO,(char*)(cooo.str().c_str()),"");
		m_isReady=true;
		m_sendCond.notify_all();
	}
//  return true;  

}
/*
void ATU_TCPClient::addLog(string source,string str,int severity)
{
	ATU_logfeed_struct s;
	s.m_logMessage=str;
	s.m_logSeverity=severity;
	s.m_logSource=source;
	s.m_logFormat=ATU_logfeed_struct::PLAINTEXT;
	notify_logfeed(s);
}
*/
void ATU_TCPClient::addLog(string logSource,int logSeverity,char *argsfmt,char *argtype,...) {
	va_list listPointer;
	va_start(listPointer,argtype);

	ATU_logfeed_struct *logfeed=copy2logfeed_struct(logSource,
			logSeverity,argsfmt,argtype, listPointer);
	notify_logfeed(logfeed);

}

bool ATU_TCPClient::notify_logfeed(ATU_logfeed_struct* s) {
	if (m_logfeed_call_back_func!=NULL) {
		(*m_logfeed_call_back_func)(s);
	}
	return true;
}

void ATU_TCPClient::register_notify_logfeed_call_back_func(
		ATU_logfeed_CallBackFunc* callback) {
	m_logfeed_call_back_func = callback;
}
