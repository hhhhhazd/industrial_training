#ifndef __BSP_PWMCAPTURE_PULSEWIDTH_H
#define __BSP_PWMCAPTURE_PULSEWIDTH_H


#include "stm32f10x.h"

//ʹ����Щͨ�� ,����1ʹ�ܣ�����0ʧ��//
#define  CHANNEL_1_ENABLE     1
#define  CHANNEL_2_ENABLE     1
#define  CHANNEL_3_ENABLE     1 
#define  CHANNEL_4_ENABLE     1

/************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4��5************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������Ĭ��ʹ��TIM4
/*ͨ��1��2��3��4�����궨��*/
//ʱ���궨��
#define            PWMCAPTURE_PULSEWIDTH_TIM                   TIM4
#define            PWMCAPTURE_PULSEWIDTH_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            PWMCAPTURE_PULSEWIDTH_TIM_CLK               RCC_APB1Periph_TIM4
#define            PWMCAPTURE_PULSEWIDTH_TIM_PERIOD            (40000-1) //���������������Ϊ40ms
#define            PWMCAPTURE_PULSEWIDTH_TIM_PSC               (72-1)
//�жϺ궨��
#define            PWMCAPTURE_PULSEWIDTH_TIM_IRQ               TIM4_IRQn
#define            PWMCAPTURE_PULSEWIDTH_TIM_INT_FUN           TIM4_IRQHandler

// ��������ʼ����
#define            PWMCAPTURE_PULSEWIDTH_TIM_STRAT_ICPolarity  TIM_ICPolarity_Rising
// �����Ľ�������
#define            PWMCAPTURE_PULSEWIDTH_TIM_END_ICPolarity    TIM_ICPolarity_Falling


/*ͨ��1��غ궨��*/
#if  CHANNEL_1_ENABLE
#define            PWMCAPTURE_PULSEWIDTH1_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            PWMCAPTURE_PULSEWIDTH1_TIM_CH1_PORT          GPIOB
#define            PWMCAPTURE_PULSEWIDTH1_TIM_CH1_PIN           GPIO_Pin_6

//PWM���벶��ͨ��1
#define            PWMCAPTURE_PULSEWIDTH1_TIM_CHANNEL_1         TIM_Channel_1
#define            PWMCAPTURE_PULSEWIDTH1_TIM_IT_CC1            TIM_IT_CC1

// ��ȡ����Ĵ���ֵ�����궨��
#define            PWMCAPTURE_PULSEWIDTH1_TIM_GetCapture1_FUN                 TIM_GetCapture1
// �����źż��Ժ����궨��
#define            PWMCAPTURE_PULSEWIDTH1_TIM_OC1PolarityConfig_FUN           TIM_OC1PolarityConfig
#endif


/*ͨ��2��غ궨��*/
#if  CHANNEL_2_ENABLE
//GPIO��غ궨��PWM���벶��ͨ��2
#define            PWMCAPTURE_PULSEWIDTH2_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            PWMCAPTURE_PULSEWIDTH2_TIM_CH2_PORT          GPIOB
#define            PWMCAPTURE_PULSEWIDTH2_TIM_CH2_PIN           GPIO_Pin_7

//PWM���벶��ͨ��2
#define            PWMCAPTURE_PULSEWIDTH2_TIM_CHANNEL_2         TIM_Channel_2
#define            PWMCAPTURE_PULSEWIDTH2_TIM_IT_CC2            TIM_IT_CC2

// ��ȡ����Ĵ���ֵ�����궨��
#define            PWMCAPTURE_PULSEWIDTH2_TIM_GetCapture2_FUN                 TIM_GetCapture2
// �����źż��Ժ����궨��
#define            PWMCAPTURE_PULSEWIDTH2_TIM_OC2PolarityConfig_FUN           TIM_OC2PolarityConfig
#endif

/*ͨ��3��غ궨��*/
#if  CHANNEL_3_ENABLE
//GPIO��غ궨��PWM���벶��ͨ��2
#define            PWMCAPTURE_PULSEWIDTH3_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            PWMCAPTURE_PULSEWIDTH3_TIM_CH3_PORT          GPIOB
#define            PWMCAPTURE_PULSEWIDTH3_TIM_CH3_PIN           GPIO_Pin_8

//PWM���벶��ͨ��2
#define            PWMCAPTURE_PULSEWIDTH3_TIM_CHANNEL_3         TIM_Channel_3
#define            PWMCAPTURE_PULSEWIDTH3_TIM_IT_CC3            TIM_IT_CC3

// ��ȡ����Ĵ���ֵ�����궨��
#define            PWMCAPTURE_PULSEWIDTH3_TIM_GetCapture3_FUN                 TIM_GetCapture3
// �����źż��Ժ����궨��
#define            PWMCAPTURE_PULSEWIDTH3_TIM_OC3PolarityConfig_FUN           TIM_OC3PolarityConfig
#endif

/*ͨ��4��غ궨��*/
#if  CHANNEL_4_ENABLE
//GPIO��غ궨��PWM���벶��ͨ��2
#define            PWMCAPTURE_PULSEWIDTH4_TIM_CH4_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            PWMCAPTURE_PULSEWIDTH4_TIM_CH4_PORT          GPIOB
#define            PWMCAPTURE_PULSEWIDTH4_TIM_CH4_PIN           GPIO_Pin_9

//PWM���벶��ͨ��2
#define            PWMCAPTURE_PULSEWIDTH4_TIM_CHANNEL_4         TIM_Channel_4
#define            PWMCAPTURE_PULSEWIDTH4_TIM_IT_CC4            TIM_IT_CC4

// ��ȡ����Ĵ���ֵ�����궨��
#define            PWMCAPTURE_PULSEWIDTH4_TIM_GetCapture4_FUN                 TIM_GetCapture4
// �����źż��Ժ����궨��
#define            PWMCAPTURE_PULSEWIDTH4_TIM_OC4PolarityConfig_FUN           TIM_OC4PolarityConfig
#endif

// ��ʱ�����벶���û��Զ�������ṹ������
typedef struct
{   
	uint8_t   Capture_FinishFlag;   // ���������־λ
	uint8_t   Capture_StartFlag;    // ����ʼ��־λ
	uint16_t  Capture_CcrValue;     // ����Ĵ�����ֵ
	uint16_t  Capture_Period;       // �Զ���װ�ؼĴ������±�־ 
}TIM_ICUserValueTypeDef;

#if  CHANNEL_1_ENABLE
extern uint16_t pulse_value1;
#endif

#if  CHANNEL_2_ENABLE
extern uint16_t pulse_value2;
#endif

#if  CHANNEL_3_ENABLE
extern uint16_t pulse_value3;
#endif

#if  CHANNEL_4_ENABLE
extern uint16_t pulse_value4;
#endif

/**************************��������********************************/
void PWMCAPTURE_PULSEWIDTH_TIM_Init(void);


#endif	/* __BSP_PWMCAPTURE_PULSEWIDTH_H */
