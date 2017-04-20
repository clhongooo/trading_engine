#include <boost/cstdint.hpp>
#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <boost/date_time/time_duration.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string.hpp>

#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

#include "CtpMd.h"

using namespace std;
using namespace boost;

int main(int argc, const char *argv[])
{
  if (argc != 2)
  {
    cerr << "Usage: " << argv[0] << " [config_file]" << endl << flush;
    return 0;
  }

  boost::shared_ptr<CtpMd> p_ctpmd(new CtpMd());
  p_ctpmd->ReadConfig(argv[1]);

  boost::scoped_ptr<boost::thread> m_p_init_and_run_thread(new boost::thread(boost::bind(&CtpMd::run,p_ctpmd.get())));
  m_p_init_and_run_thread->join();

  return 0;
}
