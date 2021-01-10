#include "IRremoteLocation.h"
#include "Port.h"

//0xa90 2704 左边红外值
//0xa80 2688 右边红外值
//0xa10 2576 请求打开红外定位
//0xa20 2592 回复确认红外激光已打开

IRremoteLocation* IRremoteLocation::instance = nullptr;



void IRremoteLocation::request_open_irlaser()
{
	request = true;

}

void IRremoteLocation::finddirection()
{
	auto a_lambda_func = [](int x) {
		
	};
	 mc->turnLeft(45, a_lambda_func);
	 

}

int IRremoteLocation::get_ir_instensity()
{
	int intensity = 0;
	size_t length = 5;
	for (size_t i = 0; i < length; i++)
	{
		intensity += analogRead(ir_intensity_pin);
	}
	return intensity / length;
}

bool IRremoteLocation::get_ir_signal()
{
	size_t length = 5;
	for (size_t i = 0; i < length; i++)
	{
		if (irrecv->decode(&results)) {
			if (results.value == 2704) {
				bink_red_led();
				irrecv->resume();
				return true;
			}
		}
		irrecv->resume();
	}
	return false;
}

void IRremoteLocation::findIrLocation()
{

	 
		if (lock) {
			return;
		}
		 
		confirm_forward_direction();
		lock = true;
	
}

bool IRremoteLocation::get_laser_state()
{
	 int state = digitalRead(LASER_RECEIVE_PIN);
	 return state == 0 ? true : false;

}

void IRremoteLocation::confirm_forward_direction()
{
	
	int  middle_crash = digitalRead(forward_middle_crash);
	current_signal = instance->get_ir_signal();
	current_ir_density = instance->get_ir_instensity();
	//Serial.println(current_ir_density);
	//Serial.println(current_signal);
	if (middle_crash == 0) {
		if (current_ir_density >= 70) {
			mc->stop();
			return;
		}
		mc->back();
		delay(300);
		mc->left();
		delay(300);
	}
	int  left_crash = digitalRead(forward_left_crash);
	int  right_crash = digitalRead(forward_right_crash);
	if (left_crash == 0 || right_crash == 0) {
		mc->back();
		delay(200);
		mc->left();
		delay(300);
		mc->forward();
	}
	
	     
		 mc->left();//左边100ms
		 delay(150);
		 left_signal = instance->get_ir_signal();
		 left_ir_density = instance->get_ir_instensity();

		 if (left_signal &&left_ir_density > current_ir_density) {//如果左边大于右边
	 			instance->mc->forward();
	 			delay(300);
				return;
	 	}
		mc->right();//右边200ms
	 	delay(300);
		right_signal = instance->get_ir_signal();
		right_ir_density = instance->get_ir_instensity();

		if (right_signal && right_ir_density > current_ir_density) {//如果右边大于中间
			 mc->forward();
			delay(300);
			return;
		}
		mc->left();
		delay(150);//回到原位直行
		mc->forward();
		delay(300);	
}

void IRremoteLocation::bink_red_led()
{
	 
		digitalWrite(led_red, HIGH);
		delay(10);
		digitalWrite(led_red, LOW);
	
}

void IRremoteLocation::loop()
{
	 
}

void IRremoteLocation::init_irremote_location(MotorControllerClass *mc)
{
	this->instance = this;
	this->mc = mc;
	irrecv = new IRrecv((RECV_PIN));
	irsend = new IRsend();
	irrecv->enableIRIn(); 
	pinMode(led_red, OUTPUT);
	pinMode(led_yellow, OUTPUT);
	pinMode(led_green, OUTPUT);
	pinMode(LASER_RECEIVE_PIN, INPUT);
	pinMode(ir_intensity_pin, INPUT);


}

void IRremoteLocation::findIR()
{
	if(request){
		irSend(0xa10);
		delay(100);
	}
	if (find_ir) {//找到紅外信號才進入此方法

		confirm_forward_direction();
		return;
	}
	if (irrecv->decode(&results)) {
		if (results.value == 2704) {
			find_ir = true;
			request = false;
			irrecv->resume(); // Receive the next value
		}
		irrecv->resume(); // Receive the next value
	}
}



void IRremoteLocation::irSend(unsigned long val)
{
	irsend->sendSony(val, 12);
	irrecv->enableIRIn();
}
