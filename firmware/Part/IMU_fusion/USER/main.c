#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "string.h"		
#include "led.h"
#include "TIME.h"
#include "mpu6050.h"


float Pitch,Roll,Yaw;
int temp;					//温度	
short aacx,aacy,aacz;		//加速度传感器原始数据
short gyrox,gyroy,gyroz;	//陀螺仪原始数据
int main(void)
{	
	//u8 string[10] = {0};	
	delay_init();
	uart_init(115200);
	LED_Init();

	IIC_Init();
	MPU6050_initialize();     //=====MPU6050初始化	
	DMP_Init();
	
	TIM2_Getsample_Int(1999,719);		//50ms任务定时中断
  
	while(1)
	{	
    
//    temp= Read_Temperature();
//    MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
//		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
//    printf(" \n\r aacx=%.d  temp=%.d  \n\r",aacx,temp);
//    delay_ms(20);
    /*********
		delay_ms(999);
		sprintf((char *)string,"Pitch:%.2f",Pitch);//0300
		
		sprintf((char *)string,"Roll :%.2f",Roll);//0300

		sprintf((char *)string,"Yaw  :%.2f",Yaw);//0300
    *********/
	
  }
}
