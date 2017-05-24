#include "ZMQServerClient.h"

ZMQServerClientBase::ZMQServerClientBase()
{
  //--------------------------------------------------
  m_send_queue.reset(new ExpandableCirBuffer(1,4096,81920,CentralMemMgr::Instance()));
  m_recv_queue.reset(new ExpandableCirBuffer(1,4096,81920,CentralMemMgr::Instance()));
  //--------------------------------------------------
  m_zmqcontext.reset(new zmq::context_t(1));
  m_zmqsocket.reset(new zmq::socket_t(*m_zmqcontext, ZMQ_PAIR));

  const int val = 0;
  const int rc = zmq_setsockopt (*m_zmqsocket, ZMQ_LINGER, &val, sizeof(val));
  assert (rc == 0);
}

ZMQServer::ZMQServer(const string & s_Port) : ZMQServerClientBase()
{
  const string sConn = "tcp://*:"+s_Port;
  m_zmqsocket->bind(sConn.c_str());
}

ZMQClient::ZMQClient(const string & s_IP_Port) : ZMQServerClientBase()
{
  const string sConn = "tcp://"+s_IP_Port;
  m_zmqsocket->connect(sConn.c_str());
}

void ZMQServerClientBase::Send(const string & s)
{
  const size_t sz = s.length();
  BYTE * pw = m_send_queue->GetWritingPtr();
  memcpy((char*)pw,(char*)s.c_str(),sz);
  ((char*)pw)[sz] = '\0';
  m_send_queue->PushBack(sz+1);
}

void ZMQServerClientBase::Run()
{
  p_thd.reset(new boost::thread(&ZMQServerClientBase::SendRecvLoop, this));
}


void ZMQServerClientBase::KeepWaitingForRead()
{
  zmq::pollitem_t m_pollitems [] = {
    { *m_zmqsocket, 0, ZMQ_POLLIN, 0 }
  };
  do
  {
    zmq::poll (&m_pollitems[0], 1, -1);
    boost::this_thread::sleep(boost::posix_time::microseconds(10*1000));
  } while (!(m_pollitems[0].revents & ZMQ_POLLIN));
}

void ZMQServerClientBase::KeepWaitingForWrite()
{
  zmq::pollitem_t m_pollitems [] = {
    { *m_zmqsocket, 0, ZMQ_POLLOUT, 0 }
  };
  do
  {
    zmq::poll (&m_pollitems[0], 1, -1);
    boost::this_thread::sleep(boost::posix_time::microseconds(10*1000));
  } while (!(m_pollitems[0].revents & ZMQ_POLLOUT));
}


void ZMQServerClientBase::SendRecvLoop()
{
  zmq::message_t zmqm;
  unsigned long ulTS;
  BYTE * pb;

  for (;;)
  {
    //--------------------------------------------------
    // send first
    //--------------------------------------------------
    if (m_send_queue->GetReadingPtrTStamp(pb,&ulTS))
    {
      const size_t sz = m_send_queue->GetPktSize();
      zmq::message_t zmq_send_msg(sz);
      memcpy((void *)zmq_send_msg.data(),pb,sz);
      KeepWaitingForWrite();
      m_zmqsocket->send(zmq_send_msg);
      // m_zmqsocket->send(zmq_send_msg, ZMQ_NOBLOCK);
      m_send_queue->PopFront();
    }

    //--------------------------------------------------
    // recv
    //--------------------------------------------------
    // KeepWaitingForRead();
    const int rc = m_zmqsocket->recv(&zmqm, ZMQ_DONTWAIT);
    const size_t sz = zmqm.size();
    if (rc <= 0) continue;
    BYTE * pw = m_recv_queue->GetWritingPtr();
    memcpy(pw,(char*)zmqm.data(),sz);
    ((char*)pw)[sz] = '\0';
    m_recv_queue->PushBack(sz);
  }
}

Tuple2<bool,string> ZMQServerClientBase::GetStr()
{
  unsigned long ulTS;
  BYTE * pb;
  if (m_recv_queue->GetReadingPtrTStamp(pb,&ulTS))
  {
    const string sRtn((char*)pb,m_recv_queue->GetPktSize());
    m_recv_queue->PopFront();
    return Tuple2<bool,string>(true,sRtn);
  }
  else
  {
    return Tuple2<bool,string>(false,"");
  }
}

const size_t ZMQServerClientBase::GetSendQueueSize()
{
  return m_send_queue->Size();
}

const size_t ZMQServerClientBase::GetRecvQueueSize()
{
  return m_recv_queue->Size();
}

