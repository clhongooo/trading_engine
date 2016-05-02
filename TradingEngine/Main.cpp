#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "TradingEngineMainThread.h"
#include <boost/scoped_ptr.hpp>

using namespace std;
using namespace boost;

int main(int argc, const char *argv[])
{
  //--------------------------------------------------
  // Input arg
  //--------------------------------------------------
  if (argc <= 1)
  {
    cout << "Usage: TradingEngine [config.ini] [(optional) csv with high / low prices]" << endl << flush;
    return 0; 
  }

  boost::scoped_ptr<TradingEngineMainThread> p_mainthd;

  if (argc == 2) p_mainthd.reset(new TradingEngineMainThread(argv[1]));
  else p_mainthd.reset(new TradingEngineMainThread(argv[1],argv[2]));

  boost::thread thread(&TradingEngineMainThread::RunMainThread, p_mainthd.get());
  thread.join();

  return 0;
}
