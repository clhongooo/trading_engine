#include <ShortTermAlpha.h>

boost::weak_ptr<ShortTermAlpha> ShortTermAlpha::m_pInstance;

boost::shared_ptr<ShortTermAlpha> ShortTermAlpha::Instance() {
  boost::shared_ptr<ShortTermAlpha> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new ShortTermAlpha());
    m_pInstance = instance;
  }
  return instance;
}

ShortTermAlpha::ShortTermAlpha()
{
}

ShortTermAlpha::~ShortTermAlpha()
{
}
