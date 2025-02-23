#ifndef __BSP_PWMCAPTURE_FREQUENCYANDPROPORTION_H
#define __BSP_PWMCAPTURE_FREQUENCYANDPROPORTION_H


#include "stm32f10x.h"


/************�߼���ʱ��TIM�������壬ֻ��TIM1��TIM8************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// һ����ʱ�����ܲ���һ·PWM�źţ���Ȼ��4��ͨ��������ֻ��ǰ����ͨ������������PWM
//��һ��ͨ����Ƶ�ʣ�����һ��ͨ����ռ�ձ�
/************ʹ��TIME1������һ·PWM�źţ�PWM_IN->PA8************/
#define            Pwm1Capture_TIM                   TIM1
#define            Pwm1Capture_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            Pwm1Capture_TIM_CLK               RCC_APB2Periph_TIM1

// ���벶���ܲ��񵽵���С��Ƶ��Ϊ 72M/{ (ARR+1)*(PSC+1) }
#define            Pwm1Capture_TIM_PERIOD            (10000-1)
#define            Pwm1Capture_TIM_PSC               (144-1)

// �ж���غ궨��
#define            Pwm1Capture_TIM_IRQ               TIM1_CC_IRQn
#define            Pwm1Capture_TIM_IRQHandler        TIM1_CC_IRQHandler

// TIM1 ���벶��ͨ��1
#define            Pwm1Capture_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            Pwm1Capture_TIM_CH1_PORT          GPIOA
#define            Pwm1Capture_TIM_CH1_PIN           GPIO_Pin_8

#define            Pwm1Capture_TIM_IC1PWM_CHANNEL    TIM_Channel_1



/**************************��������********************************/
void Pwm1Capture_TIM_Init(void);


#endif	/* __BSP_Pwm1CaptureTIME_H */


