// MotorController.h
//�������ࣺ����С�����ٶȺͷ����Լ����ֶ���
//С���ᾶ130mm������ֱ��65mm
#ifndef _MOTORCONTROLLER_h
#define _MOTORCONTROLLER_h

#include "arduino.h"
#include"MyMotor.h"
#include"Velometer.h"

class MotorControllerClass
{
public:
	static const int SPEED_ALLOW_MAX_DIFF = 20;//�ٶ�����������ֵ
	static const int STOP = 0;
	static const int FORWARD = 1;
	static const int BACK = 2;
	static const int BRAKE = 3;//�ƶ�
	static const int LEFT = 4;//��ת
	static const int RIGHT = 5;//��ת
	static const int ROUND = 6;//ԭ��תȦ
	static const int TURN_RIGHT = 7;//��ת
	static const int TURN_LEFT = 8;//��ת

	static void onSpeedDetect(VehicleSpeed*);
	static MotorControllerClass* instance;

	static const int FOUR_WHEEL_DRIVER = 1;
	static const int TWO_WHEEL_DRIVER = 2;
	static void onTimer( );

	



public:
	//�ĸ����
	MyMotorClass *m1;
	MyMotorClass *m2;
	MyMotorClass *m3;
	MyMotorClass *m4;
	int state = 0;//��¼��ǰС���н�״̬��0��ֹͣ��1��ǰ����2�����ˣ�3���ƶ�,4,
	int speed = 0;//��¼��ǰС���ٶ�
	VelometerClass velometer;//������
	int mode = TWO_WHEEL_DRIVER;
	static bool left_finished  ;
	static bool right_finished  ;
	bool is_forward_back = false;
	
	



public:
	MotorControllerClass(MyMotorClass* motor1, MyMotorClass* motor2, MyMotorClass* motor3, MyMotorClass* motor4);

	MotorControllerClass(MyMotorClass* motor1, MyMotorClass* motor2);

	//�����ĸ����
	void init(MyMotorClass *motor1, MyMotorClass *motor2, MyMotorClass *motor3, MyMotorClass *motor4);
	//���Ƹ��������
	void init(MyMotorClass* motor1, MyMotorClass* motor2);
	

	
	void forward();
	void setSpeed(int speed);
	void back();
	void left();
	void right();
	void stop();
	void brake();//�ƶ�
	void adjustSpeed(struct VehicleSpeed* speed);//��ʱ�����ٶȣ���Ϊ�����Ħ���͵�ص�ѹӰ����ת�ٶȲ��ȶ���Ҫ��ʱ�Ӽ�����΢��������ָ���ٶ�
	void turnLeft(int degree);//��ת������0~360
	void turnLeft(int degree, void(*f)(int));
	void turnRight(int degree);//��ת������0~360
	void turnRight(int degree, void(*f)(int));
	void back(int distance,void(*f)(int));//��ǰ�ƶ�ָ�����루��λ���ף�
	void back(int distance);
	void forward(int distance);//����ƶ�ָ�����루��λ���ף�
	void forward(int distance,void(*f)(int x));//����ƶ�ָ�����루��λ���ף�
	void forward_back();

	 
 
	VehicleSpeed  getSpeed();
	void openLeftMotorMaxSpeed();//������ߵ������ٶ�
	int getLeftAllGridNum() {
		return velometer.leftSum;
	}
	void continueGo();

	
private:
	void printSpeed(MyMotorClass *m);
	static void on_back_finished(int x);
	

};



#endif

