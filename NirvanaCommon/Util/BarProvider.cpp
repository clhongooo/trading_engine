#include "BarProvider.h"

using namespace std;

void BarProvider::Reset()
{

  m_bMaster    = true;
  m_iPsn       = -1;
  m_iNumOfBars = -1;
  m_cFwdBkwd   = ' ';

  m_viYMD      = NULL;
  m_viTime     = NULL;
  m_vdOpen     = NULL;
  m_vdHigh     = NULL;
  m_vdLow      = NULL;
  m_vdClose    = NULL;
  m_vlVolume   = NULL;

  m_ColumnPresent_A = false;
  m_ColumnPresent_C = false;
  m_ColumnPresent_D = false;
  m_ColumnPresent_H = false;
  m_ColumnPresent_L = false;
  m_ColumnPresent_O = false;
  m_ColumnPresent_T = false;
  m_ColumnPresent_V = false;
  m_ColumnPresent_x = false;

  m_DataModified = false;

  FreeVec();
}

BarProvider::BarProvider()
{
  Reset();
}

BarProvider::BarProvider(
    const char *ccPath,
    const string &sFormat, 
    int iRndDecimalPlc, 
    char cReverseInputOrder, 
    char cHeader,
    char cCompressionType)
{
  Reset();
  LoadDataFile(
      ccPath,
      sFormat,
      iRndDecimalPlc,
      cReverseInputOrder,
      cHeader,
      cCompressionType);
}


void BarProvider::MakeDeepCopy(const BarProvider & rhs)
{
  m_bMaster         = false;
  m_iPsn            = rhs.m_iPsn;
  m_iNumOfBars      = rhs.m_iNumOfBars;
  m_cFwdBkwd        = rhs.m_cFwdBkwd;

  m_viYMD           = rhs.m_viYMD;
  m_viTime          = rhs.m_viTime;
  m_vdOpen          = rhs.m_vdOpen;
  m_vdHigh          = rhs.m_vdHigh;
  m_vdLow           = rhs.m_vdLow;
  m_vdClose         = rhs.m_vdClose;
  m_vlVolume        = rhs.m_vlVolume;

  m_ColumnPresent_A = rhs.m_ColumnPresent_A;
  m_ColumnPresent_C = rhs.m_ColumnPresent_C;
  m_ColumnPresent_D = rhs.m_ColumnPresent_D;
  m_ColumnPresent_H = rhs.m_ColumnPresent_H;
  m_ColumnPresent_L = rhs.m_ColumnPresent_L;
  m_ColumnPresent_O = rhs.m_ColumnPresent_O;
  m_ColumnPresent_T = rhs.m_ColumnPresent_T;
  m_ColumnPresent_V = rhs.m_ColumnPresent_V;
  m_ColumnPresent_x = rhs.m_ColumnPresent_x;

}

BarProvider::BarProvider(const BarProvider & rhs)
{
  MakeDeepCopy(rhs);
}

const BarProvider& BarProvider::operator=(const BarProvider & rhs)
{
  MakeDeepCopy(rhs);
  return *this;
}


void BarProvider::FreeVec() {
  if (m_bMaster)
  {
    if (m_viYMD != NULL)
    {
      m_viYMD->clear();
      delete m_viYMD; m_viYMD = NULL;
    }
    if (m_viTime != NULL)
    {
      m_viTime->clear();
      delete m_viTime; m_viTime = NULL;
    }
    if (m_vdOpen   != NULL)  { delete m_vdOpen;   m_vdOpen   = NULL;    }
    if (m_vdHigh   != NULL)  { delete m_vdHigh;   m_vdHigh   = NULL;    }
    if (m_vdLow    != NULL)  { delete m_vdLow;    m_vdLow    = NULL;    }
    if (m_vdClose  != NULL)  { delete m_vdClose;  m_vdClose  = NULL;    }
    if (m_vlVolume != NULL)  { delete m_vlVolume; m_vlVolume = NULL;    }
  }
}
// BarProvider & operator=(const BarProvider &rhs)

BarProvider::~BarProvider() {
  FreeVec();
}


