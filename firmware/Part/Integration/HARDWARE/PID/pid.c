#include "pid.h"
#include "MPU6050.h"
#include "motor.h"
#include "encoder.h"
PID pid;
void PID_Init()
{
    /*平衡PID环控制参数初始化*/
    pid.Sv =  8;
    pid.Kp = 650;
    pid.Kd = -5;

    /*速度PID环控制参数初始化*/
    pid.target_speed = 0;
    pid.Kp_speed = 100 ;   //max 130 about
    pid.Ki_speed = 0.56;//pid.Kp_speed/200;
    pid.Least_speed = 0;
    pid.Encoder = 0;
    pid.Encoder_Integral = 0;


    /*转向PID环控制参数初始化*/
    pid.Kp_turn = 35;
    pid.Angle_turn = 0;
}

/*小车平衡环瞫糠郑⒎?比例控制
微分变量为直接读取的加速度*/

int balance(float Angle)
{

    signed  int  Bias;
    int bal;
    extern short gyrox;
    Bias = (Angle - pid.Sv);
    bal = pid.Kp * Bias + gyrox * pid.Kd;
    return bal;
}

/*小车速度环部分， 积分+比例控制*/
int velocity(int encoder_left, int encoder_right)
{

    static int vel;
    pid.Least_speed =  (encoder_left + encoder_right) - pid.target_speed;
    pid.Encoder *= 0.7;
    pid.Encoder += pid.Least_speed * 0.3;
    pid.Encoder_Integral += pid.Encoder;
    vel = pid.Kp_speed * pid.Encoder + (int)(pid.Ki_speed * pid.Encoder_Integral);
    return vel;
}

void calculation(void)
{
    extern int Encoder_Left, Encoder_Right;      //左右编码器的脉冲计数
    extern short gyrox, gyroy, gyroz;							//陀螺仪原始数据
    extern float Pitch, Roll, Yaw; 								//欧拉角
    extern int Moto1, Moto2;  							//电机PWM变量

    Read_DMP  (&Pitch, &Roll, &Yaw);
    MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);									//得到陀螺仪数据
    Encoder_Left = Read_Encoder(2);                           //===读取编码器的值
    Encoder_Right = Read_Encoder(4);

    pid.Pv = Roll ;																				//角度*十倍
    printf("\n\r 偏航角=%.2f \n\r",  pid.Pv);
    Moto1 = balance(pid.Pv)+ velocity(Encoder_Left, Encoder_Right) ;		//获得PWM输出值
    Moto2 = balance(pid.Pv)+ velocity(Encoder_Left, Encoder_Right) ;
    //printf("\n\r 偏航角=%.d \n\r", Moto1 );
    //printf(" \n\r  Encoder_Left= %d   Encoder_Right= %d   \n\r ", Encoder_Left,Encoder_Left);
    Xianfu_Pwm();																							//对PWM进行限幅
    Set_Pwm(Moto1, Moto2);  																		//设置PWM
}

