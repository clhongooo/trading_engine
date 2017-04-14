#include <ProbDistributionGenerator.h>

ProbDistributionGenerator::ProbDistributionGenerator() :
  m_CalcIntervalInSeconds(60)
{
  m_MarketData      = MarketData::Instance();
  m_SystemState     = SystemState::Instance();
  m_SysCfg          = SystemConfig::Instance();
  m_VolSurfaces     = VolSurfaces::Instance();
  m_Logger          = Logger::Instance();
  m_Exchg           = Exchange::Instance();
  m_HKFE            = HKFE::Instance();
  m_HKSE            = HKSE::Instance();
  m_HKMA            = HKMA::Instance();
  m_TechInd         = TechIndicators::Instance();
  m_ProbDistrn      = ProbDistributions::Instance();
  m_CorrelMatrices  = CorrelMatrices::Instance();
  m_ThrdHlthMon     = ThreadHealthMonitor::Instance();
}

ProbDistributionGenerator::~ProbDistributionGenerator()
{
  for (map<long,ProbMassFunc*>::iterator it = m_fsmc_pmf.begin(); it != m_fsmc_pmf.end(); ++it)
  {
    if (it->second)
    {
      delete it->second;
      it->second = NULL;
    }
  }
  m_fsmc_pmf.clear();
}

void ProbDistributionGenerator::SetCalcIntervalInSec(long calcint)
{
  m_CalcIntervalInSeconds = calcint;
}

long ProbDistributionGenerator::HashKey(const int iYYYYMM, const int iFSMCState)
{
  if (iYYYYMM > 999999 || iYYYYMM < 190000) return -1;
  return iYYYYMM * 10000 + iFSMCState;
}

void ProbDistributionGenerator::LoadTrainedFSMCData(const string & sPath)
{

  //--------------------------------------------------
  // Get all files in directory. Expect filename in the YYYYMM format
  //--------------------------------------------------
  namespace fs = boost::filesystem;
  fs::path someDir(sPath);
  fs::directory_iterator end_iter;

  if ( fs::exists(someDir) && fs::is_directory(someDir))
  {
    for( fs::directory_iterator dir_iter(someDir) ; dir_iter != end_iter ; ++dir_iter)
    {
      if (fs::is_regular_file(dir_iter->status()) )
      {

        string sFileName = dir_iter->path().filename().string();
        int iYYYYMM = boost::lexical_cast<int>(sFileName);

        m_Logger->Write(Logger::DEBUG,"ProbDistributionGenerator: Reading FSMC trained data file: %s", sFileName.c_str());

        deque<string> dqRawFile;
        STool::ReadFile(dir_iter->path().string(),dqRawFile);
        if (dqRawFile.empty()) return;
        for (unsigned int i = 0; i < dqRawFile.size(); ++i)
        {
          vector<string> vs;
          boost::split(vs, dqRawFile[i], boost::is_any_of(","));

          //--------------------------------------------------
          // Not checking input files, let it crash if there's problem
          //--------------------------------------------------
          int    iFSMCState  = boost::lexical_cast<int>   (vs[0]);
          double dRtnOvVR    = boost::lexical_cast<double>(vs[1]);
          double dProbMass   = boost::lexical_cast<double>(vs[2]);

          long lKey = HashKey(iYYYYMM,iFSMCState);
          map<long,ProbMassFunc*>::iterator it =  m_fsmc_pmf.find(lKey);
          if (it == m_fsmc_pmf.end())
          {
            m_fsmc_pmf[lKey] = new ProbMassFunc();
            it =  m_fsmc_pmf.find(lKey);
          }
          it->second->SetPr(FSMC_DEFAULT_YMDHMS,dRtnOvVR,dProbMass);
        }


      }
    }
  }
  //--------------------------------------------------

  return;
}