void BarProvider::LoadDataFile(
    const char *ccPath,
    const string &sFormat,
    int iRndDecimalPlc,
    char cFwdBkwd, 
    char cHeader,
    char cCompressionType)
{
  boost::scoped_ptr<boost::unique_lock<shared_mutex> > lock; if (m_DataModified) lock.reset(new boost::unique_lock<shared_mutex>(m_BarProvider_Mutex));

  m_bMaster = true;

  // Won't call LoadDataFile() very often, can afford to new vectors here
  // enough size so as to avoid reallocation
  int iReserveSize = 0;
  if (sFormat.find("T") == string::npos)
  {
    iReserveSize = 252*100; // 100 years worth of daily data
  }
  else
  {
    iReserveSize = 60*24*23*12*10; // 10 years worth of 1-min bar in the US CME market, 24 hours / day, 23 days / mth
  }

  if (m_viYMD    == NULL) { m_viYMD    = new vector<YYYYMMDD>(); m_viYMD   ->reserve(iReserveSize);       }
  if (m_viTime   == NULL) { m_viTime   = new vector<HHMMSS>();   m_viTime  ->reserve(iReserveSize);       }
  if (m_vdOpen   == NULL) { m_vdOpen   = new vector<double>();   m_vdOpen  ->reserve(iReserveSize);       }
  if (m_vdHigh   == NULL) { m_vdHigh   = new vector<double>();   m_vdHigh  ->reserve(iReserveSize);       }
  if (m_vdLow    == NULL) { m_vdLow    = new vector<double>();   m_vdLow   ->reserve(iReserveSize);       }
  if (m_vdClose  == NULL) { m_vdClose  = new vector<double>();   m_vdClose ->reserve(iReserveSize);       }
  if (m_vlVolume == NULL) { m_vlVolume = new vector<long>();     m_vlVolume->reserve(iReserveSize);       }

  if (
      (sFormat == "")
      ||
      (cCompressionType != 'G' && cCompressionType != 'P') ||
      (cFwdBkwd != 'F' && cFwdBkwd != 'B') ||
      (cHeader != 'H' && cHeader != 'N')
     )
  {
    FreeVec();
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }

  m_cFwdBkwd = cFwdBkwd;

  ifstream ifs(ccPath, std::ios_base::in | std::ios_base::binary);

  try
  {
    boost::iostreams::filtering_istream fis;

    if (cCompressionType == 'G') fis.push(boost::iostreams::gzip_decompressor());
    fis.push(ifs);

    int iLineNum = 0;


    for (string str; getline(fis, str); )
    {
      ++iLineNum;
      if (cHeader == 'H' && iLineNum == 1) continue; // Skip header row

      vector<string> vs;
      boost::split(vs, str, boost::is_any_of(","));

      if (vs.size() < sFormat.length())
      {
        FreeVec();
        SException se;
        se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
        throw se;
      }

      for (unsigned int i = 0; i < sFormat.size(); ++i)
      {
        char cColSpec = sFormat.at(i);

        switch(cColSpec)
        {
          case 'D':
            {
              m_viYMD->push_back(YYYYMMDD(vs[i]));
              break;
            }
          case 'T':
            {
              m_viTime->push_back(HHMMSS(vs[i]));
              break;
            }
          case 'O':
            {
              // m_vdOpen->push_back(boost::lexical_cast<double>(vs[i]));
              m_vdOpen->push_back(STool::ToDbl(vs[i]));
              break;
            }
          case 'H':
            {
              // m_vdHigh->push_back(boost::lexical_cast<double>(vs[i]));
              m_vdHigh->push_back(STool::ToDbl(vs[i]));
              break;
            }
          case 'L':
            {
              // m_vdLow->push_back(boost::lexical_cast<double>(vs[i]));
              m_vdLow->push_back(STool::ToDbl(vs[i]));
              break;
            }
          case 'C':
            {
              // m_vdClose->push_back(boost::lexical_cast<double>(vs[i]));
              m_vdClose->push_back(STool::ToDbl(vs[i]));
              break;
            }
          case 'V':
            {
              // m_vlVolume->push_back(boost::lexical_cast<long>(vs[i]));
              m_vlVolume->push_back(STool::ToDbl(vs[i]));
              break;
            }
          default:
            {
              break;
            }
        }
      }

    }


    if (m_viTime->empty())
    {
      m_viTime->insert(m_viTime->begin(),m_viYMD->size(),HHMMSS::BAD_TIME);
    }

    if (m_vlVolume->empty())
    {
      m_vlVolume->insert(m_vlVolume->begin(),m_viYMD->size(),1);
    }

    if (m_vdOpen->empty()) m_vdOpen->insert(m_vdOpen->begin(), m_vdClose->begin(),m_vdClose->end());
    if (m_vdHigh->empty()) m_vdHigh->insert(m_vdHigh->begin(), m_vdClose->begin(),m_vdClose->end());
    if (m_vdLow ->empty()) m_vdLow ->insert(m_vdLow ->begin(), m_vdClose->begin(),m_vdClose->end());


    if (m_viYMD->size() != m_viTime->size() ||
        m_viYMD->size() != m_vdOpen->size() ||
        m_viYMD->size() != m_vdHigh->size() ||
        m_viYMD->size() != m_vdLow->size() ||
        m_viYMD->size() != m_vdClose->size() ||
        m_viYMD->size() != m_vlVolume->size() )
    {
      cerr << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << m_viYMD->size() << endl << flush;
      cerr << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << m_viTime->size() << endl << flush;
      cerr << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << m_vdOpen->size() << endl << flush;
      cerr << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << m_vdHigh->size() << endl << flush;
      cerr << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << m_vdLow->size() << endl << flush;
      cerr << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << m_vdClose->size() << endl << flush;
      cerr << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") " << m_vlVolume->size() << endl << flush;
      FreeVec();
      SException se;
      se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
      throw se;
    }
    else m_iNumOfBars = m_viYMD->size();

    if (cFwdBkwd == 'F') m_iPsn = -1;
    else m_iPsn = m_iNumOfBars;

    if (iLineNum == 0)
    {
      FreeVec();
      SException se;
      se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
      throw se;
    }
  }
  catch(const boost::iostreams::gzip_error& e)
  {
    cout << e.what() << endl;
    {
      FreeVec();
      SException se;
      se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
      throw se;
    }
  }
}


