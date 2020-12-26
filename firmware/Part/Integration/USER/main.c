#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "string.h"
#include "led.h"
#include "TIME.h"
#include "mpu6050.h"
#include "motor.h"
#include "pid.h"

//#define IMU_GYRO
//#define IMU_ANGLE

int temp;					//温度
short aacx, aacy, aacz;		//加速度传感器原始数据
short gyrox, gyroy, gyroz;	//陀螺仪原始数据
float Pitch, Roll, Yaw;
extern int Moto1, Moto2;  							//电机PWM变量
int Encoder_Left, Encoder_Right;      //左右编码器的脉冲计数

int main(void)
{
    //u8 string[10] = {0};
    delay_init();
    uart_init(115200);
    LED_Init();

    IIC_Init();
    MPU6050_initialize();     //=====MPU6050初始化
    DMP_Init();
    PID_Init();
    TIM3_Config(99, 7199);		//10ms任务定时中断

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    MiniBalance_Motor_Init();
    MiniBalance_PWM_Init(7199, 0);  //初始化PWM 10KHZ，用于驱动电机 ,PWM最多7200

    Encoder_Init_TIM2();            //=====编码器接口
    Encoder_Init_TIM4();            //=====初始化编码器2

    while(1)
    {
        delay_ms(10);
        #ifdef IMU_GYRO
        MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);	//得到陀螺仪数据
        printf("\n\r x=%.d      y=%.d     z=%.d \n\r", gyrox, gyroy, gyroz);
        #endif

        #ifdef IMU_ANGLE
        Read_DMP(&Pitch, &Roll, &Yaw);
        printf("\n\r 俯仰角=%.2f      横滚角=%.2f      偏航角=%.2f \n\r", Pitch, Roll, Yaw);
        #endif
        //    pv = Roll ;
//    Moto1 = balance(pv) ;		//获得PWM输出值
//    Moto2 = balance(pv) ;
//    //printf("\n\r moto=%.2d   ",Moto1 );
//    Xianfu_Pwm();																							//对PWM进行限幅
//    Set_Pwm(Moto1, Moto2);  																		//设置PWM


    }
}