void ProbDistributionGenerator::Run()
{
  for (;;)
  {
    m_MarketData->WaitForData();

    if (m_SystemState->ChkIfThreadShouldStop()) break;

    m_ThrdHlthMon->ReportHealthy(ID_PROBDISTGENR);

    //--------------------------------------------------
    // Figure out system time and whether should generate again
    //--------------------------------------------------
    YYYYMMDDHHMMSS ymdhms_MDITime = m_MarketData->GetSystemTimeHKT();

    if (m_ymdhms_LastCalculated.GetYYYYMMDD() == YYYYMMDD::BAD_DATE)
    {
      m_ymdhms_LastCalculated.Set(ymdhms_MDITime);
    }

    long lTimeDiff = ymdhms_MDITime - m_ymdhms_LastCalculated;
    if (lTimeDiff < m_CalcIntervalInSeconds) continue;

    //--------------------------------------------------
    // Not generate if not trading hour
    //--------------------------------------------------
    if (!m_Exchg->IsTradingHour(HSI_CONT_FUT_1,ymdhms_MDITime))
    {
      m_Logger->Write(Logger::DEBUG,"ProbDistributionGenerator: System time: %s. Non-trading hour.", ymdhms_MDITime.ToStr().c_str());
      continue;
    }

    //--------------------------------------------------
    // What is the next trading day
    //--------------------------------------------------
    YMD ymdNextDay(ymdhms_MDITime.GetYYYYMMDD());
    m_HKFE->NextTrdgDate(ymdNextDay, 1);
    YYYYMMDDHHMMSS ymdhmsNextDayTime(ymdNextDay,HMS(16,0,0));

    //--------------------------------------------------
    // Get HSI constitutents
    //--------------------------------------------------
    ProbMassFunc pmf_HSIrtn;
    map<string,double> * mHSICon = m_HKSE->GetHSIConstituents(ymdhms_MDITime.GetYYYYMMDD());
    for (map<string,double>::iterator ithsicon = mHSICon->begin(); ithsicon != mHSICon->end(); ++ithsicon)
    {
      //--------------------------------------------------
      // Figure out PMF for each HSI constitutent
      //--------------------------------------------------
      int iFSMCState = m_TechInd->GetFSMCState(ithsicon->first);
      if (iFSMCState >= 0)
      {
        long lKey = HashKey(ymdhms_MDITime.GetYYYYMMDD().ToIntYM(),iFSMCState);
        map<long,ProbMassFunc*>::iterator itfsmcstatepmf = m_fsmc_pmf.find(lKey);

        double dGKYZ = m_TechInd->GetGKYZValue(ithsicon->first);
        if (itfsmcstatepmf != m_fsmc_pmf.end() && dGKYZ > 0)
          pmf_HSIrtn.ImportButNotNmlz(ymdhmsNextDayTime.ToInt(), *(itfsmcstatepmf->second), FSMC_DEFAULT_YMDHMS, ithsicon->second, dGKYZ);
      }
    }

    //--------------------------------------------------
    // Variance sum(w1 w2 s1 s2 rho)
    //--------------------------------------------------
    double dIdxVar = 0;
    for (map<string,double>::iterator ithsicon1 = mHSICon->begin(); ithsicon1 != mHSICon->end(); ++ithsicon1)
    {
      for (map<string,double>::iterator ithsicon2 = mHSICon->begin(); ithsicon2 != mHSICon->end(); ++ithsicon2)
      {
        double dGKYZ1 = m_TechInd->GetGKYZValue(ithsicon1->first);
        double dGKYZ2 = m_TechInd->GetGKYZValue(ithsicon2->first);

        int iFSMCState1 = m_TechInd->GetFSMCState(ithsicon1->first);
        int iFSMCState2 = m_TechInd->GetFSMCState(ithsicon2->first);

        long lKey1 = HashKey(ymdhms_MDITime.GetYYYYMMDD().ToIntYM(),iFSMCState1);
        long lKey2 = HashKey(ymdhms_MDITime.GetYYYYMMDD().ToIntYM(),iFSMCState2);

        map<long,ProbMassFunc*>::iterator itfsmcstatepmf1 = m_fsmc_pmf.find(lKey1);
        map<long,ProbMassFunc*>::iterator itfsmcstatepmf2 = m_fsmc_pmf.find(lKey2);

        //--------------------------------------------------
        // Since the variance given by FSM PMF is Var(RtnOvVR). need to multiply by the standard deviation to get back the variance of return.
        //--------------------------------------------------
        dIdxVar +=
          ithsicon1->second *
          ithsicon2->second *
          dGKYZ1 *
          dGKYZ2 *
          sqrt(itfsmcstatepmf1->second->GetVariance(ymdhms_MDITime.GetYYYYMMDD().ToInt())) *
          sqrt(itfsmcstatepmf2->second->GetVariance(ymdhms_MDITime.GetYYYYMMDD().ToInt())) *
          m_CorrelMatrices->GetCorrel(ymdhms_MDITime.GetYYYYMMDD().ToInt(), ithsicon1->first, ithsicon2->first);
      }
    }
    //--------------------------------------------------

    //--------------------------------------------------
    // need to scale the variance of this HSI return to dIdxVar
    //--------------------------------------------------
    pmf_HSIrtn.AdjustToStdevNmlz(ymdhmsNextDayTime.ToInt(),sqrt(dIdxVar));


    m_Logger->Write(Logger::DEBUG,"ProbDistributionGenerator: %s. PMF: -50\% to -10% : %f", ymdhmsNextDayTime.ToStr().c_str(), pmf_HSIrtn.GetPr(ymdhmsNextDayTime.ToInt(),-0.50,-0.10));
    m_Logger->Write(Logger::DEBUG,"ProbDistributionGenerator: %s. PMF: -10\% to -5 % : %f", ymdhmsNextDayTime.ToStr().c_str(), pmf_HSIrtn.GetPr(ymdhmsNextDayTime.ToInt(),-0.10,-0.05));
    m_Logger->Write(Logger::DEBUG,"ProbDistributionGenerator: %s. PMF: -5 \% to -3 % : %f", ymdhmsNextDayTime.ToStr().c_str(), pmf_HSIrtn.GetPr(ymdhmsNextDayTime.ToInt(),-0.05,-0.03));
    m_Logger->Write(Logger::DEBUG,"ProbDistributionGenerator: %s. PMF: -3 \% to -1 % : %f", ymdhmsNextDayTime.ToStr().c_str(), pmf_HSIrtn.GetPr(ymdhmsNextDayTime.ToInt(),-0.03,-0.01));

    m_Logger->Write(Logger::DEBUG,"ProbDistributionGenerator: %s. PMF: -1 \% to  1 % : %f", ymdhmsNextDayTime.ToStr().c_str(), pmf_HSIrtn.GetPr(ymdhmsNextDayTime.ToInt(),-0.01, 0.01));

    m_Logger->Write(Logger::DEBUG,"ProbDistributionGenerator: %s. PMF:  1 \% to  3 % : %f", ymdhmsNextDayTime.ToStr().c_str(), pmf_HSIrtn.GetPr(ymdhmsNextDayTime.ToInt(), 0.01, 0.03));
    m_Logger->Write(Logger::DEBUG,"ProbDistributionGenerator: %s. PMF:  3 \% to  5 % : %f", ymdhmsNextDayTime.ToStr().c_str(), pmf_HSIrtn.GetPr(ymdhmsNextDayTime.ToInt(), 0.03, 0.05));
    m_Logger->Write(Logger::DEBUG,"ProbDistributionGenerator: %s. PMF:  5 \% to  10% : %f", ymdhmsNextDayTime.ToStr().c_str(), pmf_HSIrtn.GetPr(ymdhmsNextDayTime.ToInt(), 0.05, 0.10));
    m_Logger->Write(Logger::DEBUG,"ProbDistributionGenerator: %s. PMF:  10\% to  50% : %f", ymdhmsNextDayTime.ToStr().c_str(), pmf_HSIrtn.GetPr(ymdhmsNextDayTime.ToInt(), 0.10, 0.50));

    m_ProbDistrn->SetPMF(string(UNDERLYING_HSI), ymdhmsNextDayTime.ToInt(), pmf_HSIrtn);

    m_ymdhms_LastCalculated.Set(ymdhms_MDITime);

  }

  m_Logger->Write(Logger::NOTICE,"ProbDistributionGenerator has ended.");
  sleep(2);

  return;
}