// void BarProvider::LoadDataFileOld(
//     const char *ccPath,
//     const string &sFormat,
//     int iRndDecimalPlc,
//     char cFwdBkwd, 
//     char cHeader,
//     char cCompressionType)
// {
//   m_bMaster = true;
//
//   // Won't call LoadDataFile() very often, can afford to new vectors here
//   int iReserveSize = 60*8*22*12*10;
//
//   if (m_viYMD    == NULL) { m_viYMD    = new vector<YYYYMMDD>(); m_viYMD->reserve(iReserveSize);  }
//   if (m_viTime   == NULL) { m_viTime   = new vector<HHMMSS>(); m_viTime->reserve(iReserveSize);   }
//   if (m_vdOpen   == NULL) { m_vdOpen   = new vector<double>(); m_vdOpen->reserve(iReserveSize);   }
//   if (m_vdHigh   == NULL) { m_vdHigh   = new vector<double>(); m_vdHigh->reserve(iReserveSize);   }
//   if (m_vdLow    == NULL) { m_vdLow    = new vector<double>(); m_vdLow->reserve(iReserveSize);    }
//   if (m_vdClose  == NULL) { m_vdClose  = new vector<double>(); m_vdClose->reserve(iReserveSize);  }
//   if (m_vdReturn == NULL) { m_vdReturn = new vector<double>(); m_vdReturn->reserve(iReserveSize); }
//   if (m_vlVolume == NULL) { m_vlVolume = new vector<int>(); m_vlVolume->reserve(iReserveSize);    }
//
//   if (
//       ((((((((
//               sFormat.compare(BarProvider::_sDOHLC) != 0) &&
//              sFormat.compare(BarProvider::_sDOHLCV) != 0) &&
//             sFormat.compare(BarProvider::_sDxOHLC) != 0) &&
//            sFormat.compare(BarProvider::_sDxOHLCV) != 0) &&
//           sFormat.compare(BarProvider::_sDOHLCVA) != 0) &&
//          sFormat.compare(BarProvider::_sDTOHLC) != 0) &&
//         sFormat.compare(BarProvider::_sDTOHLCV) != 0) &&
//        sFormat.compare(BarProvider::_sDTOHLCVA) != 0)
//       ||
//       (cCompressionType != 'G' && cCompressionType != 'P') ||
//       (cFwdBkwd != 'F' && cFwdBkwd != 'B') ||
//       (cHeader != 'H' && cHeader != 'N'))
//   {
//     FreeVec();
//     SException se;
//     se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
//     throw se;
//   }
//
//   m_cFwdBkwd = cFwdBkwd;
//
//   ifstream ifs(ccPath, std::ios_base::in | std::ios_base::binary);
//
//   try {
//     boost::iostreams::filtering_istream fis;
//
//
//     if (cCompressionType == 'G') fis.push(boost::iostreams::gzip_decompressor());
//     fis.push(ifs);
//
//     int iLineNum = 0;
//
//
//     for (string str; getline(fis, str); )
//     {
//       ++iLineNum;
//       if (cHeader == 'H' && iLineNum == 1) continue; // Skip header row
//
//       string sDate;
//       string sTime="";
//       double dOHLCVA[6];
//       Tokenizer tknzr(str, ",");
//
//       if (tknzr.NextToken()) sDate = tknzr.GetToken();
//       if ((sFormat.compare(BarProvider::_sDTOHLC) == 0 ||
//             sFormat.compare(BarProvider::_sDTOHLCV) == 0 ||
//             sFormat.compare(BarProvider::_sDTOHLCVA) == 0)
//           && tknzr.NextToken())
//         sTime = tknzr.GetToken();
//
//       unsigned int iCnt = 0;
//       unsigned int iToRead = 6;
//       if      (sFormat.compare(BarProvider::_sDOHLC) == 0) iToRead = 4;
//       else if (sFormat.compare(BarProvider::_sDOHLCV) == 0) iToRead = 5;
//       else if (sFormat.compare(BarProvider::_sDxOHLC) == 0) iToRead = 4;
//       else if (sFormat.compare(BarProvider::_sDxOHLCV) == 0) iToRead = 5;
//       else if (sFormat.compare(BarProvider::_sDOHLCVA) == 0) iToRead = 6;
//       else if (sFormat.compare(BarProvider::_sDTOHLC) == 0) iToRead = 4;
//       else if (sFormat.compare(BarProvider::_sDTOHLCV) == 0) iToRead = 5;
//       else if (sFormat.compare(BarProvider::_sDTOHLCVA) == 0) iToRead = 6;
//
//       //--------------------------------------------------
//       // To skip the useless field after Date
//       //--------------------------------------------------
//       if (sFormat.compare(BarProvider::_sDxOHLC) == 0 || sFormat.compare(BarProvider::_sDxOHLCV) == 0)
//       {
//         if (tknzr.NextToken())
//           tknzr.GetToken();
//       }
//
//       while (tknzr.NextToken() && iCnt < iToRead)
//       {
//         dOHLCVA[iCnt] = STool::ToDbl((string)(tknzr.GetToken()), (unsigned)iRndDecimalPlc);
//         ++iCnt;
//       }
//
//       // If the data is in reverse order, then of course the vectors will contain reverse order data
//       m_viYMD->push_back(YYYYMMDD(sDate));
//       if (
//           sFormat.compare(BarProvider::_sDTOHLC) == 0 ||
//           sFormat.compare(BarProvider::_sDTOHLCV) == 0 ||
//           sFormat.compare(BarProvider::_sDTOHLCVA) == 0)
//       {
//         m_viTime->push_back(HHMMSS(sTime));
//       }
//       else
//       {
//         m_viTime->push_back(HHMMSS::BAD_TIME);
//       }
//
//       if (!m_vdClose->empty())
//       {
//         if (m_cFwdBkwd == 'F')
//         {
//           if (sFormat.compare(BarProvider::_sDOHLCVA) == 0 ||
//               sFormat.compare(BarProvider::_sDTOHLCVA) == 0)
//             m_vdReturn->push_back(dOHLCVA[5] / m_vdClose->back());
//           else
//             m_vdReturn->push_back(dOHLCVA[3] / m_vdClose->back());
//         }
//         else if (m_cFwdBkwd == 'B')
//         {
//           if (sFormat.compare(BarProvider::_sDOHLCVA) == 0 ||
//               sFormat.compare(BarProvider::_sDTOHLCVA) == 0)
//             m_vdReturn->push_back(m_vdClose->back() / dOHLCVA[5]);
//           else
//             m_vdReturn->push_back(m_vdClose->back() / dOHLCVA[3]);
//         }
//       }
//       else
//       {
//         if (m_cFwdBkwd == 'F') m_vdReturn->push_back(0);
//       }
//
//       if (sFormat.compare(BarProvider::_sDOHLCVA) == 0 ||
//           sFormat.compare(BarProvider::_sDTOHLCVA) == 0)
//       {
//         m_vdOpen->push_back(dOHLCVA[5]);
//         m_vdHigh->push_back(dOHLCVA[5]);
//         m_vdLow->push_back(dOHLCVA[5]);
//         m_vdClose->push_back(dOHLCVA[5]);
//         m_vlVolume->push_back(dOHLCVA[4]);
//       }
//       else
//       {
//         m_vdOpen->push_back(dOHLCVA[0]);
//         m_vdHigh->push_back(dOHLCVA[1]);
//         m_vdLow->push_back(dOHLCVA[2]);
//         m_vdClose->push_back(dOHLCVA[3]);
//         if (sFormat.compare(BarProvider::_sDOHLC) == 0 ||
//             sFormat.compare(BarProvider::_sDTOHLC) == 0 ||
//             sFormat.compare(BarProvider::_sDxOHLC) == 0
//            )
//           m_vlVolume->push_back(1);
//         else
//           m_vlVolume->push_back(dOHLCVA[4]);
//       }
//
//     }
//
//     if (m_cFwdBkwd == 'B') m_vdReturn->push_back(0);
//
//     if (m_viYMD->size() != m_viTime->size() ||
//         m_viYMD->size() != m_vdOpen->size() ||
//         m_viYMD->size() != m_vdHigh->size() ||
//         m_viYMD->size() != m_vdLow->size() ||
//         m_viYMD->size() != m_vdClose->size() ||
//         //m_viYMD->size() != m_vdReturn->size() ||
//         m_viYMD->size() != m_vlVolume->size() )
//     {
//       FreeVec();
//       SException se;
//       se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
//       throw se;
//     }
//     else m_iNumOfBars = m_viYMD->size();
//
//     if (cFwdBkwd == 'F') m_iPsn = -1;
//     else m_iPsn = m_iNumOfBars;
//
//     if (iLineNum == 0)
//     {
//       FreeVec();
//       SException se;
//       se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
//       throw se;
//     }
//   }
//   catch(const boost::iostreams::gzip_error& e)
//   {
//     cout << e.what() << endl;
//     {
//       FreeVec();
//       SException se;
//       se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
//       throw se;
//     }
//   }
//
// }

