//--------------------------------------------------
// The reason why 2 ExpandableCirBuffer's are used is that if we relied on zmq's socket queue,
// We will have a chance of calling the same socket for both send and receive simultaneously,
// which cases the program to crash.
//--------------------------------------------------
#ifndef ZMQSERVERCLIENTTHREAD_H_
#define ZMQSERVERCLIENTTHREAD_H_

#include "PCH.h"
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <string>
#include <zmq.hpp>
#include "CentralMemMgr.h"
#include "BinaryTools.h"
#include "ExpandableCirBuffer.h"
#include "SFunctional.h"

using namespace boost;

class ZMQServerClientBase {
  public:
    ZMQServerClientBase();
    virtual ~ZMQServerClientBase() {}
    virtual void Send(const string &);
    virtual Tuple2<bool,string> GetStr();
    const size_t GetSendQueueSize();
    const size_t GetRecvQueueSize();
    virtual void Set(const string &) = 0;
  protected:
    virtual void SendRecvLoop();
    virtual void KeepWaitingForRead();
    virtual void KeepWaitingForWrite();
    virtual void Run();
    boost::scoped_ptr<zmq::context_t> m_zmqcontext;
    boost::scoped_ptr<zmq::socket_t>  m_zmqsocket;
    boost::scoped_ptr<ExpandableCirBuffer> m_recv_queue;
    boost::scoped_ptr<ExpandableCirBuffer> m_send_queue;
    boost::scoped_ptr<boost::thread> p_thd;
};

class ZMQServer : public ZMQServerClientBase {
  public:
    ZMQServer();
    ZMQServer(const string &);
    void Set(const string &);
};

class ZMQClient : public ZMQServerClientBase {
  public:
    ZMQClient();
    ZMQClient(const string &);
    void Set(const string &);
};
#endif
