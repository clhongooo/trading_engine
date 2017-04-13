/*
 * CentralMemMgr.h
 *
 *  Created on: Apr 12, 2014
 *      Author: sunny
 */

#ifndef CENTRALMEMMGR_H_
#define CENTRALMEMMGR_H_

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include <string.h>
#include "../OMD_Global.h"

using namespace std;
using namespace boost;

class CentralMemMgr {
  public:
    static shared_ptr<CentralMemMgr> Instance();
    virtual ~CentralMemMgr(){};
    vector<BYTE*> * AcquireVectorBlock(const int,const int);
    void ReturnVectorBlock(vector<BYTE*> *, const int, const int);

  private:
    CentralMemMgr(){};
    static weak_ptr<CentralMemMgr> m_pInstance;
};

#endif /* CENTRALMEMMGR_H_ */
