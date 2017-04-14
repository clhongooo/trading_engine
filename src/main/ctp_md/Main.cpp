#include <boost/cstdint.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/time_duration.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

int main(int argc, const char *argv[])
{
  if (argc != 2)
  {
    cout << "Usage: " << argv[0] << " [config_file]" << endl << flush;
    return 0;
  }

  // ReadConfig(argv[1],dqIP,dqPort,dqLabel);

  return 0;
}
