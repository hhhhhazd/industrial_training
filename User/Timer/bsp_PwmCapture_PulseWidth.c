#include "bsp_PwmCapture_PulseWidth.h"


// ��ʱ�����벶���û��Զ�������ṹ�嶨��
#if  CHANNEL_1_ENABLE
uint16_t pulse_value1;
TIM_ICUserValueTypeDef TIM_ICUserValue_1_Structure = {0,0,0,0};
#endif

#if  CHANNEL_2_ENABLE
uint16_t pulse_value2;
TIM_ICUserValueTypeDef TIM_ICUserValue_2_Structure = {0,0,0,0};
#endif

#if  CHANNEL_3_ENABLE
uint16_t pulse_value3;
TIM_ICUserValueTypeDef TIM_ICUserValue_3_Structure = {0,0,0,0};
#endif

#if  CHANNEL_4_ENABLE
uint16_t pulse_value4;
TIM_ICUserValueTypeDef TIM_ICUserValue_4_Structure = {0,0,0,0};
#endif

// TIM ������������ʱ�ӣ���72�ж��������time��λ��us
uint32_t TIM_PscCLK = 72/(PWMCAPTURE_PULSEWIDTH_TIM_PSC+1);	

// �ж����ȼ�����
static void PWMCAPTURE_PULSEWIDTH_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = PWMCAPTURE_PULSEWIDTH_TIM_IRQ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ������ռ���ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void PWMCAPTURE_PULSEWIDTH_TIM_GPIO_Config(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;

	#if  CHANNEL_1_ENABLE
    // ���벶��ͨ��1 GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(PWMCAPTURE_PULSEWIDTH1_TIM_CH1_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  PWMCAPTURE_PULSEWIDTH1_TIM_CH1_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(PWMCAPTURE_PULSEWIDTH1_TIM_CH1_PORT, &GPIO_InitStructure);	
	#endif

	#if  CHANNEL_2_ENABLE
	// ���벶��ͨ��2 GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(PWMCAPTURE_PULSEWIDTH2_TIM_CH2_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  PWMCAPTURE_PULSEWIDTH2_TIM_CH2_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(PWMCAPTURE_PULSEWIDTH2_TIM_CH2_PORT, &GPIO_InitStructure);
	#endif

	#if  CHANNEL_3_ENABLE
	// ���벶��ͨ��3GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(PWMCAPTURE_PULSEWIDTH3_TIM_CH3_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  PWMCAPTURE_PULSEWIDTH3_TIM_CH3_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(PWMCAPTURE_PULSEWIDTH3_TIM_CH3_PORT, &GPIO_InitStructure);
	#endif	

	#if  CHANNEL_4_ENABLE
	// ���벶��ͨ��4 GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(PWMCAPTURE_PULSEWIDTH4_TIM_CH4_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  PWMCAPTURE_PULSEWIDTH4_TIM_CH4_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(PWMCAPTURE_PULSEWIDTH4_TIM_CH4_PORT, &GPIO_InitStructure);
	#endif		
}

// * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
// * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
// * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ����
// *	TIM_CounterMode			     TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef; 
// *-----------------------------------------------------------------------------


/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)

