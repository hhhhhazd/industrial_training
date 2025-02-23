#ifndef __BSP_PWM_H
#define __BSP_PWM_H


#include "stm32f10x.h"

//�������������źţ�CCR1_Valȡֵ����Ϊ[0,200]
/************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4��5************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������ʹ��TIM3������ĸ�ͨ����PWM
//PWM1_1->PA6��PWM1_2->PA7��PWM1_3->PB0��PWM1_4->PB1
//PWM2_1->PC6��PWM2_2->PC7��PWM2_3->PC8��PWM2_4->PC9

/**************************PWM1********************************/
#define            PWM1_TIM                   TIM3
#define            PWM1_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            PWM1_TIM_CLK               RCC_APB1Periph_TIM3
#define            PWM1_TIM_Period            2000
#define            PWM1_TIM_Prescaler         720
// TIM3 ����Ƚ�ͨ��1
#define            PWM1_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            PWM1_TIM_CH1_PORT          GPIOA
#define            PWM1_TIM_CH1_PIN           GPIO_Pin_6

// TIM3 ����Ƚ�ͨ��2
#define            PWM1_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            PWM1_TIM_CH2_PORT          GPIOA
#define            PWM1_TIM_CH2_PIN           GPIO_Pin_7

// TIM3 ����Ƚ�ͨ��3
#define            PWM1_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            PWM1_TIM_CH3_PORT          GPIOB
#define            PWM1_TIM_CH3_PIN           GPIO_Pin_0

// TIM3 ����Ƚ�ͨ��4
#define            PWM1_TIM_CH4_GPIO_CLK      RCC_APB2Periph_GPIOB
#define            PWM1_TIM_CH4_PORT          GPIOB
#define            PWM1_TIM_CH4_PIN           GPIO_Pin_1


// /**************************PWM2********************************/
// #define            PWM2_TIM                   TIM8
// #define            PWM2_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
// #define            PWM2_TIM_CLK               RCC_APB2Periph_TIM8
// #define            PWM2_TIM_Period            2000
// #define            PWM2_TIM_Prescaler         720
// // TIM8 ����Ƚ�ͨ��1
// #define            PWM2_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOC
// #define            PWM2_TIM_CH1_PORT          GPIOC
// #define            PWM2_TIM_CH1_PIN           GPIO_Pin_6

// // TIM8 ����Ƚ�ͨ��2
// #define            PWM2_TIM_CH2_GPIO_CLK      RCC_APB2Periph_GPIOC
// #define            PWM2_TIM_CH2_PORT          GPIOC
// #define            PWM2_TIM_CH2_PIN           GPIO_Pin_7

// // TIM8 ����Ƚ�ͨ��32
// #define            PWM2_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOC
// #define            PWM2_TIM_CH3_PORT          GPIOC
// #define            PWM2_TIM_CH3_PIN           GPIO_Pin_8

// // TIM8 ����Ƚ�ͨ��4
// #define            PWM2_TIM_CH4_GPIO_CLK      RCC_APB2Periph_GPIOC
// #define            PWM2_TIM_CH4_PORT          GPIOC
// #define            PWM2_TIM_CH4_PIN           GPIO_Pin_9

/**************************��������********************************/

void PWM1_TIM_Init(uint16_t CCR1_Val , uint16_t CCR2_Val , uint16_t CCR3_Val , uint16_t CCR4_Val);
// void PWM2_TIM_Init(uint16_t CCR1_Val , uint16_t CCR2_Val , uint16_t CCR3_Val , uint16_t CCR4_Val);

#endif	/* __BSP_PWM_H */


