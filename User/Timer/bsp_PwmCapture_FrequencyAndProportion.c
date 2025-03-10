#include "bsp_PwmCapture_FrequencyAndProportion.h" 
#include "bsp_usart.h"
 /**
  * @brief  高级控制定时器 TIMx,x[1,8]中断优先级配置
  * @param  无
  * @retval 无
  */
static void Pwm1Capture_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 	
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = Pwm1Capture_TIM_IRQ; 	
		// 设置抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置子优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  高级定时器PWM输入用到的GPIO初始化
  * @param  无
  * @retval 无
  */
static void Pwm1Capture_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(Pwm1Capture_TIM_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  Pwm1Capture_TIM_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_Init(Pwm1Capture_TIM_CH1_PORT, &GPIO_InitStructure);	
}


///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

/**
  * @brief  高级定时器PWM输入初始化和用到的GPIO初始化
  * @param  无
  * @retval 无
  */
static void Pwm1Capture_TIM_Mode_Config(void)
{
  // 开启定时器时钟,即内部时钟CK_INT=72M
	Pwm1Capture_TIM_APBxClock_FUN(Pwm1Capture_TIM_CLK,ENABLE);

/*--------------------时基结构体初始化-------------------------*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=Pwm1Capture_TIM_PERIOD;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= Pwm1Capture_TIM_PSC;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(Pwm1Capture_TIM, &TIM_TimeBaseStructure);

	/*--------------------输入捕获结构体初始化-------------------*/	
  // 使用PWM输入模式时，需要占用两个捕获寄存器，一个测周期，另外一个测占空比
	
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	// 捕获通道IC1配置
	// 选择捕获通道
  TIM_ICInitStructure.TIM_Channel = Pwm1Capture_TIM_IC1PWM_CHANNEL;
	// 设置捕获的边沿
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	// 设置捕获通道的信号来自于哪个输入通道，有直连和非直连两种
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// 1分频，即捕获信号的每个有效边沿都捕获
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// 不滤波
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
	// 初始化PWM输入模式
  TIM_PWMIConfig(Pwm1Capture_TIM, &TIM_ICInitStructure);
	
	// 当工作做PWM输入模式时,只需要设置触发信号的那一路即可（用于测量周期）
	// 另外一路（用于测量占空比）会由硬件自带设置，不需要再配置
	
	// 捕获通道IC2配置	
//	TIM_ICInitStructure.TIM_Channel = Pwm1Capture_TIM_IC1PWM_CHANNEL;
//  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
//  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI;
//  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//  TIM_ICInitStructure.TIM_ICFilter = 0x0;
//  TIM_PWMIConfig(Pwm1Capture_TIM, &TIM_ICInitStructure);
	
	// 选择输入捕获的触发信号
  TIM_SelectInputTrigger(Pwm1Capture_TIM, TIM_TS_TI1FP1);		

	// 选择从模式: 复位模式
	// PWM输入模式时,从模式必须工作在复位模式，当捕获开始时,计数器CNT会被复位
  TIM_SelectSlaveMode(Pwm1Capture_TIM, TIM_SlaveMode_Reset);
  TIM_SelectMasterSlaveMode(Pwm1Capture_TIM,TIM_MasterSlaveMode_Enable); 

  // 使能捕获中断,这个中断针对的是主捕获通道（测量周期那个）
  TIM_ITConfig(Pwm1Capture_TIM, TIM_IT_CC1, ENABLE);	
	// 清除中断标志位
	TIM_ClearITPendingBit(Pwm1Capture_TIM, TIM_IT_CC1);
	
	 // 使能高级控制定时器，计数器开始计数
  TIM_Cmd(Pwm1Capture_TIM, ENABLE);
}
/**
  * @brief  高级定时器PWM输入初始化和用到的GPIO初始化
  * @param  无
  * @retval 无
  */
void Pwm1Capture_TIM_Init(void)
{
	Pwm1Capture_TIM_GPIO_Config();
	Pwm1Capture_TIM_NVIC_Config();
	Pwm1Capture_TIM_Mode_Config();		
}

__IO uint16_t IC2Value = 0;
__IO uint16_t IC1Value = 0;
__IO float DutyCycle = 0;
__IO float Frequency = 0;

/*
 * 如果是第一个上升沿中断，计数器会被复位，锁存到CCR1寄存器的值是0，CCR2寄存器的值也是0
 * 无法计算频率和占空比。当第二次上升沿到来的时候，CCR1和CCR2捕获到的才是有效的值。其中
 * CCR1对应的是周期，CCR2对应的是占空比。
 */
void Pwm1Capture_TIM_IRQHandler(void)
{
  /* 清除中断标志位 */
  TIM_ClearITPendingBit(Pwm1Capture_TIM, TIM_IT_CC1);

  /* 获取输入捕获值 */
  IC1Value = TIM_GetCapture1(Pwm1Capture_TIM);
  IC2Value = TIM_GetCapture2(Pwm1Capture_TIM);
	
  // 注意：捕获寄存器CCR1和CCR2的值在计算占空比和频率的时候必须加1
	if (IC1Value != 0)
  {
    /* 占空比计算 */
    DutyCycle = (float)((IC2Value+1) * 100) / (IC1Value+1);

    /* 频率计算 */
    Frequency = (72000000/(Pwm1Capture_TIM_PSC+1))/(float)(IC1Value+1);
		printf("占空比：%0.2f%%   频率：%0.2fHz\r\n",DutyCycle,Frequency);
  }
  else
  {
    DutyCycle = 0;
    Frequency = 0;
  }
}

/*********************************************END OF FILE**********************/