static void PWMCAPTURE_PULSEWIDTH_TIM_Mode_Config(void)
{
  // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	PWMCAPTURE_PULSEWIDTH_TIM_APBxClock_FUN(PWMCAPTURE_PULSEWIDTH_TIM_CLK,ENABLE);

/*--------------------ʱ���ṹ���ʼ��-------------------------*/	
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=PWMCAPTURE_PULSEWIDTH_TIM_PERIOD;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= PWMCAPTURE_PULSEWIDTH_TIM_PSC;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(PWMCAPTURE_PULSEWIDTH_TIM, &TIM_TimeBaseStructure);


	/*--------------------���벶��ṹ���ʼ��-------------------*/	
	TIM_ICInitTypeDef TIM_ICInitStructure;

	#if  CHANNEL_1_ENABLE
	//CHANNEL__1
	// �������벶���ͨ������Ҫ���ݾ����GPIO������
	TIM_ICInitStructure.TIM_Channel = PWMCAPTURE_PULSEWIDTH1_TIM_CHANNEL_1;
	// ���벶���źŵļ�������
	TIM_ICInitStructure.TIM_ICPolarity = PWMCAPTURE_PULSEWIDTH_TIM_STRAT_ICPolarity;
	// ����ͨ���Ͳ���ͨ����ӳ���ϵ����ֱ���ͷ�ֱ������
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// �������Ҫ��������źŵķ�Ƶϵ��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// �������Ҫ��������źŵ��˲�ϵ��
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// ��ʱ�����벶���ʼ��
	TIM_ICInit(PWMCAPTURE_PULSEWIDTH_TIM, &TIM_ICInitStructure);
    // ������ºͲ����жϱ�־λ
    TIM_ClearFlag(PWMCAPTURE_PULSEWIDTH_TIM, TIM_FLAG_Update|PWMCAPTURE_PULSEWIDTH1_TIM_IT_CC1);	
    // ���������ж�  
	TIM_ITConfig (PWMCAPTURE_PULSEWIDTH_TIM, PWMCAPTURE_PULSEWIDTH1_TIM_IT_CC1, ENABLE );
	#endif

	#if  CHANNEL_2_ENABLE	
	//CHANNEL__2
	// �������벶���ͨ������Ҫ���ݾ����GPIO������
	TIM_ICInitStructure.TIM_Channel = PWMCAPTURE_PULSEWIDTH2_TIM_CHANNEL_2;
	// ���벶���źŵļ�������
	TIM_ICInitStructure.TIM_ICPolarity = PWMCAPTURE_PULSEWIDTH_TIM_STRAT_ICPolarity;
	// ����ͨ���Ͳ���ͨ����ӳ���ϵ����ֱ���ͷ�ֱ������
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// �������Ҫ��������źŵķ�Ƶϵ��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// �������Ҫ��������źŵ��˲�ϵ��
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// ��ʱ�����벶���ʼ��
	TIM_ICInit(PWMCAPTURE_PULSEWIDTH_TIM, &TIM_ICInitStructure);

	// ������ºͲ����жϱ�־λ
    TIM_ClearFlag(PWMCAPTURE_PULSEWIDTH_TIM, TIM_FLAG_Update|PWMCAPTURE_PULSEWIDTH2_TIM_IT_CC2);	
    // ���������ж�  
	TIM_ITConfig (PWMCAPTURE_PULSEWIDTH_TIM, PWMCAPTURE_PULSEWIDTH2_TIM_IT_CC2, ENABLE );
	#endif

	#if  CHANNEL_3_ENABLE	
	//CHANNEL__3
	// �������벶���ͨ������Ҫ���ݾ����GPIO������
	TIM_ICInitStructure.TIM_Channel = PWMCAPTURE_PULSEWIDTH3_TIM_CHANNEL_3;
	// ���벶���źŵļ�������
	TIM_ICInitStructure.TIM_ICPolarity = PWMCAPTURE_PULSEWIDTH_TIM_STRAT_ICPolarity;
	// ����ͨ���Ͳ���ͨ����ӳ���ϵ����ֱ���ͷ�ֱ������
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// �������Ҫ��������źŵķ�Ƶϵ��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// �������Ҫ��������źŵ��˲�ϵ��
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// ��ʱ�����벶���ʼ��
	TIM_ICInit(PWMCAPTURE_PULSEWIDTH_TIM, &TIM_ICInitStructure);

	// ������ºͲ����жϱ�־λ
    TIM_ClearFlag(PWMCAPTURE_PULSEWIDTH_TIM, TIM_FLAG_Update|PWMCAPTURE_PULSEWIDTH3_TIM_IT_CC3);	
    // ���������ж�  
	TIM_ITConfig (PWMCAPTURE_PULSEWIDTH_TIM, PWMCAPTURE_PULSEWIDTH3_TIM_IT_CC3, ENABLE );
	#endif

	#if  CHANNEL_4_ENABLE	
	//CHANNEL__4
	// �������벶���ͨ������Ҫ���ݾ����GPIO������
	TIM_ICInitStructure.TIM_Channel = PWMCAPTURE_PULSEWIDTH4_TIM_CHANNEL_4;
	// ���벶���źŵļ�������
	TIM_ICInitStructure.TIM_ICPolarity = PWMCAPTURE_PULSEWIDTH_TIM_STRAT_ICPolarity;
	// ����ͨ���Ͳ���ͨ����ӳ���ϵ����ֱ���ͷ�ֱ������
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// �������Ҫ��������źŵķ�Ƶϵ��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// �������Ҫ��������źŵ��˲�ϵ��
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// ��ʱ�����벶���ʼ��
	TIM_ICInit(PWMCAPTURE_PULSEWIDTH_TIM, &TIM_ICInitStructure);

	// ������ºͲ����жϱ�־λ
    TIM_ClearFlag(PWMCAPTURE_PULSEWIDTH_TIM, TIM_FLAG_Update|PWMCAPTURE_PULSEWIDTH4_TIM_IT_CC4);	
    // ���������ж�  
	TIM_ITConfig (PWMCAPTURE_PULSEWIDTH_TIM, PWMCAPTURE_PULSEWIDTH4_TIM_IT_CC4, ENABLE );
	#endif

	// ʹ�ܼ�����
	TIM_Cmd(PWMCAPTURE_PULSEWIDTH_TIM, ENABLE);
}

