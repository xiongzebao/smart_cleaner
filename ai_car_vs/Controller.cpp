#include "Controller.h"
#include"MyUtils.h"

boolean is_going = false;
boolean isForwarding = false;
boolean isBacking = false;

int turn_resulution = 10;
unsigned long last_back_crash_time = 0;
int fast_back_crash_times = 0;
int last_crash_time = 0;

bool onleft = false;
bool onmiddle = false;
bool onright = false;

bool lock = false;

Controller* Controller::instance = nullptr;

auto cb = [](int x) {
	MyUtils.println("cb-----------");
	Controller::instance->is_interrupt = false;
	Controller::instance->is_finish_forward_back = true;
	Controller::instance->forward();
	lock = false;
};



void Controller::on_forward_right_crash()
{
	if (instance->lock_val) {
		return;
	}
	if (voidRepeatCrash() == false) {
		return;
	}
	if (lock) {//�������
		return;
	}
	
	if (onright) {
		onright = false;
		return;
	}
	onright = true;
	onleft = false;
	onmiddle = false;
	MyUtils.println("on_forward_right_crash");
	lock = true;

	instance->is_finish_forward_back = true;
	instance->is_interrupt = true;
	int level_forward_right = digitalRead(forward_right_gd);
	int level_forward_left = digitalRead(forward_left_gd);

	int level_left_forward = digitalRead(left_forward_gd);
	int level_left_back = digitalRead(left_back_gd);

	int level_right_forward = digitalRead(right_forward_gd);
	int level_right_back = digitalRead(right_back_gd);

	/*
	//ǰ����ײ�� 
	if (level_forward_right == 0) {
		if (digitalRead(level_forward_left) == 0) {
			if (level_left_forward == 1
				&& level_left_back == 1
				&& level_right_forward == 1
				&& level_right_back == 1
				) {
				instance->turn_left_right_random(90);
				return;
			}

			if (level_left_forward == 0 || level_left_back == 0) {
				auto a_lambda_func = [](int x) {
					instance->right(90, cb);
				};
				instance->back(100, a_lambda_func);
				return;
			}
			if (level_right_forward == 0 || level_right_back == 0) {

				auto a_lambda_func = [](int x) {
					instance->left(90, cb);
				};
				instance->back(100, a_lambda_func);
				return;
			}

			return;
		}
		instance->right();
		return;
	}
	*/
	auto a_lambda_func = [](int x) {
		instance->left(5, cb);
	};
	instance->mc->back(10, a_lambda_func);

	
}

void Controller::on_forward_middle_crash()
{
	if ( instance ->lock_val) {
		return;
	}
	if (voidRepeatCrash() == false) {
		return;
	}
	if (lock) {//�������
		return;
	}
	if (onmiddle) {
		onmiddle = false;
		return;
	}
	onmiddle = true;
	onleft = false;
	onright = false;


	MyUtils.println("on_forward_middle_crash");
	lock = true;
	instance->is_finish_forward_back = true;
	instance->is_interrupt = true;

	auto a_lambda_func = [](int x) {
		instance->left(5, cb);
	};
	instance->mc->back(10, a_lambda_func);

}


void Controller::on_forward_left_crash()
{
	if (instance->lock_val) {
		return;
	}
	if (voidRepeatCrash() == false) {
		return;
	}
	if (lock) {//�������
		return;
	}
	if (onleft) {
		onleft = false;
		return;
	}
	onleft = true;
	onmiddle = false;
	onright = false;

	MyUtils.println("on_forward_left_crash ");
	lock = true;
	instance->is_finish_forward_back = true;
	instance->is_interrupt = true;

	/* 
	int level_forward_right = digitalRead(forward_right_gd);
	int level_forward_left = digitalRead(forward_left_gd);

	int level_left_forward = digitalRead(left_forward_gd);
	int level_left_back = digitalRead(left_back_gd);

	int level_right_forward = digitalRead(right_forward_gd);
	int level_right_back = digitalRead(right_back_gd);


	if (level_forward_left == 0) {//������ײǽ
		if (level_forward_right == 0) {
			if (level_left_forward == 1
				&& level_left_back == 1
				&& level_right_forward == 1
				&& level_right_back == 1
				) {
				instance->turn_left_right_random(90);
				return;
			}

			if (level_left_forward == 0 || level_left_back == 0) {
				auto a_lambda_func = [](int x) {
					instance->right(90, cb);
				};
				instance->back(100, a_lambda_func);
				return;
			}
			if (level_right_forward == 0 || level_right_back == 0) {

				auto a_lambda_func = [](int x) {
					instance->left(90, cb);
				};
				instance->back(100, a_lambda_func);
				return;
			}


		}
		else {
			instance->right();

		}
	}


	*/
		auto a_lambda_func = [](int x) {
			MyUtils.println("on_forward_left_crash finshi");
			instance->left(5, cb);
			
		};
		instance->mc->back(10, a_lambda_func);
		return;
		
	}


