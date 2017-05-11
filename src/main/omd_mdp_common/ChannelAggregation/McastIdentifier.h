/*
 * McastIdentifier.h
 *
 *  Created on: Apr 9, 2014
 *      Author: sunny
 */

#ifndef MCASTCONFIG_H_
#define MCASTCONFIG_H_

#include <string>
#include <sstream>
#include <boost/shared_ptr.hpp>

using namespace std;
using namespace boost;

class McastIdentifier {

  public:
    enum EMcastType {REALTIME, REFRESH, INSTRUMENTREPLAY};
    enum EChannel {A, B};

    McastIdentifier(
      const unsigned short,
      const string &, const unsigned short,
      EMcastType, EChannel);
    McastIdentifier(
      const unsigned short,
      EMcastType, EChannel);
    virtual ~McastIdentifier();

    const unsigned short    Channel_ID()     const;
    const string          & IP()             const;
    const unsigned short    Port()           const;
    EMcastType              McastType()      const;
    EChannel                Channel()        const;
    const string            ToString()       const;
    unsigned short          Hash_MC()        const;
    unsigned short          Hash_Chnl()      const;
    static unsigned short   GetMaxHash();

  private:
    unsigned short  m_Channel_ID;
    string          m_IP;
    unsigned short  m_Port;
    EMcastType      m_McastType;
    EChannel        m_Channel;
};
#endif /* MCASTCONFIG_H_ */
