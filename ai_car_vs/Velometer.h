// Velometer.h
/*
	�������ࣺ����С������ǰ���ٶȣ�ֻ��ǰ���������ӣ�����һ�������࣬
	����MotorController����С��
*/

#ifndef _VELOMETER_h1
#define _VELOMETER_h1

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include"MyMotor.h"
 


class  VehicleSpeed {
public:
	int leftSpeed = 0;//��ǰ���ٶȣ�
	int rightSpeed = 0;//��ǰ���ٶ�;
};

class VelometerClass
{

public:
	static VelometerClass* curVelometer;
	static void onLeftCountStatic(void);//���ּ����ص����� 
	static void onRightCountStatic(void);//���ּ����ص����� 
	static void detectSpeed();

private:
	    const static int gridNum = 1;//һȦ��դ����
		 static  float  velometerPeriod;//�������ڣ�Ĭ��500ms

private:
	int leftCounter=0, rightCounter=0;
	unsigned long time=0, old_time=0; // ʱ����
	unsigned long time1=0; // ʱ����
	float leftSpeed=0, rightSpeed=0;//�������ٶ�
	int currentLeftGridCount=0;//��ǰ������������դ����
	int leftAllGridCount;//������תָ��ת����Ҫ������դ����
	void (*onLeftPedometer)(int x);//ת��ָ��ת���ص�
	bool hasLeftPedometer;//�Ƿ������ּ�����
	int currentRightGridCount;//��ǰ������������դ����
	int rightAllGridCount;//������תָ��ת����Ҫ������դ����
	void (*onRightPedometer)(int x);//����ת��ָ��ת���ص�
	bool hasRightPedometer;//�Ƿ������ּ�����
	void (*speedListener)(VehicleSpeed*);//�ٶȼ��������ص�ָ��
	MyMotorClass* rightMotor;
	MyMotorClass* leftMotor;

public:
	VehicleSpeed vs;
	int leftSum = 0;
	int rightSum = 0;

private:
	void onLeftCount();//���ּ����жϻص�����
	void onRightCount();//���ּ����жϻص�����

public:
	VelometerClass() {

	}

	~VelometerClass() {

	}
	void setCurMy()
	{//���õ�ǰ����Ϊ�ص��������õĶ���  
		curVelometer = this;
	}
	void init(MyMotorClass *rightMotor,MyMotorClass *leftMotor);
	void SpeedDetection();
	void addLeltPedometer(int step_count, void(*callback)(int));
	void addRightPedometer(int step_count, void(*callback)(int));
	void removeRightPedometer();
	void removeLeftPedometer();
	void removePedometer();
	void setSpeedListener(void(*callback)(VehicleSpeed*));
	VehicleSpeed getSpeed();
	 
	MyMotorClass* getLeftMotor();
	MyMotorClass* getRightMotor();

};

#endif

