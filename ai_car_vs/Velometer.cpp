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

//����դ������ж���Ӧ
void   VelometerClass::onLeftCount(){

	if(this->hasLeftPedometer) {//�Ʋ���
		this->currentLeftGridCount++;
		if (this->currentLeftGridCount == this->leftAllGridCount&& this->onLeftPedometer != nullptr) {
		
			(*onLeftPedometer)(0);//�ﵽ�ǲ����ص�
			this->onLeftPedometer = nullptr;
			this->hasLeftPedometer = false;
			this->currentLeftGridCount = 0;
		}
	}
	this->leftCounter++;
	this->leftSum++;
}

//����դ������ж���Ӧ
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




//��ʱ�������Իص��������ٶ�
void   VelometerClass::SpeedDetection()
{

	//ת�ٵ�λ��ÿ���Ӷ���ת����r/min���������������Ϊ20���ն���
	float leftSpeed = (float)(this->leftCounter)/Constant::velometerPeriod;//С�����ֵ��ת��
	float rightSpeed =(float)(this->rightCounter)/ Constant::velometerPeriod;//С�����ֵ��ת��
											   //�ָ������������ٵĳ�ʼ״̬
	this->leftCounter = 0;   //���������ֵ���㣬�Ա������һ����������
	this->rightCounter = 0;

	leftMotor->currentSpeed = leftSpeed;
	rightMotor->currentSpeed = rightSpeed;

	if (speedListener!= nullptr) {
		(*speedListener)(&vs);
	}

	
}

//���ּƲ���
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



//�����ٶȼ����ߣ������Իص�
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

