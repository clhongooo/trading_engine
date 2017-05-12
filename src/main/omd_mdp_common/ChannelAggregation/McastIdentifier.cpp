/*
 * McastIdentifier.cpp
 *
 *  Created on: Apr 9, 2014
 *      Author: sunny
 */

#include "McastIdentifier.h"

McastIdentifier::McastIdentifier(
  const unsigned short ChannelID,
  const string & IP, const unsigned short Port,
  EMcastType mType, EChannel chnl) :
  m_Channel_ID (ChannelID),
  m_IP         (IP       ),
  m_Port       (Port     ),
  m_McastType  (mType    ),
  m_Channel    (chnl     )
{}

McastIdentifier::McastIdentifier(
  const unsigned short ChannelID,
  EMcastType mType, EChannel chnl) :
  m_Channel_ID (ChannelID  ),
  m_IP         ("127.0.0.1"), //dummy
  m_Port       (9394       ), //dummy
  m_McastType  (mType      ),
  m_Channel    (chnl       )
{}

McastIdentifier::~McastIdentifier() {}

const unsigned short         McastIdentifier::Channel_ID() const { return m_Channel_ID; }
const string &               McastIdentifier::IP()         const { return m_IP   ;      }
const unsigned short         McastIdentifier::Port()       const { return m_Port ;      }
McastIdentifier::EMcastType  McastIdentifier::McastType()  const { return m_McastType;  }
McastIdentifier::EChannel    McastIdentifier::Channel()    const { return m_Channel;    }

const string McastIdentifier::ToString() const
{
  stringstream ss("");
  if      (m_McastType == REALTIME        ) ss << "RT";
  else if (m_McastType == REFRESH         ) ss << "RF";
  else if (m_McastType == INSTRUMENTREPLAY) ss << "IR";
  if      (m_Channel == A                 ) ss << "_A_";
  else if (m_Channel == B                 ) ss << "_B_";
  ss << m_Channel_ID;
  return ss.str();
}

const string McastIdentifier::ToString(const EMcastType & mt)
{
  if      (mt == REALTIME        ) return "RT";
  else if (mt == REFRESH         ) return "RF";
  else if (mt == INSTRUMENTREPLAY) return "IR";
}


//--------------------------------------------------
// Old: (OMDC only)
// Before line arbitration
// Index   1..100:  1..100 channels for RT A
// Index 101..200:  1..100 channels for RT B
// Index 201..300:  1..100 channels for RF A
// Index 301..400:  1..100 channels for RF B
//--------------------------------------------------
// Old: (OMDC + OMDD)
// Before line arbitration
// Index    1.. 500:  1..500 channels for RT A
// Index  501..1000:  1..500 channels for RT B
// Index 1001..1500:  1..500 channels for RF A
// Index 1501..2000:  1..500 channels for RF B
//--------------------------------------------------
// New: (OMDC + OMDD + MDP)
// Before line arbitration
// Index    1..1000:  1..1000 channels for RT A
// Index 1001..2000:  1..1000 channels for RT B
// Index 2001..3000:  1..1000 channels for RF A
// Index 3001..4000:  1..1000 channels for RF B
// Index 4001..5000:  1..1000 channels for IR A
// Index 5001..6000:  1..1000 channels for IR B
//--------------------------------------------------
unsigned short McastIdentifier::Hash_MC() const
{
  unsigned short usRtn = 0;
  if      (m_McastType == REFRESH         ) usRtn += 2000;
  else if (m_McastType == INSTRUMENTREPLAY) usRtn += 4000;
  if (m_Channel == B) usRtn += 1000;
  usRtn += m_Channel_ID;
  return usRtn;
}

//--------------------------------------------------
// Old: (OMDC only)
// After line arbitration
// Index   1..100:  1..100 channels for RT
// Index 101..200:  1..100 channels for RF
//--------------------------------------------------
// Old: (OMDC + OMDD)
// After line arbitration
// Index   1.. 500:  1..500 channels for RT
// Index 501..1000:  1..500 channels for RF
//--------------------------------------------------
// New: (OMDC + OMDD + MDP)
// After line arbitration
// Index    1..1000:  1..1000 channels for RT
// Index 1001..2000:  1..1000 channels for RF
// Index 2001..3000:  1..1000 channels for IR
//--------------------------------------------------
unsigned short McastIdentifier::Hash_Chnl() const
{
  unsigned short usRtn = 0;
  if      (m_McastType == REFRESH         ) usRtn += 1000;
  else if (m_McastType == INSTRUMENTREPLAY) usRtn += 2000;
  usRtn += m_Channel_ID;
  return usRtn;
}

unsigned short McastIdentifier::GetMaxHash()
{
  return 10000;
}
