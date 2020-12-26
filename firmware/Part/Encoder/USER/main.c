#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"



/*
-------------------------------------IO口对应功能表-------------------------------------------------------
电机驱动：                B12->BIN2       B13->BIN1      B14->AIN1     B15->AIN2
	PWM   ：                A8 ->PWMA                        A11->PWMB
MUP6050 :                 B8 ->SCL                         B9 ->SDA
电机编码器：              B6 B7 一组                       A0 A1一组
                    电机负值为正方向，速度最大值在60左右

*/

int Encoder_Left, Encoder_Right;      //左右编码器的脉冲计数

/**************************************************************************
函数功能：主函数
入口参数：无
返回  值：无
**************************************************************************/

int main(void)
{

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();	    						  //延时函数初始化
    uart_init(115200);	 							//串口初始化为9600
    LED_Init();		  								//初始化与LED连接的硬件接口

    Encoder_Init_TIM2();            //=====编码器接口
    Encoder_Init_TIM4();            //=====初始化编码器2

    while(1)
    {
        Encoder_Left = Read_Encoder(2);                           //===读取编码器的值
        Encoder_Right = Read_Encoder(4);
        printf(" \n\r  Encoder_Left= %d   \n\r ",Encoder_Left);
        delay_ms(200);
        LED0 =!LED0;

    }
}

