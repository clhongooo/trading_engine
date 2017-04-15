#ifndef UTIL_REGULARMINICTRTMANAGER_H_
#define UTIL_REGULARMINICTRTMANAGER_H_

#include "PCH.h"
#include "Constants.h"
#include "SFunctional.h"
#include "Exchange.h"

class RegularMiniCtrtManager {
  public:
    enum RMCM_Mode {RMCM_OPTZDELTA=0,RMCM_OPTZMARGINREQ};

    RegularMiniCtrtManager();
    virtual ~RegularMiniCtrtManager();

    void SetMode(const RMCM_Mode);
    long GetSuggestedCtrtSize(const string &) const;
    void UpdateRegularCtrtTgtSgnl(const string &,const double);
    double GetRegularCtrtTheoPos(const string &);
    void NetOffRndOffRegularMini();

  private:
    void SetRegularCtrtTheoPos(const string &,const double);

    //--------------------------------------------------
    // System objects
    //--------------------------------------------------
    boost::shared_ptr<Exchange> m_Exchg;

    //--------------------------------------------------
    RMCM_Mode          m_RMCM_Mode;
    map<string,long>   m_SuggestedPos;
    map<string,double> m_RegularCtrtTheoPos;
};

#endif /* UTIL_REGULARMINICTRTMANAGER_H_ */
