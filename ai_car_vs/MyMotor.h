// MyMotor.h
// ����ࣺ����һ������ٶȺͷ���
#ifndef _MYMOTOR_h
#define _MYMOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class MyMotorClass
{//һ����ת��ת�������ڲ��ܼ�����ﵽ��ת������Ҫ��pwdֵ����Ҫ�����������в��ϵ���
public:
	static int MAX_SPEED;//�������ܴﵽ�����ת��
	static const int ALLOW_ARROE_RANGE = 10;//���������ٶ����Ϊ10
  	static const int STOP = 0;
	static const int FORWARD = 1; 
	static const int BACK = 2;
	static const int BRAKE = 3;
	int currentSpeed = 0;//��ǰ�ٶȣ�

private:
	int in1;//�˿�1
	int in2;//�˿�2
	int en;//ʹ�ܶ�
	int state = 0;//��¼��ǰ����н�״̬��STOP��ֹͣ��FORWARD��ǰ����BACK�����ˣ�BRAKE���ƶ�
	int currentDutyCycle=0;//��ǰռ�ձ�,���ͨ������ռ�ձ��������ٶ�

 protected:

 public:
	 MyMotorClass(int   in1, int   in2, int   en);
	void init( int   in1,  int   in2,   int   en);
	void setSpeed(int speed);
	void speedUp();
	void speedDown();
	void forward();
	void back();
	void stop();//ֹͣ
	void brake();//�ƶ�
	int getState();//��ȡ����״̬
	void openMaxSpeed();//��������ٶ�

public:
	int speed = 0;//Ŀ���ٶȣ��������˵���ٶ�ʱ��
		//��������������õ�Ŀ���ٶȣ�����ͨ�����ϵ����ﵽ,
	//��currentSpeed��speed�Ĳ�ֵ��������Χ��,����Ϊ�ﵽĿ���ٶ�
	void increaseSpeed();//���ô˺���������ʵ���ٶ�
	void decreaseSpeed();
	 

};
 

#endif