void Controller::on_back_right_crash()
{
	if (instance->lock_val) {
		return;
	}
 
	instance->is_finish_forward_back = true;
	instance->is_interrupt = true;
	unsigned cur_time = millis();
	unsigned interval = cur_time - last_back_crash_time;
	if (interval <= 1000) {
		fast_back_crash_times++;
	}
	else {
		fast_back_crash_times = 0;
	}
	last_back_crash_time = cur_time;

	auto a_lambda_func = [](int x) {
		if (fast_back_crash_times >= 2) {
			instance->left(turn_resulution * 5, cb);
			return;
		}
		instance->left(turn_resulution, cb);
	};
	instance->forward(200, a_lambda_func);

}

void Controller::on_back_left_crash()
{
	if (instance->lock_val) {
		return;
	}
	instance->is_finish_forward_back = true;
	instance->is_interrupt = true;
	unsigned cur_time = millis();
	unsigned interval = cur_time - last_back_crash_time;
	if (interval <= 1000) {
		fast_back_crash_times++;
	}
	else {
		fast_back_crash_times = 0;
	}
	last_back_crash_time = cur_time;
	auto a_lambda_func = [](int x) {

		if (fast_back_crash_times >= 2) {
			instance->left(turn_resulution * 5, cb);
			return;
		}
		instance->left(turn_resulution, cb);
	};
	instance->forward(200, a_lambda_func);
}

bool Controller::voidRepeatCrash()
{
	unsigned cur_time = millis();
	unsigned interval = cur_time - last_crash_time;
	if (interval <= 50) {
	
		return false;
	}
	last_crash_time = cur_time;
	return true;
}

void Controller::initPort()
{
	pinMode(down_forward_gd, INPUT);
	pinMode(down_back_gd, INPUT);
	pinMode(down_right_gd, INPUT);
	pinMode(down_left_gd, INPUT);
	pinMode(back_left_gd, INPUT);
	pinMode(back_right_gd, INPUT);
	pinMode(left_back_gd, INPUT);
	pinMode(left_forward_gd, INPUT);
	pinMode(right_back_gd, INPUT);
	pinMode(right_forward_gd, INPUT);

	pinMode(forward_middle_crash, INPUT_PULLUP);
	pinMode(back_right_crash, INPUT_PULLUP);
	pinMode(forward_left_crash, INPUT_PULLUP);
	pinMode(forward_right_crash, INPUT_PULLUP);


	attachInterrupt(digitalPinToInterrupt(forward_middle_crash), Controller::on_forward_middle_crash, CHANGE);
	attachInterrupt(digitalPinToInterrupt(back_right_crash), Controller::on_back_right_crash, CHANGE);
	attachInterrupt(digitalPinToInterrupt(forward_left_crash), Controller::on_forward_left_crash, CHANGE);
	attachInterrupt(digitalPinToInterrupt(forward_right_crash), Controller::on_forward_right_crash, CHANGE);

}

Controller::Controller(MotorControllerClass* mc)
{
	this->mc = mc;
}


void Controller::forward_back()
{
	 
	//MyUtils.println("is_interrupt:");
	//Serial.print(is_interrupt);

	//MyUtils.println("is_finish_forward_back:");
	//Serial.print(is_finish_forward_back);
	//MyUtils.println("f");
	//if (instance->is_interrupt == true) {
	//	MyUtils.println("a");
	//}
	//if (instance->is_finish_forward_back == false) {
	//	MyUtils.println("b");
	//}
	if (instance->is_interrupt==true || instance->is_finish_forward_back==false) {
	//	MyUtils.println("return:");
		return;
	}

	MyUtils.println("kkkkkkkkk");
	instance->is_finish_forward_back = false;
	auto a_lambda_func = [](int x) {
		MyUtils.println("123456677888");
	auto f = [](int x) {
		MyUtils.println("9999999999");
			instance->is_finish_forward_back = true;
		};
		instance->back(200, f);
		//instance->stop();
	};
	instance->mc->forward(400, a_lambda_func);
	//String action = "forward_back:" + 0;
	//add_path(action);
	//MyUtils.println("f");
}

