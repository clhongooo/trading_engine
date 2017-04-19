#include "CentralMemMgr.h"

weak_ptr<CentralMemMgr> CentralMemMgr::m_pInstance;

shared_ptr<CentralMemMgr> CentralMemMgr::Instance() {
  shared_ptr<CentralMemMgr> instance = m_pInstance.lock();
  if (!instance) {
    instance.reset(new CentralMemMgr());
    m_pInstance = instance;
  }
  return instance;
}

vector<BYTE*> * CentralMemMgr::AcquireVectorBlock(const int iBlockSize, const int iRowSize)
{
  //TODO: Change to memory pool later
  vector<BYTE*> * v = new vector<BYTE *>(iBlockSize,NULL);
  for (unsigned int i = 0; i < iBlockSize; ++i)
  {
    (*v)[i] = new BYTE[iRowSize];
    memset((char*)((*v)[i]),'\0',1+8+2+2); //to prevent valgrind from complaining
  }

  return v;
}

void CentralMemMgr::ReturnVectorBlock(vector<BYTE*> * vb, const int iBlockSize, const int iRowSize)
{
  //TODO: Change to memory pool later
  for (unsigned int i = 0; i < iBlockSize; ++i)
    delete [] (*vb)[i];
  delete vb;
}