void BarProvider::ResetPsn()
{
  boost::scoped_ptr<boost::unique_lock<shared_mutex> > lock; if (m_DataModified) lock.reset(new boost::unique_lock<shared_mutex>(m_BarProvider_Mutex));

  if (m_cFwdBkwd == 'F') m_iPsn = -1;
  else m_iPsn = m_iNumOfBars;
}

int BarProvider::GetNumOfBars()
{
  boost::scoped_ptr<boost::unique_lock<shared_mutex> > lock; if (m_DataModified) lock.reset(new boost::unique_lock<shared_mutex>(m_BarProvider_Mutex));

  return m_iNumOfBars;
}

bool BarProvider::HasNextBar()
{
  boost::scoped_ptr<boost::unique_lock<shared_mutex> > lock; if (m_DataModified) lock.reset(new boost::unique_lock<shared_mutex>(m_BarProvider_Mutex));

  if (m_cFwdBkwd == 'F')
  {
    if (m_iPsn < m_iNumOfBars-1) return true;
    else return false;
  }
  else
  {
    if (m_iPsn > 0) return true;
    else return false;
  }
}

void BarProvider::MoveNext()
{
  boost::scoped_ptr<boost::unique_lock<shared_mutex> > lock; if (m_DataModified) lock.reset(new boost::unique_lock<shared_mutex>(m_BarProvider_Mutex));

  if (m_cFwdBkwd == 'F') ++m_iPsn; else --m_iPsn;

  // We expect that HasNextBar() is called before MoveNext()
  if ((m_cFwdBkwd == 'F' && m_iPsn >= m_iNumOfBars) || (m_cFwdBkwd == 'B' && m_iPsn < 0))
  {
    // FreeVec();
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }
}

