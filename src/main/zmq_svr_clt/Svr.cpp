#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/scoped_ptr.hpp>
#include "ZMQServerClient.h"

using namespace std;
using namespace boost;

int main(int argc, const char *argv[])
{
  if (argc != 2)
  {
    cout << "Usage: " << argv[0] << " [port]" << endl << flush;
    return 0; 
  }

  const string sPort(argv[1]);
  ZMQServer zsvr(sPort);

  string s;
  for (;;)
  {
    s = "Hello from svr";
    zsvr.Send(s);

    s = "";
    Tuple2<bool,string> tup = zsvr.GetStr();
    while (tup._1())
    {
      s = tup._2();
      tup = zsvr.GetStr();
    }
    if (s != "") cout << "Obtained: " << s << " send queue size: " << zsvr.GetSendQueueSize() << " recv queue size: " << zsvr.GetRecvQueueSize() << endl << flush;
    boost::this_thread::sleep(boost::posix_time::microseconds(2*1000));
  }

  return 0;
}
