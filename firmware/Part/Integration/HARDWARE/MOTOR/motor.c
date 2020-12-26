#include "motor.h"
int Moto1, Moto2; //电机PWM变量

/**************************************************************************
函数功能：电机驱动输出IO初始化
入口参数：无
返回  值：无
**************************************************************************/
void MiniBalance_Motor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能PB端口时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;	//端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
    GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB
}
/**************************************************************************
函数功能：电机驱动PWM输出IO口初始化
入口参数：无
返回  值：无
**************************************************************************/
void MiniBalance_PWM_Init(u16 arr, u16 psc)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    MiniBalance_Motor_Init();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //使能GPIO外设时钟使能
    //设置该引脚为复用输出功能,输出TIM1 CH1 CH4的PWM脉冲波形
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11; //TIM_CH1 //TIM_CH4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位


    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_Pulse = 0;                           //设置待装入捕获比较寄存器的脉冲值
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx

    TIM_CtrlPWMOutputs(TIM1, ENABLE);	//MOE 主输出使能

    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1预装载使能
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH4预装载使能
    TIM_SetCompare1(TIM1, 2000);
    TIM_SetCompare4(TIM1, 2000);
    TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器

    TIM_Cmd(TIM1, ENABLE);  //使能TIM1

}
/**************************************************************************
函数功能：获得整数绝对值函数
入口参数：整数
返回  值：绝对值
**************************************************************************/
int myabs(int a)
{
    int temp;

    if(a < 0)  temp = -a;
    else temp = a;

    return temp;
}
/**************************************************************************
函数功能：最后设置PWM函数，并检测是否关闭电机
入口参数：无
返回  值：无
**************************************************************************/
u8 stop_flag;
void Set_Pwm(int moto1, int moto2)
{
    if(stop_flag == 0)
    {
        if(moto1 > 0)			AIN2 = 0,			AIN1 = 1;
        else 	          AIN2 = 1,			AIN1 = 0;

        PWMA = myabs(moto1);

        if(moto2 > 0)	BIN1 = 0,			BIN2 = 1;
        else        BIN1 = 1,			BIN2 = 0;

        PWMB = myabs(moto2);
    }

}
/**************************************************************************
函数功能：PWM限幅函数
入口参数：无
返回  值：无
**************************************************************************/
void Xianfu_Pwm(void)
{
    int Amplitude = 7000;  //===PWM满幅是7200 限制在6900

    if(Moto1 < -Amplitude) Moto1 = -Amplitude;

    if(Moto1 > Amplitude)  Moto1 = Amplitude;

    if(Moto2 < -Amplitude) Moto2 = -Amplitude;

    if(Moto2 > Amplitude)  Moto2 = Amplitude;

}
/**************************************************************************
函数功能：电机异常关闭函数
入口参数：角度
返回  值：1：关闭，0：不关闭
**************************************************************************/
u8 Turn_Off(signed int angle)
{
    u8 temp = 0;

    if(angle < -400 || angle > 400)
    {
        //===倾角大于40度关闭电机
        temp = 1;                                          //===Flag_Stop置1关闭电机
        AIN1 = 0;
        AIN2 = 0;
        BIN1 = 0;
        BIN2 = 0;
    }

    return temp;
}
