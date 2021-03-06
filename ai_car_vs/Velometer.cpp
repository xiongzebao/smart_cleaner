#include "Velometer.h"
#include "port.h"
#include "MyUtils.h"
#include "FlexiTimer2.h"



VelometerClass* VelometerClass::curVelometer = nullptr;
float  VelometerClass::velometerPeriod = 0.4;

void VelometerClass::onLeftCountStatic(void)
{
	curVelometer->onLeftCount();
}

void VelometerClass::onRightCountStatic(void)
{
	 
	curVelometer->onRightCount();
}

void VelometerClass::detectSpeed()
{
	
	curVelometer->SpeedDetection();
}

//左轮栅格计数中断响应
void   VelometerClass::onLeftCount(){

	if(this->hasLeftPedometer) {//计步器
		this->currentLeftGridCount++;
		if (this->currentLeftGridCount == this->leftAllGridCount&& this->onLeftPedometer != nullptr) {
		
			(*onLeftPedometer)(0);//达到记步数回调
			this->onLeftPedometer = nullptr;
			this->hasLeftPedometer = false;
			this->currentLeftGridCount = 0;
		}
	}
	this->leftCounter++;
	this->leftSum++;
}

//右轮栅格计数中断响应
void   VelometerClass::onRightCount()
{
	if (this->hasRightPedometer) {
		this->currentRightGridCount++;
		if (this->currentRightGridCount == this->rightAllGridCount) {

			(*onRightPedometer)(0);
			Serial.println(currentRightGridCount);
			this->onRightPedometer = nullptr;
			this->hasRightPedometer = false;
			this->currentRightGridCount = 0;
		}
	}
	this->rightCounter++;
	this->rightSum++;
}

//https://blog.csdn.net/coolwriter/article/details/80598134
void   VelometerClass::init(MyMotorClass *rightMotor, MyMotorClass *leftMotor)
{
	setCurMy();
	this->leftMotor = leftMotor;
	this->rightMotor = rightMotor;
	attachInterrupt(digitalPinToInterrupt(left_interupt_pin), VelometerClass::onLeftCountStatic, FALLING);
	attachInterrupt(digitalPinToInterrupt(right_interupt_pin), VelometerClass::onRightCountStatic, FALLING);//wending

}




//定时器周期性回调，计算速度
void   VelometerClass::SpeedDetection()
{

	//转速单位是每分钟多少转，即r/min。这个编码器码盘为20个空洞。
	float leftSpeed = (float)(this->leftCounter)/Constant::velometerPeriod;//小车车轮电机转速
	float rightSpeed =(float)(this->rightCounter)/ Constant::velometerPeriod;//小车车轮电机转速
											   //恢复到编码器测速的初始状态
	this->leftCounter = 0;   //把脉冲计数值清零，以便计算下一秒的脉冲计数
	this->rightCounter = 0;

	leftMotor->currentSpeed = leftSpeed;
	rightMotor->currentSpeed = rightSpeed;

	if (speedListener!= nullptr) {
		(*speedListener)(&vs);
	}

	
}

//左轮计步器
void   VelometerClass::addLeltPedometer(int step_count, void(*callback)(int))
{
	this->leftAllGridCount = step_count * this->gridNum;
	this->onLeftPedometer = callback;
	this->hasLeftPedometer = true;
 
}

void   VelometerClass::addRightPedometer(int zhuanshu, void(*callback)(int))
{
	this->rightAllGridCount = zhuanshu * this->gridNum;
	this->onRightPedometer = callback;
	this->hasRightPedometer = true;
	
 
}

void VelometerClass::removeRightPedometer()
{
	this->rightAllGridCount = 0;
	this->onRightPedometer = nullptr;
	this->hasRightPedometer = false;
	this->currentRightGridCount = 0;
	 
}

void VelometerClass::removeLeftPedometer()
{
	this->leftAllGridCount = 0;
	this->onLeftPedometer = nullptr;
	this->hasLeftPedometer = false;
	this->currentLeftGridCount = 0;
 
}

void VelometerClass::removePedometer()
{
	removeLeftPedometer();
	removeRightPedometer();
}



//设置速度监听者，周期性回调
void   VelometerClass::setSpeedListener(void(*callback)(VehicleSpeed*))
{
	this->speedListener = callback;
}

VehicleSpeed VelometerClass::getSpeed()
{
	return this->vs;
}
 

MyMotorClass* VelometerClass::getLeftMotor()
{
	return this->leftMotor;
}

MyMotorClass* VelometerClass::getRightMotor()
{
	return this->rightMotor;
}

