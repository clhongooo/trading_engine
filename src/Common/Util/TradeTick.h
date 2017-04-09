#ifndef TRADETICK_H_
#define TRADETICK_H_

#include "PCH.h"
#include "Constants.h"
#include "SDateTime.h"

typedef struct {
  HMS       hms;
  double    Price;
  long      Volume;
  int       Initiator;
} TradeTick;

#endif
