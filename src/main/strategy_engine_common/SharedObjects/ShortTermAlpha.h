#ifndef SHORTTERMALPHA_H_
#define SHORTTERMALPHA_H_

#include "PCH.h"
#include "Constants.h"
#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>

using namespace std;
using namespace boost;

class ShortTermAlpha {
  public:
    virtual ~ShortTermAlpha();
    static boost::shared_ptr<ShortTermAlpha> Instance();

  private:
    //--------------------------------------------------
    // Singleton
    //--------------------------------------------------
    ShortTermAlpha();
    ShortTermAlpha(ShortTermAlpha const&);
    ShortTermAlpha& operator=(ShortTermAlpha const&){};
    static boost::weak_ptr<ShortTermAlpha> m_pInstance;
};

#endif /* SHORTTERMALPHA_H_ */
