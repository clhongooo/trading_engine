/*
 * CentralMemMgr.h
 *
 *  Created on: Apr 12, 2014
 *      Author: sunny
 */

#ifndef CENTRALMEMMGR_HPP_
#define CENTRALMEMMGR_HPP_

#include <boost/utility.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

using namespace std;
using namespace boost;

class CentralMemMgr {
  public:
    static shared_ptr<CentralMemMgr> Instance();
    virtual ~CentralMemMgr(){};
    BYTE** AcquireMemBlock(unsigned int &);
    void ReturnMemBlock(const unsigned int);

  private:
    ptr_vector<BYTE> m_MemBlocks;

    CentralMemMgr();
    static weak_ptr<CentralMemMgr> m_pInstance;
};

weak_ptr<CentralMemMgr > CentralMemMgr::m_pInstance;

shared_ptr<CentralMemMgr > CentralMemMgr::Instance() {
  shared_ptr<CentralMemMgr> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new CentralMemMgr());
    m_pInstance = instance;
  }
  return instance;
}

shared_ptr<BYTE*> CentralMemMgr::AcquireMemBlock()
{
  //TODO: Change to memory pool later
  BYTE** vbp = new BYTE*[iNumOfRows];

  for (unsigned int i = 0; i < iNumOfRows; ++i) vbp[i] = pB+i*iRowBytes;
  return vbp;
}

void CentralMemMgr::ReturnMemBlock(BYTE** vb, const unsigned int iNumOfRows)
{
  //TODO: Change to memory pool later
}

#endif /* CENTRALMEMMGR_HPP_ */


CentralMemMgr::CentralMemMgr()
{
pB = (BYTE*)malloc(iNumOfRows*iRowBytes);
}

CentralMemMgr::CentralMemMgr()
{
  free(vb[0]); //BYTE* pB = (BYTE*)malloc(iNumOfRows*iRowBytes);
  free(vb); //BYTE** vbp = new BYTE*[iNumOfRows];
}
