#ifndef SYSTEMSTATE_H_
#define SYSTEMSTATE_H_

#include "PCH.h"
#include "Constants.h"
#include <boost/thread.hpp>

using namespace std;
using namespace boost;

class SystemState {
  public:
    enum State {STATE_ON=1, STATE_OFF=2};
    static boost::shared_ptr<SystemState> Instance();
    virtual ~SystemState();

    void SetSystemState(State);
    State GetSystemState();
    bool ChkIfThreadShouldStop();
  private:
    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    SystemState();
    SystemState(SystemState const&);
    SystemState& operator=(SystemState const&){};
    static boost::weak_ptr<SystemState> m_pInstance;

    //--------------------------------------------------
    // System state
    //--------------------------------------------------
    State m_SystemState;
};

#endif /* SYSTEMSTATE_H_ */