void PWMCAPTURE_PULSEWIDTH_TIM_Init(void)
{
	PWMCAPTURE_PULSEWIDTH_TIM_GPIO_Config();
	PWMCAPTURE_PULSEWIDTH_TIM_NVIC_Config();
	PWMCAPTURE_PULSEWIDTH_TIM_Mode_Config();		
}


//�жϺ���
void PWMCAPTURE_PULSEWIDTH_TIM_INT_FUN(void)
{

#if  CHANNEL_1_ENABLE
	/*CHANNEL__1*/
	// �����ز����ж�
	if ( TIM_GetITStatus (PWMCAPTURE_PULSEWIDTH_TIM, PWMCAPTURE_PULSEWIDTH1_TIM_IT_CC1) != RESET)
	{
		// ��һ�β���
		if (TIM_ICUserValue_1_Structure.Capture_StartFlag == 0 )
		{
			// ��������0
			TIM_SetCounter ( PWMCAPTURE_PULSEWIDTH_TIM, 0 );
			// �Զ���װ�ؼĴ������±�־��0
			TIM_ICUserValue_1_Structure.Capture_Period = 0;
      		// �沶��ȽϼĴ�����ֵ�ı�����ֵ��0			
			TIM_ICUserValue_1_Structure.Capture_CcrValue = 0;

			// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���
			PWMCAPTURE_PULSEWIDTH1_TIM_OC1PolarityConfig_FUN(PWMCAPTURE_PULSEWIDTH_TIM, TIM_ICPolarity_Falling);
      		// ��ʼ�����׼��1			
			TIM_ICUserValue_1_Structure.Capture_StartFlag = 1;			
		}
		// �½��ز����ж�
		else // �ڶ��β���
		{
			// ��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ
			TIM_ICUserValue_1_Structure.Capture_CcrValue = 
			PWMCAPTURE_PULSEWIDTH1_TIM_GetCapture1_FUN (PWMCAPTURE_PULSEWIDTH_TIM);

			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ��ÿ����µ�һ�ֲ���
			PWMCAPTURE_PULSEWIDTH1_TIM_OC1PolarityConfig_FUN(PWMCAPTURE_PULSEWIDTH_TIM, TIM_ICPolarity_Rising);
      		// ��ʼ�����־��0		
			TIM_ICUserValue_1_Structure.Capture_StartFlag = 0;
      		// ������ɱ�־��1			
			TIM_ICUserValue_1_Structure.Capture_FinishFlag = 1;		
		}			
		
		if(TIM_ICUserValue_1_Structure.Capture_FinishFlag == 1)
		{
			// ����ߵ�ƽʱ��ļ�������ֵ
			pulse_value1 = (TIM_ICUserValue_1_Structure.Capture_Period * (PWMCAPTURE_PULSEWIDTH_TIM_PERIOD+1) + 
			       (TIM_ICUserValue_1_Structure.Capture_CcrValue+1))/TIM_PscCLK;		
			// ��ӡ�ߵ�ƽ����ʱ��
			//printf ( "\r\nCHANNEL__1��%d us\r\n",pulse_value1);			
			TIM_ICUserValue_1_Structure.Capture_FinishFlag = 0;			 	
	    }		
			TIM_ClearITPendingBit (PWMCAPTURE_PULSEWIDTH_TIM,PWMCAPTURE_PULSEWIDTH1_TIM_IT_CC1);	
	}		
#endif

#if  CHANNEL_2_ENABLE
	/*CHANNEL__2*/
	// �����ز����ж�
	if ( TIM_GetITStatus (PWMCAPTURE_PULSEWIDTH_TIM, PWMCAPTURE_PULSEWIDTH2_TIM_IT_CC2) != RESET)
	{
		// ��һ�β���
		if ( TIM_ICUserValue_2_Structure.Capture_StartFlag == 0 )   
		{
			// ��������0
			TIM_SetCounter ( PWMCAPTURE_PULSEWIDTH_TIM, 0 );
			// �Զ���װ�ؼĴ������±�־��0
			TIM_ICUserValue_2_Structure.Capture_Period = 0;
      		// �沶��ȽϼĴ�����ֵ�ı�����ֵ��0			
			TIM_ICUserValue_2_Structure.Capture_CcrValue = 0;

			// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���
			PWMCAPTURE_PULSEWIDTH2_TIM_OC2PolarityConfig_FUN(PWMCAPTURE_PULSEWIDTH_TIM, TIM_ICPolarity_Falling);
      		// ��ʼ�����׼��1			
			TIM_ICUserValue_2_Structure.Capture_StartFlag = 1;			
		}
		// �½��ز����ж�
		else // �ڶ��β���
		{
			// ��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ
			TIM_ICUserValue_2_Structure.Capture_CcrValue = 
			PWMCAPTURE_PULSEWIDTH2_TIM_GetCapture2_FUN (PWMCAPTURE_PULSEWIDTH_TIM);

			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ��ÿ����µ�һ�ֲ���
			PWMCAPTURE_PULSEWIDTH2_TIM_OC2PolarityConfig_FUN(PWMCAPTURE_PULSEWIDTH_TIM, TIM_ICPolarity_Rising);
      		// ��ʼ�����־��0		
			TIM_ICUserValue_2_Structure.Capture_StartFlag = 0;
      		// ������ɱ�־��1			
			TIM_ICUserValue_2_Structure.Capture_FinishFlag = 1;		
		}			
			
		
		if(TIM_ICUserValue_2_Structure.Capture_FinishFlag == 1)
		{
			// ����ߵ�ƽʱ��ļ�������ֵ
			pulse_value2 = (TIM_ICUserValue_2_Structure.Capture_Period * (PWMCAPTURE_PULSEWIDTH_TIM_PERIOD+1) + 
			       (TIM_ICUserValue_2_Structure.Capture_CcrValue+1))/TIM_PscCLK;		                         
			// ��ӡ�ߵ�ƽ����ʱ��
			// printf ( "\r\nCHANNEL_2��%d us\r\n",pulse_value2);			
			TIM_ICUserValue_2_Structure.Capture_FinishFlag = 0;		
	    }		
			TIM_ClearITPendingBit (PWMCAPTURE_PULSEWIDTH_TIM,PWMCAPTURE_PULSEWIDTH2_TIM_IT_CC2);
	}		
#endif

#if  CHANNEL_3_ENABLE
		/*CHANNEL__3*/
	// �����ز����ж�
	if ( TIM_GetITStatus (PWMCAPTURE_PULSEWIDTH_TIM, PWMCAPTURE_PULSEWIDTH3_TIM_IT_CC3) != RESET)
	{
		// ��һ�β���
		if ( TIM_ICUserValue_3_Structure.Capture_StartFlag == 0 )   
		{
			// ��������0
			TIM_SetCounter ( PWMCAPTURE_PULSEWIDTH_TIM, 0 );
			// �Զ���װ�ؼĴ������±�־��0
			TIM_ICUserValue_3_Structure.Capture_Period = 0;
      		// �沶��ȽϼĴ�����ֵ�ı�����ֵ��0			
			TIM_ICUserValue_3_Structure.Capture_CcrValue = 0;

			// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���
			PWMCAPTURE_PULSEWIDTH3_TIM_OC3PolarityConfig_FUN(PWMCAPTURE_PULSEWIDTH_TIM, TIM_ICPolarity_Falling);
      		// ��ʼ�����׼��1			
			TIM_ICUserValue_3_Structure.Capture_StartFlag = 1;			
		}
		// �½��ز����ж�
		else // �ڶ��β���
		{
			// ��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ
			TIM_ICUserValue_3_Structure.Capture_CcrValue = 
			PWMCAPTURE_PULSEWIDTH3_TIM_GetCapture3_FUN (PWMCAPTURE_PULSEWIDTH_TIM);

			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ��ÿ����µ�һ�ֲ���
			PWMCAPTURE_PULSEWIDTH3_TIM_OC3PolarityConfig_FUN(PWMCAPTURE_PULSEWIDTH_TIM, TIM_ICPolarity_Rising);
      		// ��ʼ�����־��0		
			TIM_ICUserValue_3_Structure.Capture_StartFlag = 0;
      		// ������ɱ�־��1			
			TIM_ICUserValue_3_Structure.Capture_FinishFlag = 1;		
		}			
			
		
		if(TIM_ICUserValue_3_Structure.Capture_FinishFlag == 1)
		{
			// ����ߵ�ƽʱ��ļ�������ֵ
			pulse_value3 = (TIM_ICUserValue_3_Structure.Capture_Period * (PWMCAPTURE_PULSEWIDTH_TIM_PERIOD+1) + 
			       (TIM_ICUserValue_3_Structure.Capture_CcrValue+1))/TIM_PscCLK;		                         
			// ��ӡ�ߵ�ƽ����ʱ��
			// printf ( "\r\nCHANNEL_3��%d us\r\n",pulse_value3);		
			TIM_ICUserValue_3_Structure.Capture_FinishFlag = 0;			
	    }		
			TIM_ClearITPendingBit (PWMCAPTURE_PULSEWIDTH_TIM,PWMCAPTURE_PULSEWIDTH3_TIM_IT_CC3);
	}		
#endif

#if  CHANNEL_4_ENABLE
	/*CHANNEL__4*/
	// �����ز����ж�
	if ( TIM_GetITStatus (PWMCAPTURE_PULSEWIDTH_TIM, PWMCAPTURE_PULSEWIDTH4_TIM_IT_CC4) != RESET)
	{
		// ��һ�β���
		if ( TIM_ICUserValue_4_Structure.Capture_StartFlag == 0 )   
		{
			// ��������0
			TIM_SetCounter ( PWMCAPTURE_PULSEWIDTH_TIM, 0 );
			// �Զ���װ�ؼĴ������±�־��0
			TIM_ICUserValue_4_Structure.Capture_Period = 0;
      		// �沶��ȽϼĴ�����ֵ�ı�����ֵ��0			
			TIM_ICUserValue_4_Structure.Capture_CcrValue = 0;

			// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���
			PWMCAPTURE_PULSEWIDTH4_TIM_OC4PolarityConfig_FUN(PWMCAPTURE_PULSEWIDTH_TIM, TIM_ICPolarity_Falling);
      		// ��ʼ�����׼��1			
			TIM_ICUserValue_4_Structure.Capture_StartFlag = 1;			
		}
		// �½��ز����ж�
		else // �ڶ��β���
		{
			// ��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ
			TIM_ICUserValue_4_Structure.Capture_CcrValue = 
			PWMCAPTURE_PULSEWIDTH4_TIM_GetCapture4_FUN (PWMCAPTURE_PULSEWIDTH_TIM);

			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ��ÿ����µ�һ�ֲ���
			PWMCAPTURE_PULSEWIDTH4_TIM_OC4PolarityConfig_FUN(PWMCAPTURE_PULSEWIDTH_TIM, TIM_ICPolarity_Rising);
      		// ��ʼ�����־��0		
			TIM_ICUserValue_4_Structure.Capture_StartFlag = 0;
      		// ������ɱ�־��1			
			TIM_ICUserValue_4_Structure.Capture_FinishFlag = 1;		
		}			
			
		
		if(TIM_ICUserValue_4_Structure.Capture_FinishFlag == 1)
		{
			// ����ߵ�ƽʱ��ļ�������ֵ
			pulse_value4 = (TIM_ICUserValue_4_Structure.Capture_Period * (PWMCAPTURE_PULSEWIDTH_TIM_PERIOD+1) + 
			       (TIM_ICUserValue_4_Structure.Capture_CcrValue+1))/TIM_PscCLK;		                         
			// ��ӡ�ߵ�ƽ����ʱ��
			// printf ( "\r\nCHANNEL_4��%d us\r\n",pulse_value4);			
			TIM_ICUserValue_4_Structure.Capture_FinishFlag = 0;				
	    }		
			TIM_ClearITPendingBit (PWMCAPTURE_PULSEWIDTH_TIM,PWMCAPTURE_PULSEWIDTH4_TIM_IT_CC4);	
	}	
#endif	
}

/*********************************************END OF FILE**********************/
