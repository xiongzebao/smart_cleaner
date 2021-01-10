#pragma once

#include "IRremote.h"
#include "MotorController.h"


class IRremoteLocation
{
private:
	MotorControllerClass *mc;
	IRsend *irsend;
	IRrecv *irrecv;
	decode_results results;
	bool request=false;
	bool find_ir = false;
	static IRremoteLocation* instance;
	bool lock;

	int current_ir_density = 0;
	int left_ir_density = 0;
	int right_ir_density = 0;
	bool left_signal = false;
	bool right_signal = false;
	bool current_signal = false;
	




public:
	 void init_irremote_location(MotorControllerClass* mc);
	 void findIR();
	 void irSend(unsigned long val);
	 void request_open_irlaser();
	 void finddirection();
	 int get_ir_instensity();
	 bool get_ir_signal();
	 void findIrLocation();
	 bool get_laser_state();
	 void confirm_forward_direction();
	 void bink_red_led();
	 void loop();


};