void Controller::loop() {




	int right = digitalRead(forward_right_crash);

	int  left = digitalRead(forward_left_crash);
	int middle = digitalRead(forward_middle_crash);
	//int level_back_left = digitalRead(back_left_gd);
	//if (level_forward_left == 0){
	//	on_forward_left_crash();
	//}
	//if (level_forward_right == 0){
	//	on_forward_right_crash();
	//}
	//if (level_back_right == 0){
	//	on_back_right_crash();
	//}
	//if (level_back_l eft == 0){
	//	on_back_left_crash();
	//}

	//forward_back();
	if (right == 0) {
		on_forward_right_crash();
	}
	if (middle == 0) {
		on_forward_middle_crash();
	}

	if (left == 0) {
		on_forward_left_crash();
	}


}


//void Controller::loop() {////������0����1
//
//
//
//	/*
//	//ǰ������,����5cm����ת30��
//	if (digitalRead(down_forward_gd) == 1) {
//		auto a_lambda_func = [](int x) {
//			instance->right(30);
//		};
//		mc->back(5, a_lambda_func);
//		return;
//
//	}
//	//������,ǰ��5cm����ת30��
//	if (digitalRead(down_forward_gd) == 1) {
//		auto a_lambda_func = [](int x) {
//			instance->left(30);
//		};
//		mc->forward(5, a_lambda_func);
//		return;
//	}
//	//�������,����5cm����ת30��
//	if (digitalRead(down_left_gd) == 1) {
//		auto a_lambda_func = [](int x) {
//			instance->right(30);
//		};
//		mc->back(5, a_lambda_func);
//		return;
//	}
//
//
//	//�ұ�����,����5cm����ת30��
//	if (digitalRead(down_right_gd) == 0) {
//		auto a_lambda_func = [](int x) {
//			instance->left(30);
//		};
//		MyUtils.println("d");
//		mc->back(5, a_lambda_func);
//		return;
//	}
//	*/
//
//	int p_forward_left_crash = digitalRead(forward_left_crash);
//	int p_forward_right_crash = digitalRead(forward_right_crash);
//
//	////��ǰ�����ⴥ��������5cm����ת30��ǰ��
//	MyUtils.println(p_forward_left_crash);
//	if (p_forward_left_crash == 0) {
//		//auto a_lambda_func = [](int x) {
//		//	MyUtils.println("callback");
//		//	instance->right(30);
//		//};
//		//mc->back(5, a_lambda_func);
//		mc->left();
//		return;
//	}
//
//
//	/*
//	////�Һ󷽺��ⴥ����ǰ��5cm����ת30��ǰ��
//	if (digitalRead(right_back_gd) == 0) {
//		auto a_lambda_func = [](int x) {
//			instance->left(30);
//		};
//		mc->forward(5, a_lambda_func);
//	}
//
//	////��󷽺��ⴥ����ǰ��5cm����ת30��ǰ��
//	if (digitalRead(left_back_gd) == 0) {
//		auto a_lambda_func = [](int x) {
//			instance->right(30);
//		};
//		mc->forward(5, a_lambda_func);
//	}
//	*/
//}






void Controller::forward()
{
	mc->forward();
}



void Controller::back()
{
	if (isRightHover()) {
		mc->turnLeft(90);

		return;
	}
	Serial.println("back");
	if (isBackHover()) {
		mc->stop();
		if (!isForwardHover()) {
			mc->forward();
		}
	}
	else {
		//�ж�ǰ���Ƿ����ϰ��� 
		if (getForwardState() != 3) {//ǰ�����ϰ���
			mc->back();
		}
		return;
	}
}

void Controller::left()
{
	if (mc->state == MotorControllerClass::RIGHT) {
		return;
	}
	mc->left();
	String action = "left:" + 0;
	add_path(action);
}

void Controller::right()
{
	if (mc->state == MotorControllerClass::RIGHT) {
		return;
	}
	mc->right();
	String action = "right:" + 0;
	add_path(action);
}

