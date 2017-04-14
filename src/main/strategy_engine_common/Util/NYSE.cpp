#include <NYSE.h>

const double NYSE::_TRDG_DAY_PER_YR = 252;

boost::weak_ptr<NYSE> NYSE::m_pInstance;

boost::shared_ptr<NYSE> NYSE::Instance() {
  boost::shared_ptr<NYSE> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new NYSE());
    m_pInstance = instance;
  }
  return instance;
}

NYSE::NYSE()
{
}

NYSE::~NYSE()
{
}

bool NYSE::IsAProductInThisMarket(const string & symbol)
{
  //--------------------------------------------------
  // FIXME: incorrect
  //--------------------------------------------------
  if (STool::IsStringAllLetters(symbol)) return true;
  else return false;
}

