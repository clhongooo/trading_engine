#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "TradingEngineMainThread.h"

using namespace std;
using namespace boost;

int main(int argc, const char *argv[])
{
  //--------------------------------------------------
  // Input arg
  //--------------------------------------------------
  if (argc <= 1)
  {
    cout << "Usage: TradingEngine [config.ini]" << endl << flush;
    return 0; 
  }

  TradingEngineMainThread mainthd(argv[1]);
  boost::thread thread(&TradingEngineMainThread::RunMainThread, &mainthd);
  thread.join();

  return 0;
}
