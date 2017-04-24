//**************************************************
//  Author:      Sunny Yan
//  Created On:  Sun Apr 23 22:23:29 HKT 2017
//**************************************************

#ifndef MDPMESSAGEHEADER_H_
#define MDPMESSAGEHEADER_H_

#include <cstring>

using namespace std;
//----------------------------------------------------------------------------------------------------

#pragma pack(1)
typedef struct {
  uint32_t MsgSeqNum;
  uint64_t SendingTime;
} MDP_Binary_Packet_Header;
#pragma pack(8)

#endif