//returns -1 if there is no more bars
const YYYYMMDD BarProvider::PeekNextYMD()
{
  boost::scoped_ptr<boost::unique_lock<shared_mutex> > lock; if (m_DataModified) lock.reset(new boost::unique_lock<shared_mutex>(m_BarProvider_Mutex));

  if (HasNextBar())
  {
    if (m_cFwdBkwd == 'F') return ((*m_viYMD)[m_iPsn+1]);
    else return ((*m_viYMD)[m_iPsn-1]);
  }
  else return YYYYMMDD::BAD_DATE;
}

const YYYYMMDD BarProvider::PeekPrevYMD()
{
  boost::scoped_ptr<boost::unique_lock<shared_mutex> > lock; if (m_DataModified) lock.reset(new boost::unique_lock<shared_mutex>(m_BarProvider_Mutex));

  if (m_cFwdBkwd == 'F' && m_iPsn > 0) return (*m_viYMD)[m_iPsn-1];
  else if (m_cFwdBkwd == 'B' && m_iPsn < m_iNumOfBars-1) return (*m_viYMD)[m_iPsn+1];
  else
  {
    FreeVec();
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }
}

const double BarProvider::PeekPrevClose()
{
  boost::scoped_ptr<boost::unique_lock<shared_mutex> > lock; if (m_DataModified) lock.reset(new boost::unique_lock<shared_mutex>(m_BarProvider_Mutex));

  if (m_cFwdBkwd == 'F' && m_iPsn > 0) return (*m_vdClose)[m_iPsn-1];
  else if (m_cFwdBkwd == 'B' && m_iPsn < m_iNumOfBars-1) return (*m_vdClose)[m_iPsn+1];
  else
  {
    FreeVec();
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }
}

