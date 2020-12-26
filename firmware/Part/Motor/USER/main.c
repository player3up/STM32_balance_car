#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "motor.h"


/*
-------------------------------------IO口对应功能表-------------------------------------------------------
电机驱动：                B12->BIN2       B13->BIN1      B14->AIN1     B15->AIN2
	PWM   ：                A8 ->PWMA                        A11->PWMB
MUP6050 :                 B8 ->SCL                         B9 ->SDA
电机编码器：              B6 B7 一组                       A0 A1一组
                    电机负值为正方向，速度最大值在60左右

*/

u8 stop_flag = 0;    										//小车停止标志位
extern int Moto1, Moto2;  							//电机PWM变量

/**************************************************************************
函数功能：主函数
入口参数：无
返回  值：无
**************************************************************************/

int main(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();	    						  //延时函数初始化
    uart_init(9600);	 							//串口初始化为9600
    LED_Init();		  								//初始化与LED连接的硬件接口
    MiniBalance_Motor_Init();
    MiniBalance_PWM_Init(7199, 0);  //初始化PWM 10KHZ，用于驱动电机 ,PWM最多7200

    while(1)
    {
        delay_ms(200);
        LED0 = ~LED0;
        Set_Pwm(7199 ,7199);  																	//设置PWM

    }
}

