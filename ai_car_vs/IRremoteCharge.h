// IRremoteCharge.h

#ifndef _IRREMOTECHARGE_h
#define _IRREMOTECHARGE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class IRremoteChargeClass
{
 protected:


 public:
	void init();
};

extern IRremoteChargeClass IRremoteCharge;

#endif

