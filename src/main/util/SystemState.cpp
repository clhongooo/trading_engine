#include <SystemState.h>

SystemState::SystemState() :
  m_SystemState(STATE_ON)
{

}

SystemState::~SystemState()
{
}

boost::weak_ptr<SystemState> SystemState::m_pInstance;

boost::shared_ptr<SystemState> SystemState::Instance() {
  boost::shared_ptr<SystemState> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new SystemState());
    m_pInstance = instance;
  }
  return instance;
}


void SystemState::SetSystemState(State s)
{
  m_SystemState = s;
}


SystemState::State SystemState::GetSystemState()
{
  return m_SystemState;
}


bool SystemState::ChkIfThreadShouldStop()
{
  if (m_SystemState == STATE_ON) return false;
  else if (m_SystemState == STATE_OFF) return true;
  else return false;
}

