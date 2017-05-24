#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/scoped_ptr.hpp>
#include "ZMQServerClient.h"
#include "SFunctional.h"

using namespace std;
using namespace boost;

int main(int argc, const char *argv[])
{
  if (argc != 2)
  {
    cout << "Usage: " << argv[0] << " [ip:port]" << endl << flush;
    return 0; 
  }

  const string sIPPort(argv[1]);
  ZMQClient zclt(sIPPort);
  zclt.Run();

  string s;
  for (;;)
  {
    s = "Hello from clt";
    zclt.Send(s);

    s = "";
    Tuple2<bool,string> tup = zclt.GetStr();
    while (tup._1())
    {
      s = tup._2();
      tup = zclt.GetStr();
    }
    if (s != "") cout << "Obtained: " << s << " send queue size: " << zclt.GetSendQueueSize() << " recv queue size: " << zclt.GetRecvQueueSize() << endl << flush;
    boost::this_thread::sleep(boost::posix_time::microseconds(2*1000));
  }

  return 0;
}
