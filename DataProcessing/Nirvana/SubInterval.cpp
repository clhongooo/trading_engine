#include "SubInterval.h"

SubInterval::SubInterval() {
}

SubInterval::~SubInterval() {
}

void SubInterval::Convert(const char * sInfile,const char * sOutfile,const char * sIntervalA)
{
  string sInterval(sIntervalA);
  int iInterval = boost::lexical_cast<int>(sInterval);

  long lCount = 0;

  ofstream outFileStrm(sOutfile,std::ifstream::out);

  std::ifstream filein(sInfile);
  for (std::string line; std::getline(filein, line); )
  {
    if (lCount % iInterval == 0)
      outFileStrm << line << std::endl;
    lCount++;
  }


  outFileStrm.close();

  return;

}