bool BarProvider::GetCloseArr(const int iLen, vector<double> & vdOut)
{
  boost::scoped_ptr<boost::unique_lock<shared_mutex> > lock; if (m_DataModified) lock.reset(new boost::unique_lock<shared_mutex>(m_BarProvider_Mutex));

  if (iLen > 0)
  {
    vdOut.clear();
    vdOut.reserve(iLen);
    if (m_cFwdBkwd == 'F' && m_iPsn >= iLen-1)
    {
      vector<double>::iterator it = (*m_vdClose).begin() + (m_iPsn-iLen+1);
      vdOut.insert(vdOut.begin(),it,(*m_vdClose).end());
      return true;
    }
    else if (m_cFwdBkwd == 'B')
    {
      vector<double>::iterator it = (*m_vdClose).begin() + (m_iPsn);
      vdOut.insert(vdOut.begin(),it,(*m_vdClose).end());
      return true;
    }
    return false;
  }
  else
  {
    return false;
  }
}


const YYYYMMDD & BarProvider::GetYYYYMMDD()
{
  boost::scoped_ptr<boost::unique_lock<shared_mutex> > lock; if (m_DataModified) lock.reset(new boost::unique_lock<shared_mutex>(m_BarProvider_Mutex));

  return (*m_viYMD)[m_iPsn];
}

const HHMMSS & BarProvider::GetHHMMSS()
{
  boost::scoped_ptr<boost::unique_lock<shared_mutex> > lock; if (m_DataModified) lock.reset(new boost::unique_lock<shared_mutex>(m_BarProvider_Mutex));

  return (*m_viTime)[m_iPsn];
}

const double BarProvider::Open()
{
  boost::scoped_ptr<boost::unique_lock<shared_mutex> > lock; if (m_DataModified) lock.reset(new boost::unique_lock<shared_mutex>(m_BarProvider_Mutex));

  return (*m_vdOpen)[m_iPsn];
}

const double BarProvider::High()
{
  boost::scoped_ptr<boost::unique_lock<shared_mutex> > lock; if (m_DataModified) lock.reset(new boost::unique_lock<shared_mutex>(m_BarProvider_Mutex));

  return (*m_vdHigh)[m_iPsn];
}

const double BarProvider::Low()
{
  boost::scoped_ptr<boost::unique_lock<shared_mutex> > lock; if (m_DataModified) lock.reset(new boost::unique_lock<shared_mutex>(m_BarProvider_Mutex));

  return (*m_vdLow)[m_iPsn];
}

const double BarProvider::Close()
{
  boost::scoped_ptr<boost::unique_lock<shared_mutex> > lock; if (m_DataModified) lock.reset(new boost::unique_lock<shared_mutex>(m_BarProvider_Mutex));

  return (*m_vdClose)[m_iPsn];
}

const double BarProvider::HLCAvg()
{
  boost::scoped_ptr<boost::unique_lock<shared_mutex> > lock; if (m_DataModified) lock.reset(new boost::unique_lock<shared_mutex>(m_BarProvider_Mutex));

  return ((*m_vdHigh)[m_iPsn] + (*m_vdLow)[m_iPsn] + (*m_vdClose)[m_iPsn]) / 3;
}