bool Controller::is_circle()
{
	if (path_array_index < 6) {
		return false;
	}
	for (int i = 2; i < 20; i++)
	{
		if (path_array_index + 1 < i * 3) {
			return false;
		}
		for (int j = 0; j < i; j++)
		{
			if (path_array[j].compareTo(path_array[j + i]) != 0) {
				return false;
			}
		}
		for (int k = i; k < (i + 1) * 2; k++)
		{
			if (path_array[k].compareTo(path_array[k + i]) != 0) {
				return false;
			}
		}
		return true;
	}
	return false;
}

void Controller::back_right()
{
	auto a_lambda_func = [](int x) {
		instance->right(90, cb);
	};
	instance->back(100, a_lambda_func);
}

void Controller::back_left()
{
	if (is_circle()) {
		//turnRight(random() % 360);
		return;
	}
	auto a_lambda_func = [](int x) {
		instance->left(90, cb);
	};
	instance->back(100, a_lambda_func);
}

void Controller::turn_left_right_random(const int degree)
{

	auto a_lambda_func = [](int x) {
		long randNumber = random(1000);

		if (randNumber % 2 == 0) {
			instance->left(90, cb);
		}
		else {
			instance->right(90, cb);
		}
	};

	instance->back(100, a_lambda_func);
}



void Controller::left(int degree)
{
	if (mc->state == MotorControllerClass::LEFT) {
		return;
	}
	mc->turnLeft(degree);
	String action = "left:" + degree;
	add_path(action);
}

void Controller::right(int degree)
{
	if (mc->state == MotorControllerClass::RIGHT) {
		return;
	}
	mc->turnRight(degree);
	String action = "right:" + degree;
	add_path(action);
}

void Controller::left(int degree, void(*f)(int))
{
	if (mc->state == MotorControllerClass::LEFT) {
		return;
	}
	mc->turnLeft(degree, f);
	String action = "left:" + degree;
	add_path(action);
}

void Controller::right(int degree, void(*f)(int))
{
	if (mc->state == MotorControllerClass::RIGHT) {
		return;
	}
	mc->turnRight(degree, f);
	String action = "right:" + degree;
	add_path(action);
}

void Controller::back(int degree, void(*f)(int))
{
	if (mc->state == MotorControllerClass::BACK) {
		return;
	}
	mc->back(degree, f);
	String action = "back:" + degree;
	add_path(action);
}

void Controller::forward(int degree, void(*f)(int))
{
	if (mc->state == MotorControllerClass::FORWARD) {
		return;
	}
	mc->forward(degree, f);
	String action = "forward:" + degree;
	add_path(action);

}

void Controller::stop()
{
	mc->stop();

}


//0;���Ҷ��£�1����£�2���Ҷ£�3����ͨ
int Controller::getBackState()
{

	int back_left_level = digitalRead(back_left_gd);
	int back_right_level = digitalRead(back_right_gd);
	if (back_left_level == 0 && back_right_level == 0) {//�������ϰ���
		return 0;
	}
	if (back_left_level == 0 && back_right_level == 1) {
		return 1;
	}
	if (back_left_level == 1 && back_right_level == 0) {
		return 2;
	}
	return 3;
}

int Controller::getForwardState()
{


}

bool Controller::isBackHover()
{
	if (digitalRead(down_back_gd) == 1)
		return true;
	return false;
}

bool Controller::isForwardHover()
{
	if (digitalRead(down_forward_gd) == 1)
		return true;
	return false;
}

bool Controller::isLeftHover()
{
	if (digitalRead(down_left_gd) == 1)
		return true;
	return false;
}

bool Controller::isRightHover()
{
	if (digitalRead(down_right_gd) == 1)
		return true;
	return false;
}

void Controller::Lock()
{
	this->lock_val = true;
	detachInterrupt(digitalPinToInterrupt(forward_left_crash));
	detachInterrupt(digitalPinToInterrupt(forward_right_crash));
	detachInterrupt(digitalPinToInterrupt(forward_middle_crash));
	detachInterrupt(digitalPinToInterrupt(back_right_crash));
}

void Controller::unLock()
{
	this->lock_val = false;
	attachInterrupt(digitalPinToInterrupt(forward_middle_crash), Controller::on_forward_middle_crash, CHANGE);
	attachInterrupt(digitalPinToInterrupt(back_right_crash), Controller::on_back_right_crash, CHANGE);
	attachInterrupt(digitalPinToInterrupt(forward_left_crash), Controller::on_forward_left_crash, CHANGE);
	attachInterrupt(digitalPinToInterrupt(forward_right_crash), Controller::on_forward_right_crash, CHANGE);

}
