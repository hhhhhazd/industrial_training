#include "bsp_ppm.h"

//����8��PWMͨ��
uint16_t Channel_x[9]={1000,1000,500,1000,1000,1000,1000,1000,1000};

void PPM_TIME_Init(void)
{
    GPIO_InitTypeDef            GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef            NVIC_InitStructure;

    PPM_RCC_Periph_COM(PPM_RCC_Periph, ENABLE); //ʹ��PPM IO��ʱ��
    PPM_TIM_APBxClock_FUN(PPM_TIM_CLK,ENABLE);  //ʹ��TIM6ʱ��
    
    //GPIO�ṹ���ʼ��
    GPIO_InitStructure.GPIO_Pin = PPM_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PPM_GPIO_PORT , &GPIO_InitStructure);
    GPIO_SetBits(PPM_GPIO_PORT,PPM_PIN);

    /* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
    // ARR ���Զ���װ�ؼĴ�����ֵ
    // CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
    // PWM �źŵ����� T = (ARR+1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 72M
    // ռ�ձ�P=CCR/(ARR+1)

    //��ʱ��2�ṹ���ʼ��
    TIM_TimeBaseInitStructure.TIM_Period = PPM_TIM_Period-1; 	//�Զ���װ��ֵ
    TIM_TimeBaseInitStructure.TIM_Prescaler=PPM_TIM_Prescaler-1;  //��ʱ����Ƶ
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    TIM_TimeBaseInit(PPM_TIM ,&TIM_TimeBaseInitStructure);//��ʼ��TIM6

    TIM_ITConfig(PPM_TIM ,TIM_IT_Update,ENABLE); //����ʱ��6�����ж�
    TIM_Cmd(PPM_TIM ,ENABLE); //ʹ�ܶ�ʱ��6

    NVIC_InitStructure.NVIC_IRQChannel=PPM_TIM_IRQn; //��ʱ��6�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; //��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=0; //�����ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

u8 ppm_count = 1;
u32 time;

//��ʱ��6�жϷ������
void PPM_TIME_IRQHandler(void)
{
    TIM_ClearITPendingBit(PPM_TIM, TIM_IT_Update);
    switch(ppm_count)
    {
    case 1:
        PPM_Out_H;
        time=Channel_x[1];
        TIM_SetAutoreload(PPM_TIM,Channel_x[1]);
        ppm_count++;
        break;
    case 2:
        PPM_Out_L;
        TIM_SetAutoreload(PPM_TIM,499);
        ppm_count++;
        break;
    case 3:
        PPM_Out_H;
        time+=Channel_x[2];
        TIM_SetAutoreload(PPM_TIM,Channel_x[2]);
        ppm_count++;
        break;
    case 4:
        PPM_Out_L;
        TIM_SetAutoreload(PPM_TIM,499);
        ppm_count++;
        break;
    case 5:
        PPM_Out_H;
        time+=Channel_x[3];
        TIM_SetAutoreload(PPM_TIM,Channel_x[3]);
        ppm_count++;
        break;
    case 6:
        PPM_Out_L;
        TIM_SetAutoreload(PPM_TIM,499);
        ppm_count++;
        break;
    case 7:
        PPM_Out_H;
        time+=Channel_x[4];
        TIM_SetAutoreload(PPM_TIM,Channel_x[4]);
        ppm_count++;
        break;
    case 8:
        PPM_Out_L;
        TIM_SetAutoreload(PPM_TIM,499);
        ppm_count++;
        break;
    case 9:
        PPM_Out_H;
        time+=Channel_x[5];
        TIM_SetAutoreload(PPM_TIM,Channel_x[5]);
        ppm_count++;
        break;
    case 10:
        PPM_Out_L;
        TIM_SetAutoreload(PPM_TIM,499);
        ppm_count++;
        break;
    case 11:
        PPM_Out_H;
        time+=Channel_x[6];
        TIM_SetAutoreload(PPM_TIM,Channel_x[6]);
        ppm_count++;
        break;
    case 12:
        PPM_Out_L;
        TIM_SetAutoreload(PPM_TIM,499);
        ppm_count++;
        break;
    case 13:
        PPM_Out_H;
        time+=Channel_x[7];
        TIM_SetAutoreload(PPM_TIM,Channel_x[7]);
        ppm_count++;
        break;
    case 14:
        PPM_Out_L;
        TIM_SetAutoreload(PPM_TIM,499);
        ppm_count++;
        break;
    case 15:
        PPM_Out_H;
        time+=Channel_x[8];
        TIM_SetAutoreload(PPM_TIM,Channel_x[8]);
        ppm_count++;
        break;
    case 16:
        PPM_Out_L;
        TIM_SetAutoreload(PPM_TIM,499);
        ppm_count++;
        break;		
    case 17:
        PPM_Out_H;
        TIM_SetAutoreload(PPM_TIM,15500-time);
        ppm_count++;
        break;
    case 18:
        PPM_Out_L;
        TIM_SetAutoreload(PPM_TIM,499);
        time=0;
        ppm_count=1;
    }

    TIM_ClearITPendingBit(PPM_TIM,TIM_IT_Update);  //����жϱ�־λ
}