const long BarProvider::Volume()
{
  boost::scoped_ptr<boost::unique_lock<shared_mutex> > lock; if (m_DataModified) lock.reset(new boost::unique_lock<shared_mutex>(m_BarProvider_Mutex));

  return (*m_vlVolume)[m_iPsn];
}



void BarProvider::GetOHLCVInDateRange(const YYYYMMDD & ymdStart, const YYYYMMDD & ymdEnd, vector<YYYYMMDD> & vYMD, vector<double> & vO, vector<double> & vH, vector<double> & vL, vector<double> & vC, vector<long> & vV)
{
  boost::scoped_ptr<boost::unique_lock<shared_mutex> > lock; if (m_DataModified) lock.reset(new boost::unique_lock<shared_mutex>(m_BarProvider_Mutex));

  vector<HHMMSS> vHMS;
  GetOHLCVInDateTimeRange(ymdStart, HHMMSS(0), ymdEnd, HHMMSS(235959), vYMD, vHMS, vO, vH, vL, vC, vV);
}

void BarProvider::GetOHLCVInDateTimeRange(const YYYYMMDD & ymdStart, const HHMMSS & hmsStart, const YYYYMMDD & ymdEnd, const HHMMSS & hmsEnd, vector<YYYYMMDD> & vYMD, vector<HHMMSS> & vHMS, vector<double> & vO, vector<double> & vH, vector<double> & vL, vector<double> & vC, vector<long> & vV)
{
  boost::scoped_ptr<boost::unique_lock<shared_mutex> > lock; if (m_DataModified) lock.reset(new boost::unique_lock<shared_mutex>(m_BarProvider_Mutex));

  vYMD.clear();
  vHMS.clear();
  vO.clear();
  vH.clear();
  vL.clear();
  vC.clear();
  vV.clear();

  //--------------------------------------------------
  // some checkings
  //--------------------------------------------------
  if (ymdStart > ymdEnd) return;

  //--------------------------------------------------
  // Linear search (just for implementation simplicity) to the start and end date
  //--------------------------------------------------
  ResetPsn();
  while (HasNextBar())
  {
    MoveNext();
    if (GetHHMMSS() == HHMMSS::BAD_TIME)
    {
      if (GetYYYYMMDD() > ymdEnd) break;
    }
    else
    {
      if (GetYYYYMMDD() > ymdEnd) break;
      if (GetYYYYMMDD() == ymdEnd && GetHHMMSS() > hmsEnd) break;
    }

    if (GetHHMMSS() == HHMMSS::BAD_TIME)
    {
      if (GetYYYYMMDD() < ymdStart) continue;
    }
    else
    {
      if (
          (GetYYYYMMDD() < ymdStart) ||
          (GetYYYYMMDD() == ymdStart && GetHHMMSS() < hmsStart)
         )
        continue;
    }

    vYMD.push_back(GetYYYYMMDD());
    vHMS.push_back(GetHHMMSS());
    vO.push_back(Open());
    vH.push_back(High());
    vL.push_back(Low());
    vC.push_back(Close());
    vV.push_back(Volume());
  }

  return;
}


void BarProvider::AddBar(const YYYYMMDD & ymd, const double o, const double h, const double l, const double c, const long v)
{
  AddBar(ymd, HHMMSS::BAD_TIME, o, h, l, c, v);
}

void BarProvider::AddBar(const YYYYMMDD & ymd, const HHMMSS & hms, const double o, const double h, const double l, const double c, const long v)
{
  if (m_cFwdBkwd != 'F')
  {
    cerr << __FILE__ << "::" << __FUNCTION__ << " (" << __LINE__ << ") AddBar() not supported in backward mode." << endl << flush;
    SException se;
    se.PrintMsg(__FILE__, __FUNCTION__, __LINE__);
    throw se;
  }

  boost::unique_lock<boost::shared_mutex> lock(m_BarProvider_Mutex);
  m_DataModified = true;

  m_viYMD->push_back(ymd);
  m_viTime->push_back(hms);
  m_vdOpen->push_back(o);
  m_vdHigh->push_back(h);
  m_vdLow->push_back(l);
  m_vdClose->push_back(c);
  m_vlVolume->push_back(v);

  m_iNumOfBars++;
}

