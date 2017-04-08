#include <CorrelMatrices.h>

boost::weak_ptr<CorrelMatrices> CorrelMatrices::m_pInstance;

boost::shared_ptr<CorrelMatrices> CorrelMatrices::Instance() {
  boost::shared_ptr<CorrelMatrices> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new CorrelMatrices());
    m_pInstance = instance;
  }
  return instance;
}

CorrelMatrices::CorrelMatrices()
{
  m_Logger = Logger::Instance();
}

CorrelMatrices::~CorrelMatrices()
{
}


string CorrelMatrices::GenerateCombinedKey(const string & symA, const string & symB)
{
  if (symA < symB) return symA + "_" + symB;
  else             return symB + "_" + symA;
}

double CorrelMatrices::GetCorrel(const YYYYMMDD & yyyymmdd, const string & sym1,const string & sym2)
{
  return GetCorrel(yyyymmdd.ToInt(), sym1, sym2);
}

double CorrelMatrices::GetCorrel(const int yyyymmdd, const string & sym1,const string & sym2)
{
  boost::shared_lock<boost::shared_mutex> lock(m_Mutex);

  map<int,map<string,double> >::iterator it = m_CorrelMatrices.find(yyyymmdd);
  if (it == m_CorrelMatrices.end()) return UNDEFINED_CORRELATION;

  string sCombinedKey = GenerateCombinedKey(sym1,sym2);
  map<string,double> & mComSymCorrel = it->second;

  map<string,double>::iterator it2 = mComSymCorrel.find(sCombinedKey);
  if (it2 == mComSymCorrel.end()) return UNDEFINED_CORRELATION;

  return it2->second;
}

void CorrelMatrices::LoadCorrelMatrices(const string & sPath)
{
  boost::unique_lock<boost::shared_mutex> lock(m_Mutex);

  namespace fs = boost::filesystem;
  fs::path someDir(sPath);
  fs::directory_iterator end_iter;

  if (fs::exists(someDir) && fs::is_directory(someDir))
  {
    for(fs::directory_iterator dir_iter(someDir) ; dir_iter != end_iter ; ++dir_iter)
    {
      if (fs::is_regular_file(dir_iter->status()) )
      {

        //--------------------------------------------------
        // Read individual file content
        //--------------------------------------------------
        int iYYYYMMDD = boost::lexical_cast<int>(dir_iter->path().filename().string());
        m_CorrelMatrices[iYYYYMMDD] = map<string,double>();

        map<string,double> & mCorrelMatrix = m_CorrelMatrices[iYYYYMMDD];

        deque<string> dqRawFile;
        STool::ReadFile(dir_iter->path().string(),dqRawFile);
        if (dqRawFile.empty()) continue;

        //--------------------------------------------------
        // Here comes the matrix
        //--------------------------------------------------
        vector<string> vHeaderSymbols;
        if (dqRawFile.size() > 0)
        {

          {
            //--------------------------------------------------
            // Header
            //--------------------------------------------------
            vector<string> vs;
            boost::split(vs, dqRawFile[0], boost::is_any_of(","));
            for (unsigned int i = 0; i < vs.size(); ++i) // the first element is not symbol, still push in
            {
              vHeaderSymbols.push_back(vs[i]);
            }
          }

          //--------------------------------------------------
          // The rest
          //--------------------------------------------------
          for (unsigned int i = 1; i < dqRawFile.size(); ++i)
          {
            vector<string> vs;
            boost::split(vs, dqRawFile[i], boost::is_any_of(","));

            for (unsigned int i = 1; i < vs.size(); ++i)
            {
              string sCombinedKey = GenerateCombinedKey(vs[0],vHeaderSymbols[i]);
              double dCorrel = boost::lexical_cast<double>(vs[i]);
              if (-1 <= dCorrel && dCorrel <= 1)
                mCorrelMatrix[sCombinedKey] = dCorrel;
            }
          }

        }
      }
    }
  }

  return;
}

